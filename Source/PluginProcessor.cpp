/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MySamplerAudioProcessor::MySamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		  .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	  ), valueTreeState(*this, &undoManager)
#endif
{
	createStateTrees();

	// const auto tree = valueTreeState.state.getOrCreateChildWithName(lastLoadedFilePathParamName, nullptr);
	// currentlyLoadedFilePath = tree.getProperty(lastLoadedFilePathParamName, "");
	// loadFile(currentlyLoadedFilePath);

	// HeaderComponent::displayText = currentlyLoadedFilePath;

	audioFormatManager.registerBasicFormats();

	for (auto i = 0; i < numVoices; ++i)
	{
		mSampler.addVoice(new SamplerVoice());
	}

	for (auto i = 0; i < bufferHistoryLength; ++i)
	{
		leftBufferHistory.add(0);
		rightBufferHistory.add(0);
	}
}

MySamplerAudioProcessor::~MySamplerAudioProcessor()
{
	audioFormatReader = nullptr;
}

//==============================================================================
const juce::String MySamplerAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool MySamplerAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool MySamplerAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
	return false;
#endif
}

bool MySamplerAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
	return false;
#endif
}

double MySamplerAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int MySamplerAudioProcessor::getNumPrograms()
{
	return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int MySamplerAudioProcessor::getCurrentProgram()
{
	return 0;
}

void MySamplerAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String MySamplerAudioProcessor::getProgramName(int index)
{
	return {};
}

void MySamplerAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void MySamplerAudioProcessor::releaseResources()
{
	midiKeyboardState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MySamplerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

//==============================================================================
bool MySamplerAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MySamplerAudioProcessor::createEditor()
{
	return new MySamplerAudioProcessorEditor(*this);
}

//==============================================================================


//=================================PERSISTENCE=============================================
void MySamplerAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// std::unique_ptr<XmlElement> xml(valueTreeState.state.createXml());
	// copyXmlToBinary(*xml, destData);

	// const auto tree = valueTreeState.state.getOrCreateChildWithName(lastLoadedFilePathParamName, nullptr);
	// currentlyLoadedFilePath = tree.getProperty(lastLoadedFilePathParamName, "");
	// loadFile(currentlyLoadedFilePath);
}

void MySamplerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	const std::unique_ptr<XmlElement> loadedParams(getXmlFromBinary(data, sizeInBytes));
	if (loadedParams != nullptr)
		if (loadedParams->hasTagName(valueTreeState.state.getType()))
			valueTreeState.state = ValueTree::fromXml(*loadedParams);
}

//######################################################################################################################################

void MySamplerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	lastSampleRate = sampleRate;
	mSampler.setCurrentPlaybackSampleRate(sampleRate);
	midiKeyboardState.reset();
	midiKeyboardState.addListener(this);

	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getTotalNumOutputChannels();
	initializeEffects(spec);
}

void MySamplerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	dsp::AudioBlock<float> block(buffer);

	midiKeyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
	ScopedNoDenormals noDenormals;

	for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
	{
		buffer.clear(i, 0, buffer.getNumSamples());
	}

	mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

	processEffects(buffer, dsp::ProcessContextReplacing<float>(block));

	// Samples for Oscilloscope
	for (auto i = 0; i < buffer.getNumSamples(); ++i)
	{
		if (i % bufferHistoryUpdateFrequency == 0)
		{
			auto sample = buffer.getSample(0, i);
			leftBufferHistory.add(sample);
			sample = buffer.getSample(1, i);
			rightBufferHistory.add(sample);

			if (leftBufferHistory.size() > bufferHistoryLength) leftBufferHistory.remove(0);
			if (rightBufferHistory.size() > bufferHistoryLength) rightBufferHistory.remove(0);
		}
	}
}

//######################################################################################################################################

void MySamplerAudioProcessor::loadFile(const File file)
{
	const WildcardFileFilter fileFormatFilter(audioFormatManager.getWildcardForAllFormats(), {}, {});
	if (!fileFormatFilter.isFileSuitable(file)) return;

	currentlyLoadedFile = file;
	currentlyLoadedFilePath = file.getFullPathName();
	audioFormatReader = audioFormatManager.createReaderFor(file);

	// valueTreeState.state.getOrCreateChildWithName(lastLoadedFilePathParamName, nullptr)
	//               .setProperty(lastLoadedFilePathParamName, currentlyLoadedFilePath, nullptr);

	// HeaderComponent::displayText = currentlyLoadedFilePath;

	noFileLoadedYet = false;
	newFileLoaded = true;

	const auto numSamples = static_cast<int>(audioFormatReader->lengthInSamples);
	loadedFileWaveform.setSize(1, numSamples);

	audioFormatReader->read(&loadedFileWaveform, 0, numSamples,
	                        0, true, false);

	BigInteger midiNotesRange;
	midiNotesRange.setRange(0, 128, true);

	mSampler.addSound(new SamplerSound(loadedSampleName, *audioFormatReader, midiNotesRange,
	                                   midiNoteForC3, samplerAttackTime, samplerReleaseTime, maxSampleLength));
}

void MySamplerAudioProcessor::loadFile(String& filePath)
{
	// HeaderComponent::displayText = currentlyLoadedFilePath;
	if (filePath.isEmpty()) return;

	loadFile(File(filePath));
}

void MySamplerAudioProcessor::loadFile()
{
	FileChooser fileChooser{"Select Audio File", File(), allowedFileFormats};

	if (fileChooser.browseForFileToOpen())
	{
		loadFile(fileChooser.getResult());
	}
}

void MySamplerAudioProcessor::initializeEffects(dsp::ProcessSpec& spec)
{
	ampPan.setRule(dsp::PannerRule::balanced);
	ampPan.prepare(spec);

	ampEnvelope.reset();
	ampEnvelope.setSampleRate(lastSampleRate);
}

void MySamplerAudioProcessor::updateEffects()
{
	ampVolume = static_cast<float>(*valueTreeState.getRawParameterValue(ampVolumeStateName));
	ampPan.setPan(static_cast<float>(*valueTreeState.getRawParameterValue(ampPanStateName)));

	ampEnvelopeParams.attack = static_cast<float>(*valueTreeState.getRawParameterValue(envelopeAttackStateName));
	ampEnvelopeParams.decay = static_cast<float>(*valueTreeState.getRawParameterValue(envelopeSustainStateName));
	ampEnvelopeParams.sustain = static_cast<float>(*valueTreeState.getRawParameterValue(ampPanStateName));
	ampEnvelopeParams.release = static_cast<float>(*valueTreeState.getRawParameterValue(envelopeReleaseStateName));
	ampEnvelope.setParameters(ampEnvelopeParams);

	for (auto i = 0; i < mSampler.getNumSounds(); ++i)
	{
		if(auto sound = dynamic_cast<SamplerSound*>(mSampler.getSound(i).get()))
		{
			sound->setEnvelopeParameters(ampEnvelopeParams);
		}
	}
}

void MySamplerAudioProcessor::processEffects(AudioBuffer<float>& buffer, dsp::ProcessContextReplacing<float> dspContext)
{
	updateEffects();
	ampPan.process(dspContext);

	for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		buffer.applyGain(channel, 0, buffer.getNumSamples(), ampVolume);
	}
}

void MySamplerAudioProcessor::createStateTrees()
{
	using Parameter = AudioProcessorValueTreeState::Parameter;

	NormalisableRange<float> envelopeParamRange{zeroToTenMinValue, zeroToTenMaxValue};
	NormalisableRange<float> zeroToOneParamRange{zeroToOneMinValue, zeroToOneMaxValue};
	NormalisableRange<float> panParamRange{panMinValue, panMaxValue};
	NormalisableRange<float> bipolarParamRange{bipolarMinValue, bipolarMaxValue};

	// Vol & Pan
	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(ampVolumeStateName,
	                                                                 ampVolumeStateName,
	                                                                 ampVolumeStateName,
	                                                                 zeroToOneParamRange,
	                                                                 zeroToOneDefaultValue,
	                                                                 nullptr, nullptr));

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(ampPanStateName,
	                                                                 ampPanStateName,
	                                                                 ampPanStateName,
	                                                                 panParamRange,
	                                                                 panDefaultValue,
	                                                                 nullptr, nullptr));

	// Envelope

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(envelopeAttackStateName,
	                                                                 envelopeAttackStateName,
	                                                                 envelopeAttackStateName,
	                                                                 envelopeParamRange,
	                                                                 zeroToTenMinValue,
	                                                                 nullptr, nullptr));

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(envelopeDecayStateName,
	                                                                 envelopeDecayStateName,
	                                                                 envelopeDecayStateName,
	                                                                 envelopeParamRange,
	                                                                 zeroToTenDefaultValue,
	                                                                 nullptr, nullptr));

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(envelopeSustainStateName,
	                                                                 envelopeSustainStateName,
	                                                                 envelopeSustainStateName,
	                                                                 zeroToOneParamRange,
	                                                                 zeroToOneDefaultValue,
	                                                                 nullptr, nullptr));

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(envelopeReleaseStateName,
	                                                                 envelopeReleaseStateName,
	                                                                 envelopeReleaseStateName,
	                                                                 envelopeParamRange,
	                                                                 zeroToTenDefaultValue,
	                                                                 nullptr, nullptr));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new MySamplerAudioProcessor();
}
