/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class MySamplerAudioProcessor : public juce::AudioProcessor
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

	const int uiMargin = 2.5;

	// Classes
	const juce::Font myFont = juce::Font(15, juce::Font::FontStyleFlags::plain);
	const juce::Font myFontSmall = juce::Font(12, juce::Font::FontStyleFlags::plain);
	const juce::Font myFontTiny = juce::Font(10, juce::Font::FontStyleFlags::plain);
	const juce::Font headerDisplayFont = juce::Font(15, juce::Font::FontStyleFlags::plain);
	
	juce::MidiKeyboardState midiKeyboardState;
	juce::AudioProcessorValueTreeState valueTreeState;
	
private:
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MySamplerAudioProcessor)
};
