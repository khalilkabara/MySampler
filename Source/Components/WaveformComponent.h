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
		// if (!editorHasBeenInitialized) { mPaint(g, waveformSectionImage); editorHasBeenInitialized = true; }
		mPaint(g, waveformSectionImage);
		
		if (debugBoundRects) g.drawRect(getLocalBounds());

		if (processor.noFileLoadedYet)
		{
			g.drawFittedText("Drag And Drop Audio File", localBounds, Justification::centred, 1);
		}

		if (fileIsBeingDragged)
		{
			g.setColour(Colours::whitesmoke);
			g.setOpacity(0.5);
			g.drawFittedText("Drop File", fileDragIndicatorRect, Justification::centred, 1);
			g.fillRoundedRectangle(fileDragIndicatorRect.getX(),
			                       fileDragIndicatorRect.getY(),
			                       fileDragIndicatorRect.getWidth(),
			                       fileDragIndicatorRect.getHeight(),
			                       5);
		}

		drawWaveform(g);
		
		drawPlayhead(g);
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

	Image waveformSectionImage;

	int currentPlayheadPosition{0};
	const int playheadLineThickness{2};
	int samplesBoundsRatio;
	const int fps = 10;
	Array<float> channelData;
	bool fileIsBeingDragged = false;

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
		if(processor.newFileLoaded)
		{
			repaint();
			processor.newFileLoaded = false;
			return;
		}
		currentPlayheadPosition = processor.lastPlaybackPosition;
		repaint();
	}

	void drawWaveform(Graphics& g)
	{
		if(processor.noFileLoadedYet) return;

		samplesBoundsRatio = processor.getLoadedFileWaveform().getNumSamples() / localBounds.getWidth();

		const float* buffer = processor.loadedFileWaveform.getReadPointer(0);

		channelData.clear();

		for (auto sample = 0; sample + samplesBoundsRatio < processor.loadedFileWaveform.getNumSamples(); sample += samplesBoundsRatio)
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

		// Rescale
		path.scaleToFit(localBounds.getX(),
		                localBounds.getY() + 0.1f * localBounds.getHeight(),
		                localBounds.getWidth(),
		                0.8f * localBounds.getHeight(),
		                false);

		g.setColour(Colours::whitesmoke);

		g.strokePath(path, PathStrokeType(0.5f + max / 2));

	}

	void drawPlayhead(Graphics& g) const
	{
		g.setColour(Colours::red);

		const auto xPos = currentPlayheadPosition / samplesBoundsRatio;

		const Line<float> line{
			static_cast<float>(xPos),
			static_cast<float>(localBounds.getY()),
			static_cast<float>(xPos + playheadLineThickness),
			static_cast<float>(localBounds.getY() + localBounds.getHeight())};
		
		g.drawLine(line);
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformComponent)
};
