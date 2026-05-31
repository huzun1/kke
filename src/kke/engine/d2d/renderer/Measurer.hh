#pragma once

#include "kke/appearance/Scale.hh"
#include "kke/appearance/Text.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"

namespace kke {
class Measurer {
  public:
	Scale getViewportSize(D2dEngineContext const& context);

	Scale measureTextSize(D2dEngineContext const& context, Text const& text);
};
} // namespace kke
