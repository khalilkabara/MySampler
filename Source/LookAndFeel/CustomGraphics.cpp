/*
  ==============================================================================

    CustomGraphics.cpp
    Created: 20 Aug 2020 9:42:14pm
    Author:  Khalil Kabara

  ==============================================================================
*/


#define POWER_ARC_ANGLE 2.5

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

// void CustomGraphics::drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
// 	float minSliderPos, float maxSliderPos, const Slider::SliderStyle, Slider& slider)
// {
// 	const float gapThickness = 10;
// 	Rectangle<float> gapTarget(x + (width - gapThickness) / 2, y, gapThickness, height);
// 	g.drawImage(sliderTrackImage, gapTarget, RectanglePlacement::centred | RectanglePlacement::stretchToFit);
//
// 	Rectangle<float> handleTarget(x + width * 0.1, sliderPos - height / 2, width * 0.8, height);
// 	g.drawImage(sliderHandleImage, handleTarget, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize);
// }

//
// void CustomGraphics::drawToggleButton(Graphics& g, ToggleButton& b, bool isMouseOverButton, bool isButtonDown)
// {
// 	const double width = b.getWidth();
// 	const double height = b.getHeight();
// 	const Rectangle<float> buttonTarget(0, 0, width, height);
// 	g.drawImage(buttonImage, buttonTarget, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize);
//
// 	const float size = 0.4;
// 	g.setColour(toggleFillColor);
// 	if (b.getToggleState())
// 		g.fillEllipse(width / 2 * (1 - size), height / 2 * (1 - size), width * size, height * size);
// }
//
//
// void CustomGraphics::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
// 	bool isMouseOverButton, bool isButtonDown)
// {
// 	if (IsOfType("no_background", button))
// 	{
// 		if (isMouseOverButton)
// 		{
// 			g.setColour(Colour((uint8)255, 255, 255, (uint8)20));
// 			g.fillRect(0, 0, button.getWidth(), button.getHeight());
// 		}
// 		return;
// 	}
//
// 	LookAndFeel_V4::drawButtonBackground(g, button, backgroundColour, isMouseOverButton, isButtonDown);
// }

//
// void CustomGraphics::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
// 	float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
// 	Slider& slider)
// {
// 	auto* image = &knobImage;
//
// 	if (IsOfType("switch", slider))
// 	{
// 		image = &switchKnobImage;
// 		isSwitch = true;
// 	}
// 	else isSwitch = false;
//
// 	if (IsOfType("bipolar", slider)) isBipolar = true;
// 	else isBipolar = false;
//
// 	if (IsOfType("effect", slider)) isEffect = true;
// 	else isBipolar = isEffect;
//
// 	if (IsOfType("bipolarEffect", slider)) isBipolarEffect = true;
// 	else isBipolarEffect = false;
//
// 	const auto rangeScale = isSwitch ? 0.5f : 1.0f;
// 	const auto range = rotaryEndAngle - rotaryStartAngle;
// 	const auto angle = rotaryStartAngle + range * sliderPosProportional * rangeScale + range * (1 - rangeScale) / 2;
//
// 	if (width > height)
// 	{
// 		x = width / 2 - height / 2;
// 		width = height;
// 	}
// 	if (height > width)
// 	{
// 		y = height / 2 - width / 2;
// 		height = width;
// 	}
//
// 	DrawKnobImage(g, *image, x, y, width, height, angle);
//
// 	if (isSwitch) return;
//
// 	Path p;
// 	if (isEffect || isBipolarEffect) g.setColour(effectKnobPathColor);
// 	else g.setColour(knobPathColor);
//
// 	if (isBipolar || isBipolarEffect)
// 	{
// 		// g.setColour(effectKnobPathColor);
// 		p.addArc(x, y, width, height, 2 * juce::MathConstants<float>::pi, angle, true);
// 	}
// 	else
// 	{
// 		// g.setColour(knobPathColor);
// 		p.addArc(x, y, width, height, rotaryStartAngle, angle, true);
// 	}
//
// 	p.applyTransform(AffineTransform::scale(0.85, 0.85, x + width / 2, y + height / 2));
// 	g.strokePath(p, PathStrokeType(3));
// }

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

//*****************************************************************************************

void CustomGraphics::drawButtonBackground(Graphics& g, Button& button,
                                          const Colour& backgroundColour, bool hover, bool is_down)
{
	if (button.isEnabled())
		g.fillAll(Colour(0xff323232));
	else
		g.fillAll(Colour(0xff484848));

	g.setFont(proportional_regular_);
	g.setColour(Colour(0xff505050));
	g.drawRect(button.getLocalBounds());

	if (is_down)
		g.fillAll(Colour(0x11000000));
	else if (hover)
		g.fillAll(Colour(0x11ffffff));
}

void CustomGraphics::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                                      float slider_pos, float min, float max,
                                      const Slider::SliderStyle style, Slider& slider)
{
	static const DropShadow thumb_shadow(Colour(0x88000000), 3, Point<int>(-1, 0));

	bool bipolar = false;
	bool flip_coloring = false;
	bool active = true;

	Colour slider_color(0xff888888);
	Colour lighten_color(0x55ffffff);
	Colour thumb_color(0xffffffff);

	if (!active)
	{
		slider_color = Colour(0xff424242);
		thumb_color = Colour(0xff888888);
		lighten_color = Colour(0x22ffffff);
	}

	float pos = slider_pos - 1.0f;
	if (style == Slider::SliderStyle::LinearBar)
	{
		g.setColour(Colour(0x22000000));
		float w = slider.getWidth();
		float h = slider.getHeight();
		g.fillRect(0.0f, 0.0f, w, h);

		g.setColour(Colour(0xff2a2a2a));
		fillSplitHorizontalRect(g, 0.0f, w, h, Colours::transparentBlack);

		g.setColour(slider_color);
		if (bipolar)
			fillSplitHorizontalRect(g, w / 2.0f, pos, h, lighten_color);
		else if (flip_coloring)
			fillSplitHorizontalRect(g, pos, w - pos, h, lighten_color);
		else
			fillSplitHorizontalRect(g, 0.0f, pos, h, lighten_color);

		thumb_shadow.drawForRectangle(g, Rectangle<int>(pos + 0.5f, 0, 2, h));
		g.setColour(thumb_color);
		g.fillRect(pos, 0.0f, 2.0f, h);
	}
	else if (style == Slider::SliderStyle::LinearBarVertical)
	{
		g.setColour(Colour(0x22000000));
		float w = slider.getWidth();
		float h = slider.getHeight();
		g.fillRect(0.0f, 0.0f, w, h);

		g.setColour(Colour(0xff2a2a2a));
		fillSplitVerticalRect(g, 0.0f, h, w, Colours::transparentBlack);

		g.setColour(slider_color);
		if (bipolar)
			fillSplitVerticalRect(g, h / 2.0f, pos, w, lighten_color);
		else if (flip_coloring)
			fillSplitVerticalRect(g, h + 1, pos, w, lighten_color);
		else
			fillSplitVerticalRect(g, 0, pos, w, lighten_color);

		thumb_shadow.drawForRectangle(g, Rectangle<int>(0, pos + 0.5f, w, 2));
		g.setColour(thumb_color);
		g.fillRect(0.0f, pos, w, 2.0f);
	}
}

// void CustomGraphics::drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
// 	float slider_pos, float min, float max,
// 	const Slider::SliderStyle style, Slider& slider) {
// 	LookAndFeel_V3::drawLinearSliderThumb(g, x, y, width, height,
// 		slider_pos, min, max, style, slider);
// }

void CustomGraphics::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                                      float slider_t, float start_angle, float end_angle,
                                      Slider& slider)
{
	static const float stroke_percent = 0.1f;

	float full_radius = std::min(width / 2.0f, height / 2.0f);
	float stroke_width = 2.0f * full_radius * stroke_percent;
	float knob_radius = 0.63f * full_radius;
	float small_outer_radius = knob_radius + stroke_width / 6.0f;
	PathStrokeType outer_stroke =
		PathStrokeType(stroke_width, PathStrokeType::beveled, PathStrokeType::butt);

	float current_angle = start_angle + slider_t * (end_angle - start_angle);
	float end_x = full_radius + 0.8f * knob_radius * sin(current_angle);
	float end_y = full_radius - 0.8f * knob_radius * cos(current_angle);

	if (slider.getInterval() == 1)
	{
		static const float TEXT_W_PERCENT = 0.35f;
		Rectangle<float> text_bounds(1.0f + width * (1.0f - TEXT_W_PERCENT) / 2.0f,
		                             0.5f * height, width * TEXT_W_PERCENT, 0.5f * height);

		g.setColour(Colour(0xff464646));
		g.fillRoundedRectangle(text_bounds, 2.0f);

		g.setColour(Colour(0xff999999));
		g.setFont(proportional_regular_.withPointHeight(0.2f * height));
		g.drawFittedText(String(slider.getValue()), text_bounds.getSmallestIntegerContainer(),
		                 Justification::horizontallyCentred | Justification::bottom, 1);
	}

	Path active_section;
	bool bipolar = false;
	const bool active = true;

	if (IsOfType("switch", slider))
	{
		isSwitch = true;
	}
	else isSwitch = false;

	if (IsOfType("bipolar", slider))
	{
		isBipolar = true;
		bipolar = true;
	}
	else isBipolar = false;

	if (IsOfType("effect", slider)) isEffect = true;
	else isBipolar = isEffect;

	if (IsOfType("bipolarEffect", slider)) isBipolarEffect = true;
	else isBipolarEffect = false;
	
	Path rail;
	rail.addCentredArc(full_radius, full_radius, small_outer_radius, small_outer_radius,
	                   0.0f, start_angle, end_angle, true);

	if (active)
		g.setColour(Colour(0xff4a4a4a));
	else
		g.setColour(Colour(0xff333333));

	g.strokePath(rail, outer_stroke);

	if (bipolar)
	{
		active_section.addCentredArc(full_radius, full_radius, small_outer_radius, small_outer_radius,
		                             0.0f, 0.0f, current_angle - 2.0f * juce::MathConstants<float>::pi, true);
	}
	else
	{
		active_section.addCentredArc(full_radius, full_radius, small_outer_radius, small_outer_radius,
		                             0.0f, start_angle, current_angle, true);
	}

	if (active)
		g.setColour(Colour(0xffffab00));
	else
		g.setColour(Colour(0xff555555));

	g.strokePath(active_section, outer_stroke);

	if (active)
		g.setColour(Colour(0xff000000));
	else
		g.setColour(Colour(0xff444444));

	g.fillEllipse(full_radius - knob_radius,
	              full_radius - knob_radius,
	              2.0f * knob_radius,
	              2.0f * knob_radius);

	if (active)
		g.setColour(Colour(0xff666666));
	else
		g.setColour(Colour(0xff555555));

	g.drawEllipse(full_radius - knob_radius + stroke_width / 4.0f + 0.5f,
	              full_radius - knob_radius + stroke_width / 4.0f + 0.5f,
	              2.0f * knob_radius - stroke_width / 2.0f - 1.0f,
	              2.0f * knob_radius - stroke_width / 2.0f - 1.0f, 1.5f);

	g.setColour(Colour(0xff999999));
	g.drawLine(full_radius, full_radius, end_x, end_y, 1.0f);
}

void CustomGraphics::drawToggleButton(Graphics& g, ToggleButton& button,
                                      bool hover, bool is_down)
{
	static const DropShadow shadow(Colour(0x88000000), 1.0f, Point<int>(0, 0));
	static float stroke_percent = 0.1f;
	float ratio = button.getWidth() / 20.0f;
	float padding = ratio * 3.0f;
	float hover_padding = ratio;

	float full_radius = std::min(button.getWidth(), button.getHeight()) / 2.0;
	float stroke_width = 2.0f * full_radius * stroke_percent;
	PathStrokeType stroke_type(stroke_width, PathStrokeType::beveled, PathStrokeType::rounded);
	float outer_radius = full_radius - stroke_width - padding;
	Path outer;
	outer.addCentredArc(full_radius, full_radius, outer_radius, outer_radius,
	                    juce::MathConstants<float>::pi, -POWER_ARC_ANGLE, POWER_ARC_ANGLE, true);

	Path shadow_path;
	stroke_type.createStrokedPath(shadow_path, outer);
	shadow.drawForPath(g, shadow_path);
	Rectangle<int> bar_shadow_rect(full_radius - 1.0f, padding, 2.0f, full_radius - padding);
	shadow.drawForRectangle(g, bar_shadow_rect);

	if (button.getToggleState())
		g.setColour(Colours::white);
	else
		g.setColour(Colours::grey);

	g.strokePath(outer, stroke_type);
	g.fillRoundedRectangle(full_radius - 1.0f, padding, 2.0f, full_radius - padding, 1.0f);

	if (is_down)
	{
		g.setColour(Colour(0x11000000));
		g.fillEllipse(hover_padding, hover_padding,
		              button.getWidth() - 2 * hover_padding, button.getHeight() - 2 * hover_padding);
	}
	else if (hover)
	{
		g.setColour(Colour(0x11ffffff));
		g.fillEllipse(hover_padding, hover_padding,
		              button.getWidth() - 2 * hover_padding, button.getHeight() - 2 * hover_padding);
	}
}


void CustomGraphics::fillHorizontalRect(Graphics& g, float x1, float x2, float height) const
{
	float x = std::min(x1, x2);
	float width = fabsf(x1 - x2);
	g.fillRect(x, 0.0f, width, height);
}

void CustomGraphics::fillVerticalRect(Graphics& g, float y1, float y2, float width) const
{
	float y = std::min(y1, y2);
	float height = fabsf(y1 - y2);
	g.fillRect(0.0f, y, width, height);
}

void CustomGraphics::fillSplitHorizontalRect(Graphics& g, float x1, float x2, float height,
                                             Colour fill_color) const
{
	float h = (height - linear_rail_width) / 2.0f;
	float x = std::min(x1, x2);
	float width = fabsf(x1 - x2);

	g.saveState();
	g.setColour(fill_color);
	g.fillRect(x, 0.0f, width, height);
	g.restoreState();

	g.fillRect(x, 0.0f, width, h);
	g.fillRect(x, h + linear_rail_width, width, h);
}

void CustomGraphics::fillSplitVerticalRect(Graphics& g, float y1, float y2, float width,
                                           Colour fill_color) const
{
	float w = (width - linear_rail_width) / 2.0f;
	float y = std::min(y1, y2);
	float height = fabsf(y1 - y2);

	g.saveState();
	g.setColour(fill_color);
	g.fillRect(0.0f, y, width, height);
	g.restoreState();

	g.fillRect(0.0f, y, w, height);
	float x2 = w + linear_rail_width;
	g.fillRect(x2, y, width - x2, height);
}
