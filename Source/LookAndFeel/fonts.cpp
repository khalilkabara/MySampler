
#include "fonts.h"

Fonts::Fonts() {
  proportional_regular_ = Font(Typeface::createSystemTypefaceFor(
      BinaryData::RobotoRegular_ttf, BinaryData::RobotoRegular_ttfSize));
  proportional_light_ = Font(Typeface::createSystemTypefaceFor(
      BinaryData::RobotoLight_ttf, BinaryData::RobotoLight_ttfSize));
  monospace_ = Font(Typeface::createSystemTypefaceFor(
      BinaryData::DroidSansMono_ttf, BinaryData::DroidSansMono_ttfSize));
}
