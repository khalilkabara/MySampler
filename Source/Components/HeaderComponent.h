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
class HeaderComponent  : public MyComponentBase
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
	Image headerSectionImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HeaderComponent)
};