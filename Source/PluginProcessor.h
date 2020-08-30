/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "MySampler.h"

//==============================================================================
/**
*/
class MySamplerAudioProcessor : public juce::AudioProcessor, MidiKeyboardState::Listener
{
public:
	//==============================================================================
	MySamplerAudioProcessor();
	~MySamplerAudioProcessor() override;

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	//==============================================================================
	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	//==============================================================================
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	Array<float> leftBufferHistory;
	Array<float> rightBufferHistory;
	int bufferHistoryLength = 160;
	const int bufferHistoryUpdateFrequency = 50;

	const int uiMargin = 2.5;

	// Classes
	const juce::Font myFont = juce::Font(15, juce::Font::FontStyleFlags::plain);
	const juce::Font myFontSmall = juce::Font(12, juce::Font::FontStyleFlags::plain);
	const juce::Font myFontTiny = juce::Font(10, juce::Font::FontStyleFlags::plain);
	const juce::Font headerDisplayFont = juce::Font(15, juce::Font::FontStyleFlags::plain);
	const String SAVED_PARAMS_NAME{"mySamplerSavedParams"};

	MidiKeyboardState midiKeyboardState;
	AudioProcessorValueTreeState valueTreeState;

	File currentlyLoadedFile;
	String currentlyLoadedFilePath = "";
	const String lastLoadedFilePathParamName = "lastLoadedFilePath";
	AudioFormatManager audioFormatManager;
	AudioFormatReader* audioFormatReader{nullptr};
	AudioBuffer<float> loadedFileWaveform;

	AudioBuffer<float>& getLoadedFileWaveform() { return loadedFileWaveform; }
	void clearLoadedWaveform() { loadedFileWaveform.clear(); }

	int lastMidiNoteNumber{60};
	float lastMidiNoteHertz{261.6};
	const float frequencyOfHighestKey{261.6};
	const float frequencyOfLowestKey{8};
	int numVoices{1};
	const int minVoices{0};
	const int maxVoices{9};
	const int defaultVoices{1};
	const String numVoicesStateName{"numVoices"};

	int samplerAttackTime = 0.1;
	int samplerReleaseTime = 0.1;
	float loadedFileNumSamples = 10.0;
	float loadedSampleLengthSecs;
	const float maxAllowedSampleLengthSecs = 5.0;
	int lastPlaybackPosition;
	bool mIsNotePlayed{false};
	// bool currentPlayHasEnded{false};

	double lastSampleRate;
	bool newFileLoaded = false;
	bool noFileLoadedYet = true;
	bool restartOnKeyUp = false;

	const String switchTooltip = "switch;";
	const String effectTooltip = "effect;";
	const String bipolarTooltip = "bipolar;";
	const String bipolarEffectTooltip = "bipolarEffect;";

	const String envelopeAttackStateName = "envelopeAttack";
	const String envelopeDecayStateName = "envelopeDecay";
	const String envelopeSustainStateName = "envelopeSustain";
	const String envelopeReleaseStateName = "envelopeRelease";

	const String filterTypeStateName = "filterType";
	const String filterCutoffStateName = "filterCutoff";
	const String filterResonanceStateName = "filterResonance";

	const String ampVolumeStateName = "ampVolume";
	const String ampPanStateName = "ampPan";

	const String noteStealingStateName = "noteStealing";

	const float zeroToTenMinValue = 0.0;
	const float zeroToTenMaxValue = 10.0;
	const float zeroToTenDefaultValue = 8.0;
	const float releaseDefaultValue = 0.5;
	const float zeroToTenMidpointValue = 3.0;
	const float zeroToTenStepValue = 0.1;

	const float zeroToOneMinValue = 0.0;
	const float zeroToOneMaxValue = 1.0;
	const float zeroToOneDefaultValue = 0.8;
	const float zeroToOneMidpointValue = 0.3;
	const float zeroToOneStepValue = 0.01;

	const float bipolarMinValue = -1.0;
	const float bipolarMaxValue = 1.0;
	const float bipolarDefaultValue = 0.0;
	const float bipolarMidpointValue = 0.0;
	const float bipolarStepValue = 0.01;

	const float panMinValue = -1.0;
	const float panMaxValue = 1.0;
	const float panDefaultValue = 0.0;
	const float panMidpointValue = 0.0;
	const float panStepValue = 0.01;

	const int toggleOffValue = 0;
	const int toggleOnValue = 1;
	const int toggleStepValue = 1;
	const int toggleDefaultValue = 0;

	const String FILTER_NONE = "None";
	const String FILTER_LOW_PASS = "Low Pass";
	const String FILTER_HIGH_PASS = "High Pass";
	const String FILTER_BAND_PASS = "Band Pass";
	Array<String> FILTER_TYPES{FILTER_NONE, FILTER_LOW_PASS, FILTER_HIGH_PASS, FILTER_BAND_PASS};

	const float filterCutoffMinValue = 20;
	const float filterCutoffMaxValue = 20000;
	const float filterCutoffDefaultValue = 500;
	const float filterCutoffMidpoint = 2500;
	const float filterCutoffStep = 0.1;

	const float filterResonanceMinValue = 0.1;
	const float filterResonanceMaxValue = 10;
	const float filterResonanceDefaultValue = 0.2;
	const float filterResonanceMidpoint = 4;
	const float filterResonanceStep = 0.01;

	void loadFile();
	void loadFile(const File& file);
	void loadFile(String& filePath);
	void resetNumVoices(int);
	void clearFile();
	void resetSampler();
	void setNoteStealing(bool);

	static int stringToInt(const String& str)
	{
		auto cp = str.getCharPointer();
		const auto numVal = juce::CharacterFunctions::readDoubleValue(cp);
		return static_cast<int>(numVal);
	}

private:

	UndoManager undoManager{30000, 30};

	dsp::ProcessSpec spec;
	ADSR ampEnvelope;
	ADSR::Parameters ampEnvelopeParams;
	juce::dsp::Panner<float> ampPan;
	juce::dsp::StateVariableTPTFilter<float> filter;
	// juce::dsp::Oscillator<float> lfo;
	MySampler mSampler;

	float ampVolume = 0.8;
	const int midiNoteForC3{60};
	const String loadedSampleName = "Sample";
	const String allowedFileFormats = String();
	// const String allowedFileFormats = "*.wav,*.flac,*.aiff,*.mp3,*.aac,*.ogg";

	void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override
	{
		ampEnvelope.noteOn();
		lastMidiNoteNumber = midiNoteNumber;
		lastMidiNoteHertz = MidiMessage::getMidiNoteInHertz(lastMidiNoteNumber);
		mIsNotePlayed = true;
	}

	void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override
	{
		ampEnvelope.noteOff();
		lastPlaybackPosition = 0;
		mIsNotePlayed = false;
	}

	void createStateTrees();
	void initializeEffects(dsp::ProcessSpec&);
	void updateEffects();
	void processEffects(AudioBuffer<float>& buffer, dsp::ProcessContextReplacing<float>);

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MySamplerAudioProcessor)
};
