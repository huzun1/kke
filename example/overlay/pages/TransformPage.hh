#pragma once

#include <cstdint>

#include "kke/appearance/Scale.hh"

namespace kke {
class D2dEngine;
}
namespace kke::example::overlay {
class TransformPage {
	uint64_t animationStartTime = 0;

  public:
	TransformPage();

	void render(kke::D2dEngine& engine, kke::Scale viewportSize) const;
};
} // namespace kke::example::overlay
