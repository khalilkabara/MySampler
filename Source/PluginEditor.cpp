/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MySamplerAudioProcessorEditor::MySamplerAudioProcessorEditor(MySamplerAudioProcessor& p):
	AudioProcessorEditor(&p),
	audioProcessor(p),
	headerComponent(p),
	footerComponent(p),
	scopeComponent(p),
	samplerControlsComponent(p),
	midiKeyboardComponent(p.midiKeyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
	// setLookAndFeel(&mLookAndFeel);

	setSize(samplerWidth, samplerHeight);

	defineRects();

	headerComponent.setSize(headerRect.getWidth(), headerRect.getHeight());
	headerComponent.build();
	headerComponent.setBounds(headerRect);
	addAndMakeVisible(&headerComponent);

	scopeComponent.setSize(scopeRect.getWidth(), scopeRect.getHeight());
	scopeComponent.build();
	scopeComponent.setBounds(scopeRect);
	addAndMakeVisible(&scopeComponent);

	samplerControlsComponent.setSize(samplerControlsRect.getWidth(), samplerControlsRect.getHeight());
	samplerControlsComponent.build();
	samplerControlsComponent.setBounds(samplerControlsRect);
	addAndMakeVisible(&samplerControlsComponent);

	footerComponent.setSize(footerRect.getWidth(), footerRect.getHeight());
	footerComponent.build();
	footerComponent.setBounds(footerRect);
	addAndMakeVisible(&footerComponent);

	midiKeyboardComponent.setSize(midiKeyboardRect.getWidth(), midiKeyboardRect.getHeight());
	midiKeyboardComponent.setKeyWidth(midiKeyboardRect.getWidth() / 36.0f);
	midiKeyboardComponent.setLowestVisibleKey(36);
	midiKeyboardComponent.setBounds(midiKeyboardRect);
	midiKeyboardComponent.setColour(MidiKeyboardComponent::keyDownOverlayColourId, Colours::mediumturquoise);
	midiKeyboardComponent.setColour(MidiKeyboardComponent::mouseOverKeyOverlayColourId, Colours::whitesmoke);
	addAndMakeVisible(midiKeyboardComponent);
}

MySamplerAudioProcessorEditor::~MySamplerAudioProcessorEditor()
{
}

//==============================================================================
void MySamplerAudioProcessorEditor::paint(juce::Graphics& g)
{
}

void MySamplerAudioProcessorEditor::resized()
{
}

void MySamplerAudioProcessorEditor::defineRects()
{
	localBounds = juce::Rectangle<int>(border,
	                                   border,
	                                   getLocalBounds().getWidth() - border * 2,
	                                   getLocalBounds().getHeight() - border * 2);


	headerRect = juce::Rectangle<int>(localBounds.getX(),
	                                  localBounds.getY(),
	                                  localBounds.getWidth(),
	                                  localBounds.getHeight() / 10);

	scopeRect = juce::Rectangle<int>(headerRect.getX(),
	                                 headerRect.getY() + headerRect.getHeight(),
	                                 headerRect.getWidth(),
	                                 localBounds.getHeight() * 3 / 10);

	samplerControlsRect = juce::Rectangle<int>(scopeRect.getX(),
	                                           scopeRect.getY() + scopeRect.getHeight(),
	                                           scopeRect.getWidth(),
	                                           localBounds.getHeight() * 3 / 10);

	footerRect = juce::Rectangle<int>(samplerControlsRect.getX(),
	                                  samplerControlsRect.getY() + samplerControlsRect.getHeight(),
	                                  samplerControlsRect.getWidth(),
	                                  localBounds.getHeight() * 1 / 10);

	midiKeyboardRect = juce::Rectangle<int>(footerRect.getX(),
	                                        footerRect.getY() + footerRect.getHeight(),
	                                        footerRect.getWidth(),
	                                        localBounds.getHeight() * 2 / 10);
}
