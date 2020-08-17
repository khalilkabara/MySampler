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
#include "Components/WaveformComponent.h"

//==============================================================================

class MySamplerAudioProcessorEditor : public juce::AudioProcessorEditor, public FileDragAndDropTarget
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

	void filesDropped(const StringArray& files, int x, int y) override
	{
		processor.loadFile(files[0]);
		fileIsBeingDragged = false;
		repaint();
	}

	bool isInterestedInFileDrag(const StringArray& files) override
	{
		return true;
	}

	void fileDragEnter(const StringArray& files, int x, int y) override
	{
		fileIsBeingDragged = true;
		repaint();
	}

	void fileDragExit(const StringArray& files) override
	{
		fileIsBeingDragged = false;
		repaint();
	}

private:
	MySamplerAudioProcessor& processor;

	bool fileIsBeingDragged = false;

	Label fileDragIndicator;

	juce::Rectangle<int> fileDragIndicatorRect;
	juce::Rectangle<int> localBounds;
	juce::Rectangle<int> headerRect;
	juce::Rectangle<int> waveformRect;
	juce::Rectangle<int> footerRect;
	juce::Rectangle<int> scopeRect;
	juce::Rectangle<int> samplerControlsRect;
	juce::Rectangle<int> midiKeyboardRect;

	MySamplerAudioProcessor& audioProcessor;
	HeaderComponent headerComponent;
	WaveformComponent waveformComponent;
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
