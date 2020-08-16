/*
  ==============================================================================

    FooterComponent.h
    Created: 16 Aug 2020 5:30:37pm
    Author:  Khalil Kabara

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "MyComponentBase.h"

//==============================================================================
/*
*/
class FooterComponent  : public MyComponentBase
{
public:
	FooterComponent(MySamplerAudioProcessor& p) : MyComponentBase(p)
	{
		// footerSectionImage = ImageCache::getFromMemory(BinaryData::reverb_section_art_png,
		//                                                BinaryData::reverb_section_art_pngSize);

	}

	~FooterComponent()
	{
	}

	void paint(Graphics& g) override
	{
		mPaint(g, footerSectionImage);

		// g.fillAll(Colours::aqua);
		g.drawFittedText("Footer", getLocalBounds(), Justification::centred, 1);
	}

	void resized() override
	{
	}

private:

	// Functions

	void defineRects() override
	{
		MyComponentBase::defineRects();


	}

	void defineComponents() override
	{
	}

	void addComponents() override
	{
	}

	const int labelRectHeight = 10;

	// Binary Data
	Image footerSectionImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FooterComponent)
};
