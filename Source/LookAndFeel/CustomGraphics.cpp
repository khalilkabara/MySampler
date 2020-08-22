/*
  ==============================================================================

    CustomGraphics.cpp
    Created: 20 Aug 2020 9:42:14pm
    Author:  Khalil Kabara

  ==============================================================================
*/

#include "CustomGraphics.h"
CustomGraphics::CustomGraphics()
{
	knobImage = ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
	switchKnobImage = ImageCache::getFromMemory(BinaryData::switch_knob_png, BinaryData::switch_knob_pngSize);
	sliderHandleImage = ImageCache::getFromMemory(BinaryData::slider_handle_png, BinaryData::slider_handle_pngSize);
	sliderTrackImage = ImageCache::getFromMemory(BinaryData::slider_gap_png, BinaryData::slider_gap_pngSize);
	buttonImage = ImageCache::getFromMemory(BinaryData::toggle_button_png, BinaryData::toggle_button_pngSize);
	comboBoxImage = ImageCache::getFromMemory(BinaryData::combo_box_png, BinaryData::combo_box_pngSize);
}

void CustomGraphics::drawToggleButton(Graphics& g, ToggleButton& b, bool isMouseOverButton, bool isButtonDown)
{
	const double width = b.getWidth();
	const double height = b.getHeight();
	const Rectangle<float> buttonTarget(0, 0, width, height);
	g.drawImage(buttonImage, buttonTarget, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize);

	const float size = 0.4;
	g.setColour(toggleFillColor);
	if (b.getToggleState())
		g.fillEllipse(width / 2 * (1 - size), height / 2 * (1 - size), width * size, height * size);
}

void CustomGraphics::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
	bool isMouseOverButton, bool isButtonDown)
{
	if (IsOfType("no_background", button))
	{
		if (isMouseOverButton)
		{
			g.setColour(Colour((uint8)255, 255, 255, (uint8)20));
			g.fillRect(0, 0, button.getWidth(), button.getHeight());
		}
		return;
	}

	LookAndFeel_V4::drawButtonBackground(g, button, backgroundColour, isMouseOverButton, isButtonDown);
}

void CustomGraphics::drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
	float minSliderPos, float maxSliderPos, const Slider::SliderStyle, Slider& slider)
{
	const float gapThickness = 10;
	Rectangle<float> gapTarget(x + (width - gapThickness) / 2, y, gapThickness, height);
	g.drawImage(sliderTrackImage, gapTarget, RectanglePlacement::centred | RectanglePlacement::stretchToFit);

	Rectangle<float> handleTarget(x + width * 0.1, sliderPos - height / 2, width * 0.8, height);
	g.drawImage(sliderHandleImage, handleTarget, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize);
}

void CustomGraphics::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
	float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
	Slider& slider)
{
	auto* image = &knobImage;

	if (IsOfType("switch", slider))
	{
		image = &switchKnobImage;
		isSwitch = true;
	}
	else isSwitch = false;

	if (IsOfType("bipolar", slider)) isBipolar = true;
	else isBipolar = false;

	if (IsOfType("effect", slider)) isEffect = true;
	else isBipolar = isEffect;

	if (IsOfType("bipolarEffect", slider)) isBipolarEffect = true;
	else isBipolarEffect = false;

	const auto rangeScale = isSwitch ? 0.5f : 1.0f;
	const auto range = rotaryEndAngle - rotaryStartAngle;
	const auto angle = rotaryStartAngle + range * sliderPosProportional * rangeScale + range * (1 - rangeScale) / 2;

	if (width > height)
	{
		x = width / 2 - height / 2;
		width = height;
	}
	if (height > width)
	{
		y = height / 2 - width / 2;
		height = width;
	}

	DrawKnobImage(g, *image, x, y, width, height, angle);

	if (isSwitch) return;

	Path p;
	if (isEffect || isBipolarEffect) g.setColour(effectKnobPathColor);
	else g.setColour(knobPathColor);

	if (isBipolar || isBipolarEffect)
	{
		// g.setColour(effectKnobPathColor);
		p.addArc(x, y, width, height, 2 * juce::MathConstants<float>::pi, angle, true);
	}
	else
	{
		// g.setColour(knobPathColor);
		p.addArc(x, y, width, height, rotaryStartAngle, angle, true);
	}

	p.applyTransform(AffineTransform::scale(0.85, 0.85, x + width / 2, y + height / 2));
	g.strokePath(p, PathStrokeType(3));
}

void CustomGraphics::DrawKnobImage(Graphics& g, Image& img, int x, int y, int width, int height, float angle) const
{
	const auto destAspect = static_cast<float>(width) / height;
	const auto srcAspect = static_cast<float>(img.getWidth()) / img.getHeight();
	if (destAspect < srcAspect)
	{
		height = width / srcAspect;
	}
	else if (destAspect > srcAspect)
	{
		width = height * srcAspect;
	}

	auto transform = AffineTransform::fromTargetPoints(0, 0, x, y, img.getWidth(), 0, x + width, y, 0, img.getHeight(),
		x, y + height)
		.followedBy(AffineTransform::rotation(angle, x + width / 2, y + height / 2))
		.followedBy(AffineTransform::scale(0.85, 0.85, x + width / 2, y + height / 2));

	g.drawImageTransformed(img, transform);
}

void CustomGraphics::drawComboBox(Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY,
	int buttonW,
	int buttonH, ComboBox& cb)
{
	const Rectangle<float> comboBoxTarget(-3, 0, width + 3, height);

	g.drawImage(comboBoxImage, comboBoxTarget,
		RectanglePlacement::fillDestination | RectanglePlacement::onlyReduceInSize);
}

bool CustomGraphics::IsOfType(const std::string& type, TooltipClient& component)
{
	const auto tooltip = component.getTooltip().toStdString();
	auto p = tooltip.find_first_of(";", 0);
	if (p == std::string::npos)
		return false;
	const auto types = tooltip.substr(0, p);
	return types.find(type, 0) != std::string::npos;
}
