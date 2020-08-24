/*
  ==============================================================================

    VisualEnvelopeComponent.h
    Created: 22 Aug 2020 5:35:02pm
    Author:  Khalil Kabara

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "MyComponentBase.h"

//==============================================================================

class VisualEnvelopeComponent : public MyComponentBase, Timer
{
public:

	VisualEnvelopeComponent(MySamplerAudioProcessor& p) : MyComponentBase(p)
	{
		// visualEnvelopeSectionImage = ImageCache::getFromMemory(BinaryData::reverb_section_art_png,
		//                                                BinaryData::reverb_section_art_pngSize);

		startTimerHz(fps);
	}

	~VisualEnvelopeComponent() override
	{
	}

	void paint(Graphics& g) override
	{
		mPaint(g, visualEnvelopeSectionImage);

		if (debugBoundRects) g.drawRect(getLocalBounds());

		drawVisualEnvelope(g);
	}

	void resized() override
	{
	}

private:

	void defineRects() override
	{
		MyComponentBase::defineRects();
	}

	void defineComponents() override
	{
		MyComponentBase::defineComponents();
	}

	void addComponents() override
	{
	}

	void timerCallback() override
	{
		currentAttack = static_cast<float>(
			*processor.valueTreeState.getRawParameterValue(processor.envelopeAttackStateName));

		currentDecay = static_cast<float>(
			*processor.valueTreeState.getRawParameterValue(processor.envelopeDecayStateName));

		currentSustain = static_cast<float>(
			*processor.valueTreeState.getRawParameterValue(processor.envelopeSustainStateName));

		currentRelease = static_cast<float>(
			*processor.valueTreeState.getRawParameterValue(processor.envelopeReleaseStateName));

		// Normalize

		currentAttack = jmap<float>(currentAttack,
		                            processor.zeroToTenMinValue,
		                            processor.zeroToTenMaxValue,
		                            0, 1);

		currentDecay = jmap<float>(currentDecay,
		                           processor.zeroToTenMinValue,
		                           processor.zeroToTenMaxValue,
		                           0, 1);

		currentSustain = jmap<float>(currentSustain,
		                             processor.zeroToOneMinValue,
		                             processor.zeroToOneMaxValue,
		                             0, 1);

		currentRelease = jmap<float>(currentRelease,
		                             processor.zeroToTenMinValue,
		                             processor.zeroToTenMaxValue,
		                             0, 1);


		if (currentAttack != lastAttack || currentDecay != lastDecay ||
			currentSustain != lastSustain || currentRelease != lastRelease)
		{
			lastAttack = currentAttack;
			lastDecay = currentDecay;
			lastSustain = currentSustain;
			lastRelease = currentRelease;

			repaint();
		}
	}

	void drawVisualEnvelope(Graphics& g)
	{
		g.setColour(Colours::whitesmoke);

		Path path;
		const auto radius = 1;
		const auto angle = 300;
		const auto radius2 = 40;
		const auto angle2 = 300;

		// Point<float> p1(localBounds.getX() + localBounds.getWidth()/ 2, localBounds.getY());
		// Point<float> p2(localBounds.getX() + localBounds.getWidth(), localBounds.getY() + localBounds.getHeight());
		// path.cubicTo(p1.getPointOnCircumference(radius, angle), p2.getPointOnCircumference(radius2, angle2), p2);
		// path.scaleToFit(localBounds.getX() + localBounds.getWidth() / 2, localBounds.getY(),
		// 	localBounds.getX() + localBounds.getWidth(), localBounds.getY() + localBounds.getHeight(), true);

		// g.strokePath(path, PathStrokeType(lineThickness));
		
		const auto sustainEndY = localBounds.getY() + localBounds.getHeight() / 4;
		
		// Attack
		minAttackPos = localBounds.getX();
		maxAttackPos = localBounds.getX() + localBounds.getWidth() / 4;
		g.drawLine(minAttackPos, localBounds.getY() + localBounds.getHeight(),
		           maxAttackPos * currentAttack, localBounds.getY(), lineThickness);
		
		// Decay
		minDecayPos = maxAttackPos * currentAttack;
		maxDecayPos = minDecayPos + localBounds.getWidth() / 4;
		g.drawLine(minDecayPos, localBounds.getY(),
		           minDecayPos + (maxDecayPos - minDecayPos) * currentDecay, localBounds.getY(), lineThickness);
		
		// Sustain
		minSustainPos = minDecayPos + (maxDecayPos - minDecayPos) * currentDecay;
		maxSustainPos = minSustainPos + localBounds.getWidth() / 4;
		g.drawLine(minSustainPos, localBounds.getY(),
		           minSustainPos + (maxSustainPos - minSustainPos) * currentSustain, sustainEndY, lineThickness);
		
		// Release
		minReleasePos = minSustainPos + (maxSustainPos - minSustainPos) * currentSustain;
		maxReleasePos = localBounds.getWidth();
		g.drawLine(minReleasePos, sustainEndY, minReleasePos + (maxReleasePos - minReleasePos) * currentRelease,
		           localBounds.getY() + localBounds.getHeight(), lineThickness);
	}

	const int fps = 15;
	const float lineThickness = 2;
	// int valueWidthRatio;
	Image visualEnvelopeSectionImage;

	float lastAttack, lastDecay, lastSustain, lastRelease;
	float currentAttack, currentDecay, currentSustain, currentRelease;

	float minAttackPos;
	float maxAttackPos;
	float minDecayPos;
	float maxDecayPos;
	float minSustainPos;
	float maxSustainPos;
	float minReleasePos;
	float maxReleasePos;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VisualEnvelopeComponent)
};
