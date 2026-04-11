#pragma once

#include <string_view>

#include "kke/geometry/Geometry.hh"
#include "kke/geometry/Rect.hh"
#include "kke/geometry/RoundedRect.hh"
#include "kke/geometry/Triangle.hh"
#include "kke/resources/brush/Brush.hh"
#include "kke/resources/font/FontAppearance.hh"

namespace kke {
class FaceRenderer {
public:
	void fillGeometry(
		kke::Geometry const& geometry,
		kke::Brush const& brush);

	void fillLine(
		kke::Point2f const& start,
		kke::Point2f const& end);

	void fillTriangle(
		kke::Triangle const& triangle,
		kke::Brush const& brush);

	void fillRect(
		kke::Rect const& rect,
		kke::Brush const& brush);

	void fillRounded(
		kke::RoundedRect const& rounded,
		kke::Brush const& brush);

	void fillText(
		std::string_view text,
		kke::Point2f const& position,
		kke::Brush const& brush,
		kke::FontAppearance const& appearance);
};
};	// namespace kke
