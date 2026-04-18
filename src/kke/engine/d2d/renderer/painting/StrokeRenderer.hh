#pragma once

#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/primitives/Line.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"

namespace kke {
class StrokeRenderer {
public:
	void draw(
		Line const& line,
		StrokeAppearance const& appearance);

	void draw(
		Triangle const& triangle,
		Brush const& brush,
		StrokeAppearance const& appearance);

	void draw(
		Rect const& rect,
		Brush const& brush,
		StrokeAppearance const& appearance);

	void draw(
		RoundedRect const& rounded,
		Brush const& brush,
		StrokeAppearance const& appearance);
};
};	// namespace kke
