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
		
		envelopeAttackAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.valueTreeState,
		                                                                                processor.envelopeAttackStateName,
		                                                                                envelopeAttackKnob));
		
		envelopeDecayAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.valueTreeState,
		                                                                                processor.envelopeDecayStateName,
		                                                                                envelopeDecayKnob));
		
		envelopeSustainAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.valueTreeState,
		                                                                                processor.envelopeSustainStateName,
		                                                                                envelopeSustainKnob));
		
		envelopeReleaseAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.valueTreeState,
		                                                                                processor.envelopeReleaseStateName,
		                                                                                envelopeReleaseKnob));
		
		ampPanAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.valueTreeState,
		                                                                                processor.ampPanStateName,
		                                                                                ampPanKnob));
		
		ampVolumeAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.valueTreeState,
		                                                                                processor.ampVolumeStateName,
		                                                                                ampVolumeKnob));
		
	}

	~SamplerControlsComponent()
	{
	}

	void paint(Graphics& g) override
	{
		mPaint(g, controlsSectionImage);

		// g.drawFittedText("Reverb", titleRect, Justification::centred, 1);
		
		g.setFont(processor.myFontTiny);
		
		g.drawFittedText("Filter", filterTitleRect, Justification::centred, 1);
		g.drawFittedText("LFO", LfoTitleRect, Justification::centred, 1);
		
		g.drawFittedText("Envelope", envelopeTitleRect, Justification::centred, 1);
		g.drawFittedText("Attack", envelopeAttackKnobLabelRect, Justification::centred, 1);
		g.drawFittedText("Decay", envelopeDecayKnobLabelRect, Justification::centred, 1);
		g.drawFittedText("Sustain", envelopeSustainKnobLabelRect, Justification::centred, 1);
		g.drawFittedText("Release", envelopeReleaseKnobLabelRect, Justification::centred, 1);
		
		g.drawFittedText("Pan", ampPanKnobLabelRect, Justification::centred, 1);
		g.drawFittedText("Volume", ampVolumeKnobLabelRect, Justification::centred, 1);

		if (debugBoundRects)
		{
			// g.drawRect(getLocalBounds());
			// g.drawRect(filterRect);
			// g.drawRect(lfoRect);
			// g.drawRect(envelopeRect);
			// g.drawRect(ampRect);
		}
	}

	void resized() override
	{
	}

	void buttonClicked(Button*) override
	{
	}

	void buttonStateChanged(Button* button) override
	{
	}

private:

	// Functions

	void sliderValueChanged(Slider* slider) override
	{
	}

	void defineRects() override
	{
		MyComponentBase::defineRects();

		// Main Rects
		localBounds = juce::Rectangle<int>(
			getLocalBounds().getX() + border,
			getLocalBounds().getY() + border,
			getLocalBounds().getWidth() - 2 * border,
			getLocalBounds().getHeight() - 2 * border);

		filterRect = juce::Rectangle<int>(
			localBounds.getX(),
			localBounds.getY(),
			localBounds.getWidth() / 2,
			localBounds.getHeight() / 2);

		lfoRect = juce::Rectangle<int>(
			filterRect.getX() + filterRect.getWidth(),
			filterRect.getY(),
			filterRect.getWidth(),
			filterRect.getHeight());

		envelopeRect = juce::Rectangle<int>(
			filterRect.getX(),
			filterRect.getY() + filterRect.getHeight(),
			localBounds.getWidth() * 2 / 3,
			filterRect.getHeight());

		ampRect = juce::Rectangle<int>(
			envelopeRect.getX() + envelopeRect.getWidth(),
			envelopeRect.getY(),
			localBounds.getWidth() * 1 / 3,
			envelopeRect.getHeight());

		//****************************Filter Section******************************


		
		//****************************LFO Section******************************


		
		//****************************Envelope Section******************************
		
		// envelopeTitleRect = juce::Rectangle<int>(
		// 	envelopeRect.getX(),
		// 	envelopeRect.getY(),
		// 	envelopeRect.getWidth(),
		// 	envelopeRect.getHeight() / 5);

		// Attack
		envelopeAttackKnobRect = juce::Rectangle<int>(
			envelopeRect.getX(),
			envelopeRect.getY(),
			envelopeRect.getWidth() / 4,
			envelopeRect.getHeight() - labelHeight);

		envelopeAttackKnobLabelRect = juce::Rectangle<int>(
			envelopeAttackKnobRect.getX(),
			envelopeAttackKnobRect.getY() + envelopeAttackKnobRect.getHeight(),
			envelopeAttackKnobRect.getWidth(),
			labelHeight);

		// Decay
		envelopeDecayKnobRect = juce::Rectangle<int>(
			envelopeAttackKnobRect.getX() + envelopeAttackKnobRect.getWidth(),
			envelopeAttackKnobRect.getY(),
			envelopeAttackKnobRect.getWidth(),
			envelopeAttackKnobRect.getHeight());

		envelopeDecayKnobLabelRect = juce::Rectangle<int>(
			envelopeAttackKnobLabelRect.getX() + envelopeAttackKnobLabelRect.getWidth(),
			envelopeAttackKnobLabelRect.getY(),
			envelopeAttackKnobLabelRect.getWidth(),
			labelHeight);

		// Sustain
		envelopeSustainKnobRect = juce::Rectangle<int>(
			envelopeDecayKnobRect.getX() + envelopeDecayKnobRect.getWidth(),
			envelopeDecayKnobRect.getY(),
			envelopeDecayKnobRect.getWidth(),
			envelopeDecayKnobRect.getHeight());

		envelopeSustainKnobLabelRect = juce::Rectangle<int>(
			envelopeDecayKnobLabelRect.getX() + envelopeDecayKnobLabelRect.getWidth(),
			envelopeDecayKnobLabelRect.getY(),
			envelopeDecayKnobLabelRect.getWidth(),
			labelHeight);

		// Release
		envelopeReleaseKnobRect = juce::Rectangle<int>(
			envelopeSustainKnobRect.getX() + envelopeSustainKnobRect.getWidth(),
			envelopeSustainKnobRect.getY(),
			envelopeSustainKnobRect.getWidth(),
			envelopeSustainKnobRect.getHeight());

		envelopeReleaseKnobLabelRect = juce::Rectangle<int>(
			envelopeSustainKnobLabelRect.getX() + envelopeSustainKnobLabelRect.getWidth(),
			envelopeSustainKnobLabelRect.getY(),
			envelopeSustainKnobLabelRect.getWidth(),
			labelHeight);


		//****************************Amp Section******************************

		// Pan
		ampPanKnobRect = juce::Rectangle<int>(
			ampRect.getX(),
			ampRect.getY(),
			ampRect.getWidth() / 2,
			ampRect.getHeight() - labelHeight);

		ampPanKnobLabelRect = juce::Rectangle<int>(
			ampPanKnobRect.getX(),
			ampPanKnobRect.getY() + ampPanKnobRect.getHeight(),
			ampPanKnobRect.getWidth(),
			labelHeight);

		// Volume
		ampVolumeKnobRect = juce::Rectangle<int>(
			ampPanKnobRect.getX() + ampPanKnobRect.getWidth(),
			ampPanKnobRect.getY(),
			ampPanKnobRect.getWidth(),
			ampPanKnobRect.getHeight());

		ampVolumeKnobLabelRect = juce::Rectangle<int>(
			ampPanKnobLabelRect.getX() + ampPanKnobLabelRect.getWidth(),
			ampPanKnobLabelRect.getY(),
			ampPanKnobLabelRect.getWidth(),
			labelHeight);

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

		//****************************Filter Section******************************



		//****************************LFO Section******************************



		//****************************Envelope Section******************************

		envelopeAttackKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		envelopeAttackKnob.setRange(processor.zeroToTenMinValue, processor.zeroToTenMaxValue, processor.zeroToTenStepValue);
		envelopeAttackKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
		envelopeAttackKnob.setBounds(envelopeAttackKnobRect);
		envelopeAttackKnob.setSkewFactorFromMidPoint(processor.zeroToTenMidpointValue);
		envelopeAttackKnob.setTooltip(translate(processor.effectTooltip));
		envelopeAttackKnob.setVisible(true);
		envelopeAttackKnob.addListener(this);

		envelopeDecayKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		envelopeDecayKnob.setRange(processor.zeroToTenMinValue, processor.zeroToTenMaxValue, processor.zeroToTenStepValue);
		envelopeDecayKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
		envelopeDecayKnob.setBounds(envelopeDecayKnobRect);
		envelopeDecayKnob.setSkewFactorFromMidPoint(processor.zeroToTenMidpointValue);
		envelopeDecayKnob.setTooltip(translate(processor.effectTooltip));
		envelopeDecayKnob.setVisible(true);
		envelopeDecayKnob.addListener(this);

		envelopeSustainKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		envelopeSustainKnob.setRange(processor.zeroToOneMinValue, processor.zeroToOneMaxValue, processor.zeroToOneStepValue);
		envelopeSustainKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
		envelopeSustainKnob.setBounds(envelopeSustainKnobRect);
		envelopeSustainKnob.setSkewFactorFromMidPoint(processor.zeroToOneMidpointValue);
		envelopeSustainKnob.setTooltip(translate(processor.effectTooltip));
		envelopeSustainKnob.setVisible(true);
		envelopeSustainKnob.addListener(this);

		envelopeReleaseKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		envelopeReleaseKnob.setRange(processor.zeroToTenMinValue, processor.zeroToTenMaxValue, processor.zeroToTenStepValue);
		envelopeReleaseKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
		envelopeReleaseKnob.setBounds(envelopeReleaseKnobRect);
		envelopeReleaseKnob.setSkewFactorFromMidPoint(processor.zeroToTenMidpointValue);
		envelopeReleaseKnob.setTooltip(translate(processor.effectTooltip));
		envelopeReleaseKnob.setVisible(true);
		envelopeReleaseKnob.addListener(this);

		//****************************Amp Section******************************

		ampPanKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		ampPanKnob.setRange(processor.bipolarMinValue, processor.bipolarMaxValue, processor.bipolarStepValue);
		ampPanKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
		ampPanKnob.setBounds(ampPanKnobRect);
		ampPanKnob.setSkewFactorFromMidPoint(processor.bipolarMidpointValue);
		ampPanKnob.setTooltip(translate(processor.bipolarEffectTooltip));
		ampPanKnob.setVisible(true);
		ampPanKnob.addListener(this);

		ampVolumeKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		ampVolumeKnob.setRange(processor.zeroToOneMinValue, processor.zeroToOneMaxValue, processor.zeroToOneStepValue);
		ampVolumeKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
		ampVolumeKnob.setBounds(ampVolumeKnobRect);
		ampVolumeKnob.setSkewFactorFromMidPoint(processor.zeroToOneMidpointValue);
		ampVolumeKnob.setTooltip(translate(processor.effectTooltip));
		ampVolumeKnob.setVisible(true);
		ampVolumeKnob.addListener(this);

	}

	void addComponents() override
	{
		addAndMakeVisible(envelopeAttackKnob);
		addAndMakeVisible(envelopeDecayKnob);
		addAndMakeVisible(envelopeSustainKnob);
		addAndMakeVisible(envelopeReleaseKnob);

		addAndMakeVisible(ampPanKnob);
		addAndMakeVisible(ampVolumeKnob);
	}

	const int border = 5;
	const int labelHeight = 7;

	// Binary Data
	Image controlsSectionImage;

	//*************************************************************************************

	//Rects - Main
	juce::Rectangle<int> filterRect;
	juce::Rectangle<int> lfoRect;
	juce::Rectangle<int> envelopeRect;
	juce::Rectangle<int> ampRect;
	// Filter Section
	juce::Rectangle<int> filterTitleRect;
	// LFO Section
	juce::Rectangle<int> LfoTitleRect;
	// Envelope Section
	juce::Rectangle<int> envelopeTitleRect;
	juce::Rectangle<int> envelopeAttackKnobRect;
	juce::Rectangle<int> envelopeAttackKnobLabelRect;
	juce::Rectangle<int> envelopeDecayKnobRect;
	juce::Rectangle<int> envelopeDecayKnobLabelRect;
	juce::Rectangle<int> envelopeSustainKnobRect;
	juce::Rectangle<int> envelopeSustainKnobLabelRect;
	juce::Rectangle<int> envelopeReleaseKnobRect;
	juce::Rectangle<int> envelopeReleaseKnobLabelRect;
	// Amp Section
	juce::Rectangle<int> ampPanKnobRect;
	juce::Rectangle<int> ampPanKnobLabelRect;
	juce::Rectangle<int> ampVolumeKnobRect;
	juce::Rectangle<int> ampVolumeKnobLabelRect;

	//*************************************************************************************

	// Components - Filter

	// LFO

	// Envelope
	Slider envelopeAttackKnob;
	Slider envelopeDecayKnob;
	Slider envelopeSustainKnob;
	Slider envelopeReleaseKnob;
	// Amp
	Slider ampPanKnob;
	Slider ampVolumeKnob;

	//**************************************Attachments***********************************************

	// Envelope
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> envelopeAttackAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> envelopeDecayAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> envelopeSustainAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> envelopeReleaseAttachment;
	// Amp
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> ampPanAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> ampVolumeAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplerControlsComponent)
};
