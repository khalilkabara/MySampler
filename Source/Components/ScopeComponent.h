/*
  ==============================================================================

    ScopeComponent.h
    Created: 13 Aug 2020 5:27:24pm
    Author:  Khalil Kabara

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


#include "MyComponentBase.h"

//==============================================================================

class ScopeComponent : public MyComponentBase, private juce::Timer
{
public:
	ScopeComponent(MySamplerAudioProcessor& p) : MyComponentBase(p)
	{
		jassert(fps > 0 && fps < 1000);
		startTimerHz(fps);
	}

	~ScopeComponent() override
	{
	}

	void paint(juce::Graphics& g) override
	{
		g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // clear the background

		g.setColour(juce::Colours::whitesmoke);

		const auto leftChannelData = processor.leftBufferHistory;
		const auto rightChannelData = processor.rightBufferHistory;

		paintChannel(g, leftChannelData, leftChannelRect);
		paintChannel(g, rightChannelData, rightChannelRect);

		if (debugBoundRects)
			g.drawRect(getLocalBounds());
		
		// g.drawRect(leftChannelRect);
		// g.drawRect(rightChannelRect);
	}

	void resized() override
	{
	}

	void defineRects() override
	{
		MyComponentBase::defineRects();

		leftChannelRect = juce::Rectangle<int>(
			getLocalBounds().getX(),
			getLocalBounds().getY(),
			getLocalBounds().getWidth(),
			getLocalBounds().getHeight() / 2);

		rightChannelRect = juce::Rectangle<int>(
			leftChannelRect.getX(),
			leftChannelRect.getY() + leftChannelRect.getHeight(),
			leftChannelRect.getWidth(),
			leftChannelRect.getHeight());
	}

	void defineComponents() override
	{
	}

	void addComponents() override
	{
	}

private:

	const int fps = 10;
	const int border = 10;

	juce::Rectangle<int> leftChannelRect;
	juce::Rectangle<int> rightChannelRect;

	void timerCallback() override
	{
		repaint();
	}

	void paintChannel(Graphics& g, const Array<float>& channelData, juce::Rectangle<int> rect) const
	{
		Path path;
		float max = 0;

		for (auto i = 0; i < rect.getWidth(); ++i)
		{
			const auto selectedIndex = static_cast<float>(i - channelData.size() /
				static_cast<float>(rect.getWidth()));
			auto val = channelData[selectedIndex];
			val = jlimit<float>(-1, 1, val);

			if (i == 0)
			{
				path.startNewSubPath(0, 1);
				path.startNewSubPath(0, -1);
				path.startNewSubPath(0, val);
			}
			else path.lineTo(i, val);

			max = FloatVectorOperations::findMaximum(channelData.getRawDataPointer(), channelData.size());
			max = jlimit<float>(0, 2, max);
		}

		// Check for Nan etc
		if (path.isEmpty()) return;
		if (path.getBounds().getWidth() < 0.01) return;
		// if (path.getBounds().getWidth() != path.getBounds().getWidth()) return;

		// Rescale
		path.scaleToFit(rect.getX(),
		                rect.getY() + 0.1f * rect.getHeight(),
		                rect.getWidth(),
		                0.8f * rect.getHeight(),
		                false);

		// Fade

		// Gradient
		const auto colour1 = juce::Colours::blue
		                     .withRotatedHue(0.2f + max).withBrightness(0.5f + 0.5f * max).withSaturation(
			                     0.1f + 0.9f * max);
		const auto colour2 = juce::Colours::red.withMultipliedBrightness(0.5f + 0.5f * max).withSaturation(
			0.1f + 0.9f * max);

		const ColourGradient gradient(colour1, rect.getWidth() / 2, rect.getHeight() / 2,
		                              colour2, rect.getX(), rect.getHeight() / 2, false);

		g.setGradientFill(gradient);

		g.strokePath(path, PathStrokeType(0.5f + max / 2));
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScopeComponent)
};
