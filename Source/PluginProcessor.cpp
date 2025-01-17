/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

#include "MySamplerSound.h"
#include "MySamplerVoice.h"
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
	valueTreeState.state = ValueTree(SAVED_PARAMS_NAME);

	// HeaderComponent::displayText = currentlyLoadedFilePath;

	mSampler.clearVoices();
	mSampler.clearSounds();
	mSampler.setNoteStealingEnabled(static_cast<int>(*valueTreeState.getRawParameterValue(noteStealingStateName)));

	audioFormatManager.registerBasicFormats();

	for (auto i = 0; i < numVoices; ++i)
	{
		// mSampler.addVoice(new MySamplerVoice(*this, lastSampleRate));
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

//=================================PERSISTENCE=============================================
void MySamplerAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	valueTreeState.state.getOrCreateChildWithName(lastLoadedFilePathParamName, nullptr)
	              .setProperty(lastLoadedFilePathParamName, currentlyLoadedFilePath, nullptr);

	// valueTreeState.state.getOrCreateChildWithName(numVoicesStateName, nullptr)
	//               .setProperty(numVoicesStateName, static_cast<String>(numVoices), nullptr);

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
			                                        .getProperty(lastLoadedFilePathParamName, "");

			if (currentlyLoadedFilePath.isNotEmpty())
			{
				loadFile(currentlyLoadedFilePath);
			}

			// const String numVoicesString = valueTreeState.state
			//                                              .getOrCreateChildWithName(numVoicesStateName, nullptr)
			//                                              .getProperty(numVoicesStateName, 0);
			//
			// numVoices = stringToInt(numVoicesString);

			numVoices = *valueTreeState.getRawParameterValue(numVoicesStateName);

			mSampler.clearVoices();

			for (auto i = 0; i < numVoices; ++i)
			{
				mSampler.addVoice(new SamplerVoice());
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
	if (noFileLoadedYet) return;

	dsp::AudioBlock<float> block(buffer);

	midiKeyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
	ScopedNoDenormals noDenormals;

	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

	// MidiMessage midiMessage;
	// MidiBuffer::Iterator midiIterator{midiMessages};
	// auto samplePos{0};
	//
	// while (midiIterator.getNextEvent(midiMessage, samplePos))
	// {
	// 	if (midiMessage.isNoteOn()) mIsNotePlayed = true;
	// 	else if (midiMessage.isNoteOff())
	// 	{
	// 		mIsNotePlayed = false;
	// 		lastPlaybackPosition = 0;
	// 	}
	// }

	const auto timeVal = jmap<float>(lastMidiNoteHertz, frequencyOfLowestKey,
	                                 frequencyOfHighestKey, 0, 1);

	const auto samplesThisTime = static_cast<float>(buffer.getNumSamples()) * timeVal;

	if (mIsNotePlayed)
	{
		// for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
		// {
		// 	buffer.addFrom(channel,
		// 	               0,
		// 	               loadedFileWaveform,
		// 	               channel % loadedFileWaveform.getNumChannels(),
		// 	               lastPlaybackPosition,
		// 	               samplesThisTime);
		// }

		lastPlaybackPosition += samplesThisTime;

	}

	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

	for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
	{
		buffer.clear(i, 0, buffer.getNumSamples());
	}

	mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

	processEffects(buffer, dsp::ProcessContextReplacing<float>(block));

	// auto position = buffer.
	//
	// HeaderComponent::displayText = static_cast<String>(position);

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

void MySamplerAudioProcessor::loadFile(const File& file)
{
	const WildcardFileFilter fileFormatFilter(audioFormatManager.getWildcardForAllFormats(), {}, {});
	if (!fileFormatFilter.isFileSuitable(file)) return;

	clearLoadedWaveform();

	currentlyLoadedFile = file;
	currentlyLoadedFilePath = file.getFullPathName();
	audioFormatReader = audioFormatManager.createReaderFor(file);
	loadedFileNumSamples = static_cast<float>(audioFormatReader->lengthInSamples);
	loadedSampleLengthSecs = loadedFileNumSamples / audioFormatReader->sampleRate;
	// loadedSampleLengthSecs = jmin(static_cast<float>(loadedSampleLengthSecs), maxAllowedSampleLengthSecs);

	loadedFileWaveform.setSize(2, static_cast<int>(loadedFileNumSamples));

	audioFormatReader->read(&loadedFileWaveform, 0, static_cast<int>(loadedFileNumSamples),
	                        0, true, true);

	lastPlaybackPosition = 0;

	BigInteger midiNotesRange;
	midiNotesRange.setRange(0, 128, true);

	mSampler.addSound(new MySamplerSound(loadedSampleName, *audioFormatReader, midiNotesRange,
	                                     midiNoteForC3, samplerAttackTime, samplerReleaseTime, loadedSampleLengthSecs));

	// currentlyLoadedFilePath.isNotEmpty()
	// 	? HeaderComponent::setDisplayText(currentlyLoadedFilePath)
	// 	: HeaderComponent::setDisplayText("Empty");

	valueTreeState.state.getOrCreateChildWithName(lastLoadedFilePathParamName, &undoManager)
	              .setProperty(lastLoadedFilePathParamName, currentlyLoadedFilePath, &undoManager);

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

	filter.reset();
	filter.prepare(spec);

	// lfo.reset();
}

void MySamplerAudioProcessor::updateEffects()
{
	ampVolume = static_cast<float>(*valueTreeState.getRawParameterValue(ampVolumeStateName));
	ampPan.setPan(static_cast<float>(*valueTreeState.getRawParameterValue(ampPanStateName)));

	ampEnvelopeParams.attack = static_cast<float>(*valueTreeState.getRawParameterValue(envelopeAttackStateName));
	ampEnvelopeParams.decay = static_cast<float>(*valueTreeState.getRawParameterValue(envelopeDecayStateName));
	ampEnvelopeParams.sustain = static_cast<float>(*valueTreeState.getRawParameterValue(envelopeSustainStateName));
	ampEnvelopeParams.release = static_cast<float>(*valueTreeState.getRawParameterValue(envelopeReleaseStateName));
	ampEnvelope.setParameters(ampEnvelopeParams);

	for (auto i = 0; i < mSampler.getNumSounds(); ++i)
	{
		if (auto sound = dynamic_cast<SamplerSound*>(mSampler.getSound(i).get()))
		{
			sound->setEnvelopeParameters(ampEnvelopeParams);
		}
	}

	const auto filterType = static_cast<int>(*valueTreeState.getRawParameterValue(filterTypeStateName));
	if (filterType == FILTER_TYPES.indexOf(FILTER_LOW_PASS))filter.setType(dsp::StateVariableTPTFilterType::lowpass);
	if (filterType == FILTER_TYPES.indexOf(FILTER_HIGH_PASS))filter.setType(dsp::StateVariableTPTFilterType::highpass);
	if (filterType == FILTER_TYPES.indexOf(FILTER_BAND_PASS))filter.setType(dsp::StateVariableTPTFilterType::bandpass);

	filter.setCutoffFrequency(static_cast<float>(*valueTreeState.getRawParameterValue(filterCutoffStateName)));
	filter.setResonance(static_cast<float>(*valueTreeState.getRawParameterValue(filterResonanceStateName)));
}

void MySamplerAudioProcessor::processEffects(AudioBuffer<float>& buffer, dsp::ProcessContextReplacing<float> dspContext)
{
	updateEffects();

	const auto filterType = static_cast<int>(*valueTreeState.getRawParameterValue(filterTypeStateName));
	if (!filterType == FILTER_TYPES.indexOf(FILTER_NONE)) filter.process(dspContext);

	ampPan.process(dspContext);

	for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		buffer.applyGain(channel, 0, buffer.getNumSamples(), ampVolume);
	}
}

void MySamplerAudioProcessor::resetNumVoices(int nVoices)
{
	numVoices = nVoices;

	// mSampler.clearSounds();
	// loadFile(currentlyLoadedFilePath);
	mSampler.clearVoices();

	for (auto i = 0; i < numVoices; ++i)
	{
		// mSampler.addVoice(new MySamplerVoice(*this, lastSampleRate));
		mSampler.addVoice(new SamplerVoice());
	}

	mSampler.setCurrentPlaybackSampleRate(lastSampleRate);
}

void MySamplerAudioProcessor::clearFile()
{
	clearLoadedWaveform();
	currentlyLoadedFilePath = "";
	mSampler.clearSounds();

	noFileLoadedYet = true;
	newFileLoaded = false;
}

void MySamplerAudioProcessor::resetSampler()
{
	clearFile();

	const ValueTree state = valueTreeState.copyState();
	const std::unique_ptr<XmlElement> tempXml(state.createXml());

	forEachXmlChildElementWithTagName(*tempXml, child, "PARAM")
	{
		const float defaultValue = valueTreeState.getParameter(child->getStringAttribute("id"))->getDefaultValue();
		const auto range = valueTreeState.getParameter(child->getStringAttribute("id"))->getNormalisableRange();
		const auto unRangedValue = jmap(defaultValue, range.start, range.end);
		child->setAttribute("value", unRangedValue);
	}

	valueTreeState.replaceState(ValueTree::fromXml(*tempXml));
}

void MySamplerAudioProcessor::setNoteStealing(bool canStealNotes)
{
	mSampler.setNoteStealingEnabled(canStealNotes);

	// HeaderComponent::setDisplayText(mSampler.isNoteStealingEnabled() ? "True" : "False");
}

void MySamplerAudioProcessor::createStateTrees()
{
	// Voices
	using Parameter = AudioProcessorValueTreeState::Parameter;

	NormalisableRange<float> numVoicesParamRange{static_cast<float>(minVoices), static_cast<float>(maxVoices)};
	NormalisableRange<float> toggleParamRange{static_cast<float>(toggleOffValue), static_cast<float>(toggleOnValue)};

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(numVoicesStateName,
	                                                                 numVoicesStateName,
	                                                                 numVoicesStateName,
	                                                                 numVoicesParamRange, defaultVoices,
	                                                                 [](float val) { return String(val, 0); },
	                                                                 [](String s) { return s.getIntValue(); }));

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(noteStealingStateName,
	                                                                 noteStealingStateName,
	                                                                 noteStealingStateName,
	                                                                 toggleParamRange, toggleOnValue,
	                                                                 [](float val) { return String(val, 0); },
	                                                                 [](String s) { return s.getIntValue(); }));

	// Vol & Pan
	NormalisableRange<float> envelopeParamRange{zeroToTenMinValue, zeroToTenMaxValue};
	NormalisableRange<float> zeroToOneParamRange{zeroToOneMinValue, zeroToOneMaxValue};
	NormalisableRange<float> panParamRange{panMinValue, panMaxValue};
	NormalisableRange<float> bipolarParamRange{bipolarMinValue, bipolarMaxValue};

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(ampVolumeStateName,
	                                                                 ampVolumeStateName,
	                                                                 ampVolumeStateName,
	                                                                 zeroToOneParamRange,
	                                                                 zeroToOneDefaultValue,
	                                                                 [](float val) { return String(val, 2); },
	                                                                 nullptr));

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(ampPanStateName,
	                                                                 ampPanStateName,
	                                                                 ampPanStateName,
	                                                                 panParamRange,
	                                                                 panDefaultValue,
	                                                                 [](float val) { return String(val, 2); },
	                                                                 nullptr));

	// Envelope

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(envelopeAttackStateName,
	                                                                 envelopeAttackStateName,
	                                                                 envelopeAttackStateName,
	                                                                 envelopeParamRange,
	                                                                 zeroToTenMinValue,
	                                                                 [](float val) { return String(val, 2); },
	                                                                 nullptr));

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(envelopeDecayStateName,
	                                                                 envelopeDecayStateName,
	                                                                 envelopeDecayStateName,
	                                                                 envelopeParamRange,
	                                                                 zeroToTenDefaultValue,
	                                                                 [](float val) { return String(val, 2); },
	                                                                 nullptr));

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(envelopeSustainStateName,
	                                                                 envelopeSustainStateName,
	                                                                 envelopeSustainStateName,
	                                                                 zeroToOneParamRange,
	                                                                 zeroToOneDefaultValue,
	                                                                 [](float val) { return String(val, 2); },
	                                                                 nullptr));

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(envelopeReleaseStateName,
	                                                                 envelopeReleaseStateName,
	                                                                 envelopeReleaseStateName,
	                                                                 envelopeParamRange,
	                                                                 releaseDefaultValue,
	                                                                 [](float val) { return String(val, 2); },
	                                                                 nullptr));

	// Filter
	NormalisableRange<float> filterTypeParam(0, FILTER_TYPES.size() - 1);
	NormalisableRange<float> filterCutoffParam(filterCutoffMinValue, filterCutoffMaxValue);
	NormalisableRange<float> filterResonanceParam(filterResonanceMinValue, filterResonanceMaxValue);

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(filterTypeStateName, filterTypeStateName,
	                                                                 filterTypeStateName, filterTypeParam, 0,
	                                                                 [](float val) { return String(val, 2); },
	                                                                 nullptr));

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(filterCutoffStateName, filterCutoffStateName,
	                                                                 filterCutoffStateName, filterCutoffParam,
	                                                                 filterCutoffDefaultValue,
	                                                                 [](float val) { return String(val, 2); },
	                                                                 nullptr));

	valueTreeState.createAndAddParameter(std::make_unique<Parameter>(filterResonanceStateName,
	                                                                 filterResonanceStateName,
	                                                                 filterResonanceStateName, filterResonanceParam,
	                                                                 filterResonanceDefaultValue,
	                                                                 [](float val) { return String(val, 2); },
	                                                                 nullptr));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new MySamplerAudioProcessor();
}
