#pragma once

#include "kke/appearance/Scale.hh"

namespace kke {
class D2dEngine;
}
namespace kke::example::overlay {
class FullBlurPage {
  public:
	static void render(kke::D2dEngine& engine, kke::Scale viewportSize);
};
} // namespace kke::example::overlay
