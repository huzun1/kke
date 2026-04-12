#pragma once

#include "kke/appearance/effect/blur/BlurAppearance.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/primitives/Line.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"

namespace kke {
class BlurRenderer {
public:
	void renderLineBlur(Line const& line, float thick, BlurAppearance const& appearance);

	void renderTriangleBlur(Triangle const& triangle, BlurAppearance const& appearance);

	void renderRectBlur(Rect const& rect, BlurAppearance const& appearance);

	void renderRoundedBlur(RoundedRect const& rounded, BlurAppearance const& appearance);
};
};	// namespace kke
