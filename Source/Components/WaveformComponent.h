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

class WaveformComponent : public MyComponentBase,
                          public FileDragAndDropTarget,
                          Timer
{
public:
	WaveformComponent(MySamplerAudioProcessor& p) : MyComponentBase(p)
	{
		// waveformSectionImage = ImageCache::getFromMemory(BinaryData::reverb_section_art_png,
		//                                                BinaryData::reverb_section_art_pngSize);

		startTimerHz(fps);
	}

	~WaveformComponent()
	{
	}

	void paint(Graphics& g) override
	{
		mPaint(g, waveformSectionImage);

		if (debugBoundRects)
			g.drawRect(getLocalBounds());

		if (processor.noFileLoadedYet)
		{
			g.drawFittedText("Drag And Drop Audio File", localBounds, Justification::centred, 1);
		}

		if (fileIsBeingDragged)
		{
			g.setColour(Colours::whitesmoke);
			g.setOpacity(0.5);
			g.drawFittedText("Drop File", fileDragIndicatorRect, Justification::centred, 1);
			// g.fillRect(fileDragIndicatorRect);
			g.fillRoundedRectangle(fileDragIndicatorRect.getX(),
			                       fileDragIndicatorRect.getY(),
			                       fileDragIndicatorRect.getWidth(),
			                       fileDragIndicatorRect.getHeight(),
			                       5);
		}

		if (newFileLoaded)
		{
			drawWaveform(g);
			processor.newFileLoaded = false;
			processor.clearLoadedWaveform();
		}
	}

	void resized() override
	{
	}

	void filesDropped(const StringArray& files, int x, int y) override
	{
		processor.loadFile(files[0]);
		fileIsBeingDragged = false;
		repaint();
	}

	bool isInterestedInFileDrag(const StringArray& files) override
	{
		return true;
	}

	void fileDragEnter(const StringArray& files, int x, int y) override
	{
		fileIsBeingDragged = true;
		repaint();
	}

	void fileDragExit(const StringArray& files) override
	{
		fileIsBeingDragged = false;
		repaint();
	}

private:

	const int fps = 10;
	Array<float> channelData;
	bool fileIsBeingDragged = false;
	bool newFileLoaded;

	juce::Rectangle<int> fileDragIndicatorRect;

	// Functions

	void defineRects() override
	{
		MyComponentBase::defineRects();

		fileDragIndicatorRect = localBounds;
	}

	void defineComponents() override
	{
	}

	void addComponents() override
	{
	}

	void timerCallback() override
	{
		newFileLoaded = processor.newFileLoaded;
		if (newFileLoaded) repaint();
	}

	void drawWaveform(Graphics& g)
	{
		const auto waveform = processor.getLoadedFileWaveform();
		auto ratio = waveform.getNumSamples() / localBounds.getWidth();

		const float* buffer = processor.loadedFileWaveform.getReadPointer(0);

		channelData.clear();

		for (auto sample = 0; sample + ratio < processor.loadedFileWaveform.getNumSamples(); sample += ratio)
		{
			channelData.add(buffer[sample]);
		}

		Path path;
		float max = 0;

		for (auto i = 0; i < localBounds.getWidth(); ++i)
		{
			const auto selectedIndex = static_cast<float>(i - channelData.size() /
				static_cast<float>(localBounds.getWidth()));
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
		path.scaleToFit(localBounds.getX(),
		                localBounds.getY() + 0.1f * localBounds.getHeight(),
		                localBounds.getWidth(),
		                0.8f * localBounds.getHeight(),
		                false);

		// Fade

		// Gradient
		// const auto colour1 = juce::Colours::blue
		//                      .withRotatedHue(0.2f + max).withBrightness(0.5f + 0.5f * max).withSaturation(
		// 	                     0.1f + 0.9f * max);
		// const auto colour2 = juce::Colours::red.withMultipliedBrightness(0.5f + 0.5f * max).withSaturation(
		// 	0.1f + 0.9f * max);
		//
		// const ColourGradient gradient(colour1, localBounds.getWidth() / 2, localBounds.getHeight() / 2,
		//                               colour2, localBounds.getX(), localBounds.getHeight() / 2, false);
		//
		// g.setGradientFill(gradient);

		g.setColour(Colours::whitesmoke);

		g.strokePath(path, PathStrokeType(0.5f + max / 2));
	}

	const int labelRectHeight = 10;

	// Binary Data
	Image waveformSectionImage;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformComponent)
};
