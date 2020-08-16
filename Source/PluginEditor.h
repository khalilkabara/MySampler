/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/FooterComponent.h"
#include "Components/HeaderComponent.h"
#include "Components/SamplerControlsComponent.h"
#include "Components/ScopeComponent.h"

//==============================================================================
/**
*/
class MySamplerAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	MySamplerAudioProcessorEditor(MySamplerAudioProcessor&);
	~MySamplerAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

	const int samplerWidth = 500;
	const int samplerHeight = 400;
	const int border = 10;

private:

	juce::Rectangle<int> localBounds;
	juce::Rectangle<int> headerRect;
	juce::Rectangle<int> footerRect;
	juce::Rectangle<int> scopeRect;
	juce::Rectangle<int> samplerControlsRect;
	juce::Rectangle<int> midiKeyboardRect;

	MySamplerAudioProcessor& audioProcessor;
	HeaderComponent headerComponent;
	FooterComponent footerComponent;
	ScopeComponent scopeComponent;
	SamplerControlsComponent samplerControlsComponent;
	
	MidiKeyboardComponent midiKeyboardComponent;
	
	// Functions
	void defineRects();
	
	void hostMIDIControllerIsAvailable(bool controllerIsAvailable) override
	{
		midiKeyboardComponent.setVisible(!controllerIsAvailable);
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MySamplerAudioProcessorEditor)
};
