#pragma once

#include "BlurAppearance.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/primitives/Line.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"

namespace kke {
class BlurRenderer {
public:
	void renderBlur(Line const& line, float thick, BlurAppearance const& appearance);

	void renderBlur(Triangle const& triangle, BlurAppearance const& appearance);

	void renderBlur(Rect const& rect, BlurAppearance const& appearance);

	void renderBlur(RoundedRect const& rounded, BlurAppearance const& appearance);
};
};	// namespace kke
