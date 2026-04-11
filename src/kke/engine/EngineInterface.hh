#pragma once

#include <string_view>

#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/primitives/Line.hh"
#include "kke/geometry/primitives/Point.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"
#include "kke/renderer/effect/shadow/ShadowAppearance.hh"
#include "kke/renderer/painting/stroke/StrokeStyle.hh"
#include "kke/resources/brush/Brush.hh"
#include "kke/resources/font/FontAppearance.hh"

namespace kke {

class EngineInterface {
public:
	/* =============== Render Statement ================ */
	virtual void beginDraw() = 0;

	virtual void endDraw() = 0;

	/* ================= State Control ================== */
	virtual void pushTranslate(kke::Point2f const& offset) = 0;

	virtual void pushScale(kke::Point2f const& center, kke::Scale2f const& scale) = 0;

	virtual void pushRotate(kke::Point2f const& center, float angle) = 0;

	virtual void popTranslate() = 0;

	virtual void popScale() = 0;

	virtual void popRotate() = 0;

	/* ================= Measurement =================== */
	virtual kke::Scale2f getViewportSize() = 0;

	virtual kke::Scale2f measureTextSize(
		std::string_view text,
		kke::FontAppearance const& appearance) = 0;

	virtual kke::Scale2f measureTextSize(
		std::wstring_view text,
		kke::FontAppearance const& appearance) = 0;

	/* ================= Stroke Rendering ================= */
	virtual void drawLine(
		kke::Line const& line,
		kke::Brush const& brush,
		StrokeStyle const& style) = 0;

	virtual void drawTriangle(
		kke::Triangle const& triangle,
		kke::Brush const& brush,
		StrokeStyle const& style) = 0;

	virtual void drawRect(
		kke::Rect const& rect,
		kke::Brush const& brush,
		StrokeStyle const& style) = 0;

	virtual void drawRounded(
		kke::RoundedRect const& roundedRect,
		kke::Brush const& brush,
		StrokeStyle const& style) = 0;

	/* ================= Fill Rendering ================= */
	virtual void fillTriangle(
		kke::Triangle const& triangle,
		kke::Brush const& brush) = 0;

	virtual void fillRect(
		kke::Rect const& rect,
		kke::Brush const& brush) = 0;

	virtual void fillRounded(
		kke::RoundedRect const& roundedRect,
		kke::Brush const& brush) = 0;

	virtual void fillText(
		std::string_view text,
		kke::Point2f const& position,
		kke::Brush const& brush,
		kke::FontAppearance const& appearance) = 0;

	virtual void fillText(
		std::wstring_view text,
		kke::Point2f const& position,
		kke::Brush const& brush,
		kke::FontAppearance const& appearance) = 0;

	/* ================= Shadow Rendering ================= */
	virtual void renderLineShadow(
		kke::Line const& line,
		kke::Brush const& brush,
		kke::ShadowAppearance const& appearance) = 0;

	virtual void renderTriangleShadow(
		kke::Triangle const& triangle,
		kke::Brush const& brush,
		kke::ShadowAppearance const& appearance) = 0;

	virtual void renderRectShadow(
		kke::Rect const& rect,
		kke::Brush const& brush,
		kke::ShadowAppearance const& appearance) = 0;

	virtual void renderRoundedShadow(
		kke::RoundedRect const& roundedRect,
		kke::Brush const& brush,
		kke::ShadowAppearance const& appearance) = 0;

	virtual void renderTextShadow(
		std::string_view text,
		kke::Point2f const& position,
		kke::Brush const& brush,
		kke::FontAppearance const& fontAppearance,
		kke::ShadowAppearance const& shadowAppearance) = 0;

	virtual void renderTextShadow(
		std::wstring_view text,
		kke::Point2f const& position,
		kke::Brush const& brush,
		kke::FontAppearance const& fontAppearance,
		kke::ShadowAppearance const& shadowAppearance) = 0;
};
};	// namespace kke
