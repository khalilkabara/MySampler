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
                          public FileDragAndDropTarget
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

		if (debugBoundRects)
			g.drawRect(getLocalBounds());

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
		else
		{

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

	const int labelRectHeight = 10;

	// Binary Data
	Image waveformSectionImage;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformComponent)
};
