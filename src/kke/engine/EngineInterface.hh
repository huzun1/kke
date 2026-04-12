#pragma once

#include <string_view>

#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/primitives/Line.hh"
#include "kke/geometry/primitives/Point.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"
#include "kke/appearance/font/FontAppearance.hh"
#include "kke/appearance/brush/Brush.hh"
#include "kke/appearance/effect/shadow/ShadowAppearance.hh"

namespace kke {
class EngineInterface {
public:
	/* =============== Render Statement ================ */
	virtual void beginDraw() = 0;

	virtual void endDraw() = 0;

	/* ================= State Control ================== */
	virtual void pushTranslate(Point2f const& offset) = 0;

	virtual void pushScale(Point2f const& center, Scale2f const& scale) = 0;

	virtual void pushRotate(Point2f const& center, float angle) = 0;

	virtual void popTranslate() = 0;

	virtual void popScale() = 0;

	virtual void popRotate() = 0;

	/* ================= Measurement =================== */
	virtual Scale2f getViewportSize() = 0;

	virtual Scale2f measureTextSize(
		std::string_view text,
		FontAppearance const& fontAppearance) = 0;

	virtual Scale2f measureTextSize(
		std::wstring_view text,
		FontAppearance const& fontAppearance) = 0;

	/* ================= Stroke Rendering ================= */
	virtual void drawLine(
		Line const& line,
		Brush const& brush,
		FontAppearance const& style) = 0;

	virtual void drawTriangle(
		Triangle const& triangle,
		Brush const& brush,
		FontAppearance const& style) = 0;

	virtual void drawRect(
		Rect const& rect,
		Brush const& brush,
		FontAppearance const& style) = 0;

	virtual void drawRounded(
		RoundedRect const& roundedRect,
		Brush const& brush,
		FontAppearance const& style) = 0;

	/* ================= Fill Rendering ================= */
	virtual void fillTriangle(
		Triangle const& triangle,
		Brush const& brush) = 0;

	virtual void fillRect(
		Rect const& rect,
		Brush const& brush) = 0;

	virtual void fillRounded(
		RoundedRect const& roundedRect,
		Brush const& brush) = 0;

	virtual void fillText(
		std::string_view text,
		Point2f const& position,
		Brush const& brush,
		FontAppearance const& appearance) = 0;

	virtual void fillText(
		std::wstring_view text,
		Point2f const& position,
		Brush const& brush,
		FontAppearance const& appearance) = 0;

	/* ================= Shadow Rendering ================= */
	virtual void renderLineShadow(
		Line const& line,
		Brush const& brush,
		ShadowAppearance const& appearance) = 0;

	virtual void renderTriangleShadow(
		Triangle const& triangle,
		Brush const& brush,
		ShadowAppearance const& appearance) = 0;

	virtual void renderRectShadow(
		Rect const& rect,
		Brush const& brush,
		ShadowAppearance const& appearance) = 0;

	virtual void renderRoundedShadow(
		RoundedRect const& roundedRect,
		Brush const& brush,
		ShadowAppearance const& appearance) = 0;

	virtual void renderTextShadow(
		std::string_view text,
		Point2f const& position,
		Brush const& brush,
		FontAppearance const& fontAppearance,
		ShadowAppearance const& shadowAppearance) = 0;

	virtual void renderTextShadow(
		std::wstring_view text,
		Point2f const& position,
		Brush const& brush,
		FontAppearance const& fontAppearance,
		ShadowAppearance const& shadowAppearance) = 0;
};
};	// namespace kke
