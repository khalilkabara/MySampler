/*
  ==============================================================================

    CustomGraphics.h
    Created: 20 Aug 2020 9:42:14pm
    Author:  Khalil Kabara

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class CustomGraphics : public juce::LookAndFeel_V4
{
public:
	CustomGraphics();
	~CustomGraphics() {}
	void drawRotarySlider(Graphics&, int x, int y, int width, int height,
		float sliderPosProportional, float rotaryStartAngle,
		float rotaryEndAngle, Slider&) override;

	void drawLinearSlider(Graphics&, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle, Slider&) override;

	void drawToggleButton(Graphics& g, ToggleButton&, bool isMouseOverButton, bool isButtonDown) override;

	void drawButtonBackground(Graphics&, Button&, const Colour& backgroundColour,
		bool isMouseOverButton, bool isButtonDown) override;


	void drawComboBox(Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW,
		int buttonH, ComboBox&) override;

private:

	void DrawKnobImage(Graphics& g, Image& img, int x, int y, int width, int height, float angle) const;

	static bool IsOfType(const std::string& type, TooltipClient& slider);

	bool isSwitch = false;
	bool isBipolar = false;
	bool isEffect = false;
	bool isBipolarEffect = false;

	Image knobImage;
	Image switchKnobImage;
	Image sliderHandleImage;
	Image sliderTrackImage;
	Image buttonImage;
	Image comboBoxImage;

	Colour toggleFillColor = juce::Colours::red;
	Colour knobPathColor = juce::Colours::cyan;
	Colour effectKnobPathColor = juce::Colours::blueviolet;
};
