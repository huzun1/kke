#pragma once

#include <d2d1.h>

#include <string_view>

#include "kke/geometry/Geometry.hh"
#include "kke/geometry/Point.hh"
#include "kke/geometry/Triangle.hh"
#include "kke/renderer/stroke/StrokeStyle.hh"
#include "kke/resources/brush/Brush.hh"
#include "kke/resources/font/FontAppearance.hh"

class EngineInterface {
public:
	virtual void beginDraw() = 0;

	virtual void endDraw() = 0;

	virtual kke::Scale2f getViewportSize() const = 0;

	virtual kke::Scale2f measureTextSize(
		std::string_view text,
		kke::FontAppearance const& appearance) const = 0;

	virtual kke::Scale2f measureTextSize(
		std::wstring_view text,
		kke::FontAppearance const& appearance) const = 0;

	virtual void drawLine(
		kke::Point2f start,
		kke::Point2f end,
		kke::Brush const& brush,
		StrokeStyle const& style) = 0;

	virtual void drawGeometry(
		kke::Geometry const& geometry,
		kke::Brush const& brush,
		StrokeStyle const& style) = 0;

	void drawTriangle(
		kke::Triangle const& triangle,
		kke::Brush const& brush,
		StrokeStyle const& style);
};
