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
	
	for (auto i = 0; i < bufferHistoryLength; ++i)
	{
		leftBufferHistory.add(0);
		rightBufferHistory.add(0);
	}
}

MySamplerAudioProcessor::~MySamplerAudioProcessor()
{
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
void MySamplerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void MySamplerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//######################################################################################################################################

void MySamplerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	midiKeyboardState.reset();
	midiKeyboardState.addListener(this);
}

void MySamplerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{


	buffer.clear();
	midiKeyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
	// mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
	
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

void MySamplerAudioProcessor::createStateTrees()
{
	
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new MySamplerAudioProcessor();
}
