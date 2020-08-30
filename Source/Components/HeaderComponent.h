/*
  ==============================================================================

    headerComponent.h
    Created: 16 Aug 2020 5:29:20pm
    Author:  Khalil Kabara

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


#include "ConfirmationDialog.h"
#include "MyComponentBase.h"

//==============================================================================

class HeaderComponent : public MyComponentBase, public Button::Listener, Timer
{
public:
	HeaderComponent(MySamplerAudioProcessor& p) : MyComponentBase(p)
	{
		// headerSectionImage = ImageCache::getFromMemory(BinaryData::reverb_section_art_png,
		//                                                BinaryData::reverb_section_art_pngSize);

		startTimerHz(fps);
	}

	~HeaderComponent()
	{
	}

	void paint(Graphics& g) override
	{
		mPaint(g, headerSectionImage);

		// g.fillAll(Colours::aqua);
		g.drawFittedText(displayText, getLocalBounds(), Justification::centred, 1);
	}

	void resized() override
	{
		openFileButton.setBounds(openFileButtonRect);
	}

	void buttonClicked(Button* button) override
	{
		if (button == &openFileButton)
		{
			loadFile();
		}
		if (button == &clearFileButton)
		{
			clearFile();
		}
		if (button == &resetSamplerButton)
		{
			resetSampler();
		}
	}

	void buttonStateChanged(Button* button) override
	{
	}

	static void setDisplayText(const String text)
	{
		displayText = text;
	}

	static String displayText;

private:

	// Functions

	void timerCallback() override
	{
		repaint();
	}

	void defineRects() override
	{
		MyComponentBase::defineRects();
		const auto headerItemWidth = localBounds.getWidth() / 10;
		const auto displayWidth = headerItemWidth * 4;

		openFileButtonRect = juce::Rectangle<int>(
			localBounds.getX() + border,
			localBounds.getY(),
			headerItemWidth,
			localBounds.getHeight());

		clearFileButtonRect = juce::Rectangle<int>(
			openFileButtonRect.getX() + openFileButtonRect.getWidth() + border,
			localBounds.getY(),
			headerItemWidth,
			localBounds.getHeight());

		resetSamplerButtonRect = juce::Rectangle<int>(
			clearFileButtonRect.getX() + clearFileButtonRect.getWidth() + border,
			localBounds.getY(),
			headerItemWidth,
			localBounds.getHeight());

		displayRect = juce::Rectangle<int>(
			localBounds.getX() + localBounds.getWidth() - displayWidth / 2,
			localBounds.getY(),
			displayWidth,
			localBounds.getHeight());

	}

	void defineComponents() override
	{
		openFileButton.setButtonText("Open File");
		openFileButton.setVisible(true);
		// openFileButton.changeWidthToFitText();
		openFileButton.addListener(this);
		openFileButton.setBounds(openFileButtonRect);

		clearFileButton.setButtonText("Clear File");
		clearFileButton.setVisible(true);
		// clearFileButton.changeWidthToFitText();
		clearFileButton.addListener(this);
		clearFileButton.setBounds(clearFileButtonRect);

		resetSamplerButton.setButtonText("Reset Sampler");
		resetSamplerButton.setVisible(true);
		// resetSamplerButton.changeWidthToFitText();
		resetSamplerButton.addListener(this);
		resetSamplerButton.setBounds(resetSamplerButtonRect);
	}

	void addComponents() override
	{
		addAndMakeVisible(openFileButton);
		addAndMakeVisible(clearFileButton);
		addAndMakeVisible(resetSamplerButton);
		// addAndMakeVisible(confirmationDialog);

		//*********************************************
		const auto tempRect = juce::Rectangle<int>(
			getLocalBounds().getWidth() - 50, getLocalBounds().getY(), 40, getLocalBounds().getHeight());

		tempSlider.setBounds(tempRect);
		tempSlider.setSliderStyle(Slider::LinearBar);
		tempSlider.setTextBoxStyle(Slider::NoTextBox, false, 30, 10);
		tempSlider.setRange(0, 10, 1);
		tempSlider.setVisible(true);
		tempSlider.setValue(6);
		addAndMakeVisible(tempSlider);
	}

	void loadFile() const
	{
		processor.loadFile();
	}

	void clearFile() const { processor.clearFile(); }

	void resetSampler() const
	{
		processor.resetSampler();
	}

	// void resetSampler() const
	// {
	// 	confirmationDialog.reset([this]() { doResetSampler(); }, [this]() { closeResetSampler(); });
	//
	// 	confirmationDialog.setBounds(getLocalBounds());
	// }
	//
	// ConfirmationDialog& confirmationDialog;
	//
	// void doResetSampler() const { processor.resetSampler(); }
	//
	// void closeResetSampler() const
	// {
	// 	confirmationDialog.setBounds(0, 0, 0, 0);
	// }

	Slider tempSlider;
	
	const int border = 7.5;
	const int fps = 10;

	// Binary Data
	Image headerSectionImage;

	// Components
	TextButton openFileButton;
	TextButton clearFileButton;
	TextButton resetSamplerButton;

	// Rects
	juce::Rectangle<int> openFileButtonRect;
	juce::Rectangle<int> clearFileButtonRect;
	juce::Rectangle<int> resetSamplerButtonRect;
	juce::Rectangle<int> displayRect;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderComponent)
};
