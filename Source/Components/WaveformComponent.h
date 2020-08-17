/*
  ==============================================================================

    WaveformComponent.h
    Created: 17 Aug 2020 2:39:30am
    Author:  Khalil Kabara

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "MyComponentBase.h"

//==============================================================================

class WaveformComponent : public MyComponentBase
{
public:
	WaveformComponent(MySamplerAudioProcessor& p) : MyComponentBase(p)
	{
		// waveformSectionImage = ImageCache::getFromMemory(BinaryData::reverb_section_art_png,
		//                                                BinaryData::reverb_section_art_pngSize);

	}

	~WaveformComponent()
	{
	}

	void paint(Graphics& g) override
	{
		mPaint(g, waveformSectionImage);

		// g.fillAll(Colours::aqua);
		g.drawFittedText("Waveform", getLocalBounds(), Justification::centred, 1);

		if (debugBoundRects)
			g.drawRect(getLocalBounds());
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
	Image waveformSectionImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformComponent)
};
