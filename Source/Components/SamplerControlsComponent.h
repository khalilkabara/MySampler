/*
  ==============================================================================

    SamplerControlsComponent.h
    Created: 16 Aug 2020 5:29:50pm
    Author:  Khalil Kabara

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "MyComponentBase.h"

//==============================================================================

class SamplerControlsComponent : public MyComponentBase, public Slider::Listener, public Button::Listener
{
public:

	SamplerControlsComponent(MySamplerAudioProcessor& p) : MyComponentBase(p)
	{
		// controlsSectionImage = ImageCache::getFromMemory(BinaryData::reverb_section_art_png,
		//                                                BinaryData::reverb_section_art_pngSize);
		//
		// reverbSwitchAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.valueTreeState,
		//                                                                                 processor.reverbSwitchStateName,
		//                                                                                 reverbSwitchSlider));
		//
		// reverbWetAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.valueTreeState,
		//                                                                              processor.reverbWetStateName,
		//                                                                              reverbWetKnob));
		//
		// reverbSizeAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.valueTreeState,
		//                                                                               processor.reverbSizeStateName,
		//                                                                               reverbSizeKnob));
		//
		// reverbWidthAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.valueTreeState,
		//                                                                                processor.reverbWidthStateName,
		//                                                                                reverbWidthKnob));
		//
		// reverbDampAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.valueTreeState,
		//                                                                               processor.reverbDampStateName,
		//                                                                               reverbDampKnob));
	}

	~SamplerControlsComponent()
	{
	}

	void paint(Graphics& g) override
	{
		mPaint(g, controlsSectionImage);
		
		// g.fillAll(Colours::aqua);
		g.drawFittedText("Controls", getLocalBounds(), Justification::centred, 1);
		
		//
		// g.drawFittedText("Reverb", reverbTitleLabelRect, Justification::centred, 1);
		//
		// g.setFont(processor.myFontTiny);
		//
		// g.drawFittedText("Mix", reverbWetLabelRect, Justification::centred, 1);
		// g.drawFittedText("Size", reverbSizeLabelRect, Justification::centred, 1);
		// g.drawFittedText("Width", reverbWidthLabelRect, Justification::centred, 1);
		// g.drawFittedText("Damp", reverbDampLabelRect, Justification::centred, 1);

		if (debugBoundRects)
			g.drawRect(getLocalBounds());

		// g.drawRect(reverbTitleLabelRect);
		// g.drawRect(reverbSwitchRect);
		// g.drawRect(reverbWetLabelRect);
		// g.drawRect(reverbSizeLabelRect);
		// g.drawRect(reverbWidthLabelRect);
		// g.drawRect(reverbDampLabelRect);
	}

	void resized() override
	{
	}

	void buttonClicked(Button*) override
	{
	}

	void buttonStateChanged(Button* button) override
	{
		if (button == &reverbSwitchButton)
		{
			reverbSwitchButton.getToggleState() ? reverbSwitchSlider.setValue(1) : reverbSwitchSlider.setValue(0);
		}
	}

private:

	// Functions

	void sliderValueChanged(Slider* slider) override
	{
	}

	void defineRects() override
	{
		MyComponentBase::defineRects();

		reverbTitleLabelRect = juce::Rectangle<int>(
			localBounds.getX(),
			localBounds.getY(),
			localBounds.getWidth(),
			localBounds.getHeight() / 5);

		reverbSwitchRect = juce::Rectangle<int>(
			localBounds.getX() + labelRectHeight / 2,
			localBounds.getY() + labelRectHeight / 2,
			reverbTitleLabelRect.getHeight() - labelRectHeight / 2,
			reverbTitleLabelRect.getHeight() - labelRectHeight / 2);

		reverbControlsRect = juce::Rectangle<int>(
			reverbTitleLabelRect.getX(),
			reverbTitleLabelRect.getY() + reverbTitleLabelRect.getHeight(),
			localBounds.getWidth(),
			localBounds.getHeight() - reverbTitleLabelRect.getHeight());

		reverbWetKnobRect = juce::Rectangle<int>(
			reverbControlsRect.getX(),
			reverbControlsRect.getY(),
			reverbControlsRect.getWidth() / 2,
			reverbControlsRect.getHeight() / 2 - labelRectHeight);

		reverbSizeKnobRect = juce::Rectangle<int>(
			reverbWetKnobRect.getX() + reverbWetKnobRect.getWidth(),
			reverbWetKnobRect.getY(),
			reverbWetKnobRect.getWidth(),
			reverbWetKnobRect.getHeight());

		reverbWidthKnobRect = juce::Rectangle<int>(
			reverbWetKnobRect.getX(),
			reverbWetKnobRect.getY() + reverbWetKnobRect.getHeight() + labelRectHeight,
			reverbWetKnobRect.getWidth(),
			reverbWetKnobRect.getHeight());

		reverbDampKnobRect = juce::Rectangle<int>(
			reverbWidthKnobRect.getX() + reverbWidthKnobRect.getWidth(),
			reverbWidthKnobRect.getY(),
			reverbWidthKnobRect.getWidth(),
			reverbWidthKnobRect.getHeight());

		// Labels
		reverbWetLabelRect = juce::Rectangle<int>(
			reverbWetKnobRect.getX(),
			reverbWetKnobRect.getY() + reverbWetKnobRect.getHeight(),
			reverbWetKnobRect.getWidth(),
			labelRectHeight);

		reverbSizeLabelRect = juce::Rectangle<int>(
			reverbWetLabelRect.getX() + reverbWetLabelRect.getWidth(),
			reverbWetLabelRect.getY(),
			reverbWetLabelRect.getWidth(),
			reverbWetLabelRect.getHeight());

		reverbWidthLabelRect = juce::Rectangle<int>(
			reverbWidthKnobRect.getX(),
			reverbWidthKnobRect.getY() + reverbWidthKnobRect.getHeight(),
			reverbWidthKnobRect.getWidth(),
			labelRectHeight);

		reverbDampLabelRect = juce::Rectangle<int>(
			reverbWidthLabelRect.getX() + reverbWidthLabelRect.getWidth(),
			reverbWidthLabelRect.getY(),
			reverbWidthLabelRect.getWidth(),
			reverbWidthLabelRect.getHeight());
	}

	void defineComponents() override
	{
		// Switch
		
		// reverbSwitchSlider.setRange(0, 1, 1);
		// reverbSwitchSlider.setVisible(false);
		// reverbSwitchSlider.addListener(this);
		//
		// reverbSwitchButton.setBounds(reverbSwitchRect);
		// const auto reverbIsEnabled = static_cast<int>(*processor.valueTreeState.getRawParameterValue(
		// 	processor.reverbSwitchStateName)) == 1;
		// reverbSwitchButton.setToggleState(reverbIsEnabled, NotificationType::dontSendNotification);
		// reverbSwitchButton.setVisible(true);
		// reverbSwitchButton.addListener(this);
		//
		//
		// reverbWetKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		// reverbWetKnob.setRange(processor.volumeMinValue, processor.volumeMaxValue, processor.volumeStepValue);
		// reverbWetKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
		// reverbWetKnob.setBounds(reverbWetKnobRect);
		// reverbWetKnob.setSkewFactorFromMidPoint(processor.effectMidpointValue);
		// reverbWetKnob.setTooltip(translate("effect;"));
		// reverbWetKnob.setVisible(true);
		// reverbWetKnob.addListener(this);
		//
		// reverbSizeKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		// reverbSizeKnob.setRange(processor.volumeMinValue, processor.volumeMaxValue, processor.volumeStepValue);
		// reverbSizeKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
		// reverbSizeKnob.setBounds(reverbSizeKnobRect);
		// reverbSizeKnob.setSkewFactorFromMidPoint(processor.effectMidpointValue);
		// reverbSizeKnob.setTooltip(translate("effect;"));
		// reverbSizeKnob.setVisible(true);
		// reverbSizeKnob.addListener(this);
		//
		// reverbWidthKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		// reverbWidthKnob.setRange(processor.volumeMinValue, processor.volumeMaxValue, processor.volumeStepValue);
		// reverbWidthKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
		// reverbWidthKnob.setBounds(reverbWidthKnobRect);
		// reverbWidthKnob.setSkewFactorFromMidPoint(processor.effectMidpointValue);
		// reverbWidthKnob.setTooltip(translate("effect;"));
		// reverbWidthKnob.setVisible(true);
		// reverbWidthKnob.addListener(this);
		//
		// reverbDampKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		// reverbDampKnob.setRange(processor.volumeMinValue, processor.volumeMaxValue, processor.volumeStepValue);
		// reverbDampKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
		// reverbDampKnob.setBounds(reverbDampKnobRect);
		// reverbDampKnob.setSkewFactorFromMidPoint(processor.effectMidpointValue);
		// reverbDampKnob.setTooltip(translate("effect;"));
		// reverbDampKnob.setVisible(true);
		// reverbDampKnob.addListener(this);
	}

	void addComponents() override
	{
		addAndMakeVisible(reverbSwitchButton);
		addAndMakeVisible(reverbWetKnob);
		addAndMakeVisible(reverbSizeKnob);
		addAndMakeVisible(reverbWidthKnob);
		addAndMakeVisible(reverbDampKnob);
	}

	const int labelRectHeight = 10;

	// Binary Data
	Image controlsSectionImage;

	//Rects
	juce::Rectangle<int> reverbSwitchRect;
	juce::Rectangle<int> reverbTitleLabelRect;
	juce::Rectangle<int> reverbControlsRect;

	juce::Rectangle<int> reverbWetKnobRect;
	juce::Rectangle<int> reverbSizeKnobRect;
	juce::Rectangle<int> reverbWidthKnobRect;
	juce::Rectangle<int> reverbDampKnobRect;

	juce::Rectangle<int> reverbWetLabelRect;
	juce::Rectangle<int> reverbSizeLabelRect;
	juce::Rectangle<int> reverbWidthLabelRect;
	juce::Rectangle<int> reverbDampLabelRect;

	// Components
	ToggleButton reverbSwitchButton;
	Slider reverbSwitchSlider;
	Slider reverbWetKnob;
	Slider reverbSizeKnob;
	Slider reverbWidthKnob;
	Slider reverbDampKnob;

	// Attachments
	// std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> reverbSwitchAttachment;
	// std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> reverbWetAttachment;
	// std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> reverbSizeAttachment;
	// std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> reverbWidthAttachment;
	// std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> reverbDampAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplerControlsComponent)
};
