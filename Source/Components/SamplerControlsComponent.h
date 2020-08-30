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
#include "VisualEnvelopeComponent.h"

//==============================================================================

class SamplerControlsComponent : public MyComponentBase,
                                 private Slider::Listener,
                                 private ComboBox::Listener,
                                 private TextEditor::Listener,
                                 public Button::Listener

{
public:

	SamplerControlsComponent(MySamplerAudioProcessor& p) : MyComponentBase(p),
	                                                       visualEnvelopeComponent(p)
	{
		// controlsSectionImage = ImageCache::getFromMemory(BinaryData::reverb_section_art_png,
		//                                                BinaryData::reverb_section_art_pngSize);

		// Filter
		filterTypeAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(
			processor.valueTreeState, processor.filterTypeStateName, filterTypeSelector));

		filterCutoffAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(
			processor.valueTreeState, processor.filterCutoffStateName, filterCutoffKnob));

		filterResonanceAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(
			processor.valueTreeState, processor.filterResonanceStateName, filterResonanceKnob));

		// Settings
		numVoicesInputBoxAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(
			processor.valueTreeState, processor.numVoicesStateName, numVoicesInputBox));

		noteStealingAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(
			processor.valueTreeState, processor.noteStealingStateName, noteStealSlider));

		// Envelope
		envelopeAttackAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(
			processor.valueTreeState, processor.envelopeAttackStateName, envelopeAttackKnob));

		envelopeDecayAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(
			processor.valueTreeState, processor.envelopeDecayStateName, envelopeDecayKnob));

		envelopeSustainAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(
			processor.valueTreeState, processor.envelopeSustainStateName, envelopeSustainKnob));

		envelopeReleaseAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(
			processor.valueTreeState, processor.envelopeReleaseStateName, envelopeReleaseKnob));

		// Amp
		ampPanAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(
			processor.valueTreeState, processor.ampPanStateName, ampPanKnob));

		ampVolumeAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(
			processor.valueTreeState, processor.ampVolumeStateName, ampVolumeKnob));
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
		g.drawFittedText("Type", filterTypeLabelRect, Justification::centred, 1);
		g.drawFittedText("Cutoff", filterCutoffKnobLabelRect, Justification::centred, 1);
		g.drawFittedText("Resonance", filterResonanceKnobLabelRect, Justification::centred, 1);

		g.drawFittedText("Poly", numVoicesLabelRect, Justification::centred, 1);
		g.drawFittedText("Note Steal", noteStealingLabelRect, Justification::centred, 1);

		g.drawFittedText("Envelope", envelopeTitleRect, Justification::centred, 1);
		g.drawFittedText("Attack", envelopeAttackKnobLabelRect, Justification::centred, 1);
		g.drawFittedText("Decay", envelopeDecayKnobLabelRect, Justification::centred, 1);
		g.drawFittedText("Sustain", envelopeSustainKnobLabelRect, Justification::centred, 1);
		g.drawFittedText("Release", envelopeReleaseKnobLabelRect, Justification::centred, 1);

		g.drawFittedText("Pan", ampPanKnobLabelRect, Justification::centred, 1);
		g.drawFittedText("Volume", ampVolumeKnobLabelRect, Justification::centred, 1);

		if (debugBoundRects)
		{
			// g.drawRect(visualEnvelopeRect);
			g.drawRect(filterRect);
			g.drawRect(filterTitleRect);
			g.drawRect(filterTypeLabelRect);
			g.drawRect(filterTypeRect);
			g.drawRect(filterCutoffKnobRect);
			g.drawRect(filterCutoffKnobLabelRect);
			g.drawRect(filterResonanceKnobLabelRect);

			g.drawRect(settingsRect);
			g.drawRect(envelopeRect);
			g.drawRect(ampRect);
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
		if (button == &noteStealSwitchButton)
		{
			noteStealSwitchButton.getToggleState()
				? noteStealSlider.setValue(processor.toggleOnValue)
				: noteStealSlider.setValue(processor.toggleOffValue);

			processor.setNoteStealing(noteStealSwitchButton.getToggleState());
		}
	}

private:

	VisualEnvelopeComponent visualEnvelopeComponent;

	// Functions

	void sliderValueChanged(Slider* slider) override
	{
		if (slider == &numVoicesInputBox)
		{
			processor.resetNumVoices(static_cast<int>(slider->getValue()));
		}
		if (slider == &noteStealSlider)
		{
			noteStealSwitchButton.setToggleState(noteStealSlider.getValue() == processor.toggleOnValue,
			                                     NotificationType::dontSendNotification);
		}
	}

	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override
	{
	}

	void textEditorTextChanged(TextEditor& textEditor) override
	{
	}

	void defineRects() override
	{
		MyComponentBase::defineRects();
		localBounds = getLocalBounds();

		// Main Rects

		visualEnvelopeRect = juce::Rectangle<int>(
			localBounds.getX() + border,
			localBounds.getY() + border,
			localBounds.getWidth() * 4 / 10 - (2 * border),
			localBounds.getHeight() / 2 - border);

		envelopeRect = juce::Rectangle<int>(
			visualEnvelopeRect.getX(),
			visualEnvelopeRect.getY() + visualEnvelopeRect.getHeight() + border,
			visualEnvelopeRect.getWidth(),
			visualEnvelopeRect.getHeight() - border);

		settingsRect = juce::Rectangle<int>(
			visualEnvelopeRect.getX() + visualEnvelopeRect.getWidth() + border,
			localBounds.getY() + border,
			localBounds.getWidth() * 5 / 10 - (2 * border),
			localBounds.getHeight() * 2 / 5 - border);

		filterRect = juce::Rectangle<int>(
			settingsRect.getX(),
			settingsRect.getY() + settingsRect.getHeight() + border,
			settingsRect.getWidth(),
			(localBounds.getHeight() * 3 / 5) - 2 * border);

		ampRect = juce::Rectangle<int>(
			settingsRect.getX() + settingsRect.getWidth() + border,
			settingsRect.getY(),
			localBounds.getWidth() * 1 / 10,
			localBounds.getHeight() - 2 * border);

		//****************************Filter Section******************************

		filterTitleRect = juce::Rectangle<int>(
			filterRect.getX(),
			filterRect.getY(),
			filterRect.getWidth() / 4,
			filterRect.getHeight());

		filterTypeLabelRect = juce::Rectangle<int>(
			filterTitleRect.getX() + filterTitleRect.getWidth(),
			filterTitleRect.getY(),
			(filterRect.getWidth() - filterTitleRect.getWidth()) / 3,
			labelHeight * 2);

		filterTypeRect = juce::Rectangle<int>(
			filterTypeLabelRect.getX() + filterTypeLabelRect.getWidth(),
			filterTypeLabelRect.getY(),
			(filterRect.getWidth() - filterTitleRect.getWidth()) * 2 / 3,
			filterTypeLabelRect.getHeight());

		filterCutoffKnobRect = juce::Rectangle<int>(
			filterTypeLabelRect.getX(),
			filterTypeLabelRect.getY() + filterTypeLabelRect.getHeight(),
			(filterRect.getWidth() - filterTitleRect.getWidth()) / 2,
			filterRect.getHeight() - filterTypeLabelRect.getHeight() - labelHeight);

		filterCutoffKnobLabelRect = juce::Rectangle<int>(
			filterCutoffKnobRect.getX(),
			filterCutoffKnobRect.getY() + filterCutoffKnobRect.getHeight(),
			filterCutoffKnobRect.getWidth(),
			labelHeight);

		filterResonanceKnobRect = juce::Rectangle<int>(
			filterCutoffKnobRect.getX() + filterCutoffKnobRect.getWidth(),
			filterCutoffKnobRect.getY(),
			filterCutoffKnobRect.getWidth(),
			filterCutoffKnobRect.getHeight());

		filterResonanceKnobLabelRect = juce::Rectangle<int>(
			filterResonanceKnobRect.getX(),
			filterResonanceKnobRect.getY() + filterResonanceKnobRect.getHeight(),
			filterResonanceKnobRect.getWidth(),
			labelHeight);

		//****************************Settings Section******************************

		numVoicesRect = juce::Rectangle<int>(
			settingsRect.getX(),
			settingsRect.getY(),
			settingsRect.getWidth() / numSettingsItems - margin,
			settingsRect.getHeight() - labelHeight);

		numVoicesLabelRect = juce::Rectangle<int>(
			numVoicesRect.getX(),
			numVoicesRect.getY() + numVoicesRect.getHeight(),
			numVoicesRect.getWidth(),
			labelHeight);

		tempRect = juce::Rectangle<int>(
			settingsRect.getX() + settingsRect.getWidth() / numSettingsItems + 2*margin,
			settingsRect.getY(),
			settingsRect.getWidth() / numSettingsItems - margin,
			settingsRect.getHeight() - labelHeight);

		noteStealingRect = tempRect;
		
		//
		// tempRect = juce::Rectangle<int>(
		// 	settingsRect.getX() + settingsRect.getWidth() / numSettingsItems + 2*margin,
		// 	settingsRect.getY(),
		// 	settingsRect.getWidth() / numSettingsItems - margin,
		// 	settingsRect.getHeight() - labelHeight);

		// noteStealingRect = juce::Rectangle<int>(
		// 	tempRect.getX() + tempRect.getWidth() / 2 - toggleSize / 2,
		// 	tempRect.getY() + tempRect.getHeight() / 2 - toggleSize / 2,
		// 	toggleSize,
		// 	toggleSize);

		noteStealingLabelRect = juce::Rectangle<int>(
			tempRect.getX(),
			tempRect.getY() + tempRect.getHeight(),
			tempRect.getWidth(),
			labelHeight);

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
			ampRect.getWidth(),
			ampRect.getHeight() / 2 - labelHeight);

		ampPanKnobLabelRect = juce::Rectangle<int>(
			ampPanKnobRect.getX(),
			ampPanKnobRect.getY() + ampPanKnobRect.getHeight(),
			ampPanKnobRect.getWidth(),
			labelHeight);

		// Volume
		ampVolumeKnobRect = juce::Rectangle<int>(
			ampPanKnobRect.getX(),
			ampPanKnobLabelRect.getY() + ampPanKnobLabelRect.getHeight(),
			ampPanKnobRect.getWidth(),
			ampPanKnobRect.getHeight());

		ampVolumeKnobLabelRect = juce::Rectangle<int>(
			ampVolumeKnobRect.getX(),
			ampVolumeKnobRect.getY() + ampVolumeKnobRect.getHeight(),
			ampVolumeKnobRect.getWidth(),
			labelHeight);
	}

	void defineComponents() override
	{
		//****************************Filter Section******************************

		filterTypeSelector.addItemList(processor.FILTER_TYPES, 1);
		const auto selectedTypeIndex = static_cast<int>(*processor.valueTreeState.getRawParameterValue(
			processor.filterTypeStateName));
		filterTypeSelector.setText(processor.FILTER_TYPES[selectedTypeIndex]);
		filterTypeSelector.setVisible(true);
		filterTypeSelector.setEnabled(true);
		filterTypeSelector.setBounds(filterTypeRect);
		filterTypeSelector.addListener(this);

		filterCutoffKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		filterCutoffKnob.setRange(processor.filterCutoffMinValue, processor.filterCutoffMaxValue,
		                          processor.filterCutoffStep);
		filterCutoffKnob.setTextBoxStyle(Slider::NoTextBox, false, 30, 10);
		filterCutoffKnob.setBounds(filterCutoffKnobRect);
		filterCutoffKnob.setSkewFactorFromMidPoint(processor.filterCutoffMidpoint);
		filterCutoffKnob.setTooltip(translate(processor.effectTooltip));
		filterCutoffKnob.setVisible(true);
		filterCutoffKnob.addListener(this);

		filterResonanceKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		filterResonanceKnob.setRange(processor.filterResonanceMinValue, processor.filterResonanceMaxValue,
		                             processor.filterResonanceStep);
		filterResonanceKnob.setTextBoxStyle(Slider::NoTextBox, false, 30, 10);
		filterResonanceKnob.setBounds(filterResonanceKnobRect);
		filterResonanceKnob.setSkewFactorFromMidPoint(processor.filterResonanceMidpoint);
		filterResonanceKnob.setTooltip(translate(processor.effectTooltip));
		filterResonanceKnob.setVisible(true);
		filterResonanceKnob.addListener(this);


		//****************************Settings Section******************************

		// numVoicesInputBox.setSliderStyle(Slider::IncDecButtons);
		// numVoicesInputBox.setTextBoxStyle(Slider::TextBoxAbove, true, numVoicesRect.getWidth() / 2, labelHeight * 1.5);
		numVoicesInputBox.setIncDecButtonsMode(Slider::incDecButtonsDraggable_AutoDirection);
		numVoicesInputBox.setRange(processor.minVoices, processor.maxVoices, 1);
		numVoicesInputBox.setTextBoxIsEditable(false);
		// numVoicesInputBox.setTextBoxStyle(Slider::TextBoxLeft, true, numVoicesRect.getWidth() / 2, labelHeight * 1.5);
		numVoicesInputBox.setNumDecimalPlacesToDisplay(0);
		numVoicesInputBox.addListener(this);
		numVoicesInputBox.setBounds(numVoicesRect);

		noteStealSlider.setRange(processor.toggleOffValue, processor.toggleOnValue, processor.toggleStepValue);
		noteStealSlider.setVisible(false);
		noteStealSlider.addListener(this);

		noteStealSwitchButton.setBounds(noteStealingRect);
		const auto chorusIsEnabled = static_cast<int>(*processor.valueTreeState.getRawParameterValue(
				processor.noteStealingStateName)) == processor.
			toggleOnValue;
		noteStealSwitchButton.setToggleState(chorusIsEnabled, NotificationType::dontSendNotification);
		noteStealSwitchButton.setVisible(true);
		noteStealSwitchButton.addListener(this);


		//****************************Visual Envelope Section******************************

		visualEnvelopeComponent.setSize(visualEnvelopeRect.getWidth(), visualEnvelopeRect.getHeight());
		visualEnvelopeComponent.build();
		visualEnvelopeComponent.setBounds(visualEnvelopeRect);
		addAndMakeVisible(&visualEnvelopeComponent);

		//****************************Envelope Section******************************

		envelopeAttackKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		envelopeAttackKnob.setRange(processor.zeroToTenMinValue, processor.zeroToTenMaxValue,
		                            processor.zeroToTenStepValue);
		envelopeAttackKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
		envelopeAttackKnob.setBounds(envelopeAttackKnobRect);
		// envelopeAttackKnob.setSkewFactorFromMidPoint(processor.zeroToTenMidpointValue);
		envelopeAttackKnob.setTooltip(translate(processor.effectTooltip));
		envelopeAttackKnob.setVisible(true);
		envelopeAttackKnob.addListener(this);

		envelopeDecayKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		envelopeDecayKnob.setRange(processor.zeroToTenMinValue, processor.zeroToTenMaxValue,
		                           processor.zeroToTenStepValue);
		envelopeDecayKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
		envelopeDecayKnob.setBounds(envelopeDecayKnobRect);
		// envelopeDecayKnob.setSkewFactorFromMidPoint(processor.zeroToTenMidpointValue);
		envelopeDecayKnob.setTooltip(translate(processor.effectTooltip));
		envelopeDecayKnob.setVisible(true);
		envelopeDecayKnob.addListener(this);

		envelopeSustainKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		envelopeSustainKnob.setRange(processor.zeroToOneMinValue, processor.zeroToOneMaxValue,
		                             processor.zeroToOneStepValue);
		envelopeSustainKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
		envelopeSustainKnob.setBounds(envelopeSustainKnobRect);
		// envelopeSustainKnob.setSkewFactorFromMidPoint(processor.zeroToOneMidpointValue);
		envelopeSustainKnob.setTooltip(translate(processor.effectTooltip));
		envelopeSustainKnob.setVisible(true);
		envelopeSustainKnob.addListener(this);

		envelopeReleaseKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		envelopeReleaseKnob.setRange(processor.zeroToTenMinValue, processor.zeroToTenMaxValue,
		                             processor.zeroToTenStepValue);
		envelopeReleaseKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
		envelopeReleaseKnob.setBounds(envelopeReleaseKnobRect);
		// envelopeReleaseKnob.setSkewFactorFromMidPoint(processor.zeroToTenMidpointValue);
		envelopeReleaseKnob.setTooltip(translate(processor.effectTooltip));
		envelopeReleaseKnob.setVisible(true);
		envelopeReleaseKnob.addListener(this);

		//****************************Amp Section******************************

		ampPanKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		ampPanKnob.setRange(processor.bipolarMinValue, processor.bipolarMaxValue, processor.bipolarStepValue);
		ampPanKnob.setTextBoxStyle(Slider::NoTextBox, false, 30, 10);
		ampPanKnob.setBounds(ampPanKnobRect);
		ampPanKnob.setSkewFactorFromMidPoint(processor.bipolarMidpointValue);
		ampPanKnob.setTooltip(translate(processor.bipolarEffectTooltip));
		ampPanKnob.setVisible(true);
		ampPanKnob.addListener(this);

		ampVolumeKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		ampVolumeKnob.setRange(processor.zeroToOneMinValue, processor.zeroToOneMaxValue, processor.zeroToOneStepValue);
		ampVolumeKnob.setTextBoxStyle(Slider::NoTextBox, false, 30, 10);
		ampVolumeKnob.setBounds(ampVolumeKnobRect);
		ampVolumeKnob.setSkewFactorFromMidPoint(processor.zeroToOneMidpointValue);
		ampVolumeKnob.setTooltip(translate(processor.effectTooltip));
		ampVolumeKnob.setVisible(true);
		ampVolumeKnob.addListener(this);
	}

	void addComponents() override
	{
		addAndMakeVisible(filterTypeSelector);
		addAndMakeVisible(filterCutoffKnob);
		addAndMakeVisible(filterResonanceKnob);

		addAndMakeVisible(numVoicesInputBox);
		addAndMakeVisible(noteStealSwitchButton);

		addAndMakeVisible(envelopeAttackKnob);
		addAndMakeVisible(envelopeDecayKnob);
		addAndMakeVisible(envelopeSustainKnob);
		addAndMakeVisible(envelopeReleaseKnob);

		addAndMakeVisible(ampPanKnob);
		addAndMakeVisible(ampVolumeKnob);
	}

	const int numSettingsItems = 5;
	const int margin = 2.5;
	const int border = 5;
	const int toggleSize = 15;
	const int labelHeight{10};

	// Binary Data
	Image controlsSectionImage;

	//*************************************************************************************

	//Rects - Main
	juce::Rectangle<int> tempRect;
	juce::Rectangle<int> filterRect;
	juce::Rectangle<int> settingsRect;
	juce::Rectangle<int> envelopeRect;
	juce::Rectangle<int> visualEnvelopeRect;
	juce::Rectangle<int> ampRect;
	// Filter Section
	juce::Rectangle<int> filterTitleRect;
	juce::Rectangle<int> filterTypeLabelRect;
	juce::Rectangle<int> filterTypeRect;
	juce::Rectangle<int> filterCutoffKnobRect;
	juce::Rectangle<int> filterCutoffKnobLabelRect;
	juce::Rectangle<int> filterResonanceKnobRect;
	juce::Rectangle<int> filterResonanceKnobLabelRect;
	// Settings Section
	juce::Rectangle<int> numVoicesRect;
	juce::Rectangle<int> numVoicesLabelRect;
	juce::Rectangle<int> noteStealingRect;
	juce::Rectangle<int> noteStealingLabelRect;

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
	ComboBox filterTypeSelector;
	Slider filterCutoffKnob;
	Slider filterResonanceKnob;
	// Settings
	Slider numVoicesInputBox{ Slider::IncDecButtons , Slider::TextBoxBelow};
	ToggleButton noteStealSwitchButton;
	Slider noteStealSlider;
	// Envelope
	Slider envelopeAttackKnob;
	Slider envelopeDecayKnob;
	Slider envelopeSustainKnob;
	Slider envelopeReleaseKnob;
	// Amp
	Slider ampPanKnob;
	Slider ampVolumeKnob;

	//**************************************Attachments***********************************************

	//Filter
	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;

	// Settings
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> numVoicesInputBoxAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> noteStealingAttachment;

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
