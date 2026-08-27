#pragma once

#include <cstdint>

#include "kke/appearance/Scale.hh"

namespace kke {
class D2dEngine;
}
namespace kke::example::overlay {
class ShadowAndClippingPage {
	uint64_t animationStartTime = 0;

  public:
	ShadowAndClippingPage();

	void render(kke::D2dEngine& engine, kke::Scale viewportSize) const;
};
} // namespace kke::example::overlay
