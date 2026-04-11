#pragma once

#include "StrokeStyle.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/Rect.hh"
#include "kke/geometry/RoundedRect.hh"
#include "kke/geometry/Triangle.hh"
#include "kke/resources/brush/Brush.hh"

namespace kke {
class StrokeRenderer {
public:
	void drawGeometry(
		kke::Geometry const& geometry,
		kke::Brush const& brush,
		StrokeStyle const& style);

	void drawLine(
		kke::Point2f const& start,
		kke::Point2f const& end,
		StrokeStyle const& style);

	void drawTriangle(
		kke::Triangle const& triangle,
		kke::Brush const& brush,
		StrokeStyle const& style);

	void drawRect(
		kke::Rect const& rect,
		kke::Brush const& brush,
		StrokeStyle const& style);

	void drawRounded(
		kke::RoundedRect const& rounded,
		kke::Brush const& brush,
		StrokeStyle const& style);
};

};	// namespace kke
