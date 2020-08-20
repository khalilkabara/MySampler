/*
  ==============================================================================

    headerComponent.h
    Created: 16 Aug 2020 5:29:20pm
    Author:  Khalil Kabara

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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
		// g.drawFittedText(displayText, getLocalBounds(), Justification::centred, 1);

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
	}

	void buttonStateChanged(Button* button) override
	{
		
	}

	// static void setDisplayText(const String text)
	// {
	// 	displayText = text;
	// }

	// static String displayText;

private:

	// Functions

	void timerCallback() override
	{
		repaint();
	}

	void defineRects() override
	{
		MyComponentBase::defineRects();
		displayWidth = localBounds.getWidth() / 3;

		openFileButtonRect = juce::Rectangle<int>(
			localBounds.getX(),
			localBounds.getY(),
			localBounds.getWidth() / 10,
			localBounds.getHeight());

		displayRect = juce::Rectangle<int>(
			localBounds.getX() + localBounds.getWidth() - displayWidth/2,
			localBounds.getY(),
			displayWidth,
			localBounds.getHeight());
	}

	void defineComponents() override
	{
		openFileButton.setButtonText("Open File");
		openFileButton.setVisible(true);
		openFileButton.changeWidthToFitText();
		openFileButton.addListener(this);
		openFileButton.setBounds(openFileButtonRect);
	}

	void addComponents() override
	{
		addAndMakeVisible(openFileButton);
	}

	void loadFile() const
	{
		processor.loadFile();
	}

	const int border = 10;
	const int fps = 10;
	int displayWidth;

	// Binary Data
	Image headerSectionImage;

	// Components
	TextButton openFileButton;

	// Rects
	juce::Rectangle<int> openFileButtonRect;
	juce::Rectangle<int> displayRect;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderComponent)
};
