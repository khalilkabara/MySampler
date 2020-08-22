/*
  ==============================================================================

    SynthComponentBase.h
    Created: 16 Jul 2020 4:18:11pm
    Author:  Khalil Kabara

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "JuceHeader.h"
#include "../PluginProcessor.h"
#include "../LookAndFeel/CustomGraphics.h"

//==============================================================================

class MyComponentBase : public juce::Component
{
public:
	explicit MyComponentBase(MySamplerAudioProcessor& p) : processor(p)
	{
		setLookAndFeel(&mLookAndFeel);
	}

	virtual ~MyComponentBase()
	{
	}

	void mPaint(juce::Graphics& g, juce::Image sectionImage) const
	{
		g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
		g.setColour(juce::Colours::whitesmoke);
		g.setFont(processor.myFont);

		auto* image = &sectionImage;
		// g.drawImageWithin(*image,
		//                   getLocalBounds().getX(),
		//                   getLocalBounds().getY(),
		//                   getLocalBounds().getWidth(),
		//                   getLocalBounds().getHeight(), RectanglePlacement::fillDestination);
	}

	void resized() override
	{
	}

	void build()
	{
		defineRects();
		defineComponents();
		addComponents();
	}

	virtual void defineRects()
	{
		localBounds = juce::Rectangle<int>(getLocalBounds().getX() + processor.uiMargin,
		                                   getLocalBounds().getY() + processor.uiMargin,
		                                   getLocalBounds().getWidth() - processor.uiMargin * 2,
		                                   getLocalBounds().getHeight() - processor.uiMargin * 2);
	}

	virtual void defineComponents()
	{
	}

	virtual void addComponents()
	{
	}

	void drawSectionImage(juce::Graphics& g, juce::Image sectionImage)
	{
		auto* image = &sectionImage;
		g.drawImageWithin(*image,
		                  getLocalBounds().getX(),
		                  getLocalBounds().getY(),
		                  getLocalBounds().getWidth(),
		                  getLocalBounds().getHeight(), juce::RectanglePlacement::fillDestination);
	}

	MySamplerAudioProcessor& processor;

	CustomGraphics mLookAndFeel;

	juce::Rectangle<int> localBounds;

	const bool debugBoundRects = true;

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyComponentBase)
};
