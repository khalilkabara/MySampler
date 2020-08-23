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
	  ), valueTreeState(*this, nullptr)
#endif
{
	createStateTrees();
	valueTreeState.state = ValueTree(SAVED_PARAMS_NAME);

	// HeaderComponent::displayText = currentlyLoadedFilePath;

	mSampler.clearVoices();
	mSampler.clearSounds();
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
	valueTreeState.state.getOrCreateChildWithName(lastLoadedFilePathParamName, nullptr)
	              .setProperty(lastLoadedFilePathParamName, currentlyLoadedFilePath, nullptr);

	std::unique_ptr<XmlElement> xml(valueTreeState.state.createXml());
	copyXmlToBinary(*xml, destData);
}

void MySamplerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	const std::unique_ptr<XmlElement> loadedParams(getXmlFromBinary(data, sizeInBytes));
	if (loadedParams != nullptr)
	{
		if (loadedParams->hasTagName(valueTreeState.state.getType()))
		{
			valueTreeState.state = ValueTree::fromXml(*loadedParams);
			currentlyLoadedFilePath = valueTreeState.state
			                                        .getOrCreateChildWithName(lastLoadedFilePathParamName, nullptr)
			                                        .getProperty(lastLoadedFilePathParamName);

			if (currentlyLoadedFilePath.isNotEmpty())
			{
				noFileLoadedYet = false;
				newFileLoaded = true;
				loadFile(currentlyLoadedFilePath);
			}
		}
	}
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

	MidiMessage midiMessage;
	MidiBuffer::Iterator midiIterator{midiMessages};
	auto sample{0};

	while (midiIterator.getNextEvent(midiMessage, sample))
	{
		if (midiMessage.isNoteOn()) mIsNotePlayed = true;
		else if (midiMessage.isNoteOff())
		{
			mIsNotePlayed = false;
			if (restartOnKeyUp)lastPlaybackPosition = 0;
		}
	}

	HeaderComponent::displayText = static_cast<String>(lastPlaybackPosition);

	const auto samplesThisTime = juce::jmin(buffer.getNumSamples(),
	                                        loadedFileWaveform.getNumSamples() - lastPlaybackPosition);

	if (mIsNotePlayed)
	{
		for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
		{
			buffer.copyFrom(channel,
			                0,
			                loadedFileWaveform,
			                channel % loadedFileWaveform.getNumChannels(),
			                lastPlaybackPosition,
			                samplesThisTime);
		}

		lastPlaybackPosition += samplesThisTime;

		if (lastPlaybackPosition == loadedFileWaveform.getNumSamples())
			lastPlaybackPosition = 0;
	}

	// mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
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

	// shutdownAudio();
	clearLoadedWaveform();

	currentlyLoadedFile = file;
	currentlyLoadedFilePath = file.getFullPathName();
	audioFormatReader = audioFormatManager.createReaderFor(file);
	loadedFileNumSamples = static_cast<float>(audioFormatReader->lengthInSamples);
	loadedSampleLengthSecs = loadedFileNumSamples / audioFormatReader->sampleRate;
	if (loadedSampleLengthSecs > maxAllowedSampleLengthSecs) loadedSampleLengthSecs = maxAllowedSampleLengthSecs;

	HeaderComponent::displayText = static_cast<String>(loadedFileNumSamples);

	loadedFileWaveform.setSize(2, static_cast<int>(loadedFileNumSamples));

	audioFormatReader->read(&loadedFileWaveform, 0, static_cast<int>(loadedFileNumSamples),
	                        0, true, true);

	lastPlaybackPosition = 0;

	BigInteger midiNotesRange;
	midiNotesRange.setRange(0, 128, true);

	mSampler.addSound(new SamplerSound(loadedSampleName, *audioFormatReader, midiNotesRange,
	                                   midiNoteForC3, samplerAttackTime, samplerReleaseTime, loadedSampleLengthSecs));

	// currentlyLoadedFilePath.isNotEmpty()
	// 	? HeaderComponent::setDisplayText(currentlyLoadedFilePath)
	// 	: HeaderComponent::setDisplayText("Empty");

	valueTreeState.state.getOrCreateChildWithName(lastLoadedFilePathParamName, nullptr)
	              .setProperty(lastLoadedFilePathParamName, currentlyLoadedFilePath, nullptr);

	noFileLoadedYet = false;
	newFileLoaded = true;
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
		if (auto sound = dynamic_cast<SamplerSound*>(mSampler.getSound(i).get()))
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
