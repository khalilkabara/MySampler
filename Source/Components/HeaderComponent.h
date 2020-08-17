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
/*
*/
class HeaderComponent : public MyComponentBase, public Button::Listener
{
public:
	HeaderComponent(MySamplerAudioProcessor& p) : MyComponentBase(p)
	{
		// headerSectionImage = ImageCache::getFromMemory(BinaryData::reverb_section_art_png,
		//                                                BinaryData::reverb_section_art_pngSize);
	}

	~HeaderComponent()
	{
	}

	void paint(Graphics& g) override
	{
		mPaint(g, headerSectionImage);

		// g.fillAll(Colours::aqua);
		g.drawFittedText("Header", getLocalBounds(), Justification::centred, 1);

		
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

private:

	// Functions

	void defineRects() override
	{
		MyComponentBase::defineRects();

		openFileButtonRect = juce::Rectangle<int>(
			localBounds.getX(),
			localBounds.getY(),
			localBounds.getWidth() / 10,
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
		processor.loadFileOpen();
	}

	const int border = 10;

	// Binary Data
	Image headerSectionImage;

	// Components
	TextButton openFileButton;

	// Rects
	juce::Rectangle<int> openFileButtonRect;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderComponent)
};
