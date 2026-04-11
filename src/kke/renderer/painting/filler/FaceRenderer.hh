#pragma once

#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"
#include "kke/resources/brush/Brush.hh"

namespace kke {
class FaceRenderer {
public:
	void fill(
		kke::Triangle const& triangle,
		kke::Brush const& brush);

	void fill(
		kke::Rect const& rect,
		kke::Brush const& brush);

	void fill(
		kke::RoundedRect const& rounded,
		kke::Brush const& brush);
};
};	// namespace kke
