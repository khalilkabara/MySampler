
#ifndef FONTS_H
#define FONTS_H

#include "JuceHeader.h"

class Fonts {
  public:
    virtual ~Fonts() { }

    Font& proportional_regular() { return proportional_regular_; }
    Font& proportional_light() { return proportional_light_; }
    Font& monospace() { return monospace_; }

    static Fonts* instance() {
      static Fonts instance;
      return &instance;
    }

  private:
    Fonts();

    static ScopedPointer<Fonts> instance_;

    Font proportional_regular_;
    Font proportional_light_;
    Font monospace_;
};

#endif // FONTS_H
