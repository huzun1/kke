#pragma once

#include <memory>
#include <optional>
#include <string_view>

#include "kke/appearance/brush/Brush.hh"
#include "kke/appearance/canvas/Canvas.hh"
#include "kke/appearance/effect/Effect.hh"
#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/text/Text.hh"
#include "kke/appearance/transform/Rotate.hh"
#include "kke/appearance/transform/Scale.hh"
#include "kke/appearance/transform/Translate.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/primitives/Line.hh"
#include "kke/geometry/primitives/Point.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"

namespace kke {
class EngineInterface {
public:
	/* =============== Render Statement ================ */
	virtual void beginDraw() = 0;

	virtual void endDraw() = 0;

	/* ================= State Control ================== */
	virtual void pushTranslate(Translate const& translate) = 0;

	virtual void pushScale(Scale const& scale) = 0;

	virtual void pushRotate(Rotate const& rotate) = 0;

	virtual void popTranslate() = 0;

	virtual void popScale() = 0;

	virtual void popRotate() = 0;

	/* ================= Canvas Control ================= */
	virtual std::shared_ptr<Canvas> createCanvas(std::optional<Scale2f> scale = std::nullopt) = 0;

	virtual void pushCanvas(std::shared_ptr<Canvas> canvas) = 0;

	virtual void popCanvas() = 0;

	virtual void draw(std::shared_ptr<Canvas> canvas) = 0;

	/* ================= Measurement =================== */
	virtual Scale2f getViewportSize() = 0;

	virtual Scale2f measureTextSize(
		std::string_view text,
		FontAppearance const& fontAppearance) = 0;

	virtual Scale2f measureTextSize(
		std::wstring_view text,
		FontAppearance const& fontAppearance) = 0;

	/* ================= Stroke Rendering ================= */
	virtual void draw(
		Line const& line,
		Brush const& brush,
		StrokeAppearance const& appearance) = 0;

	virtual void draw(
		Triangle const& triangle,
		Brush const& brush,
		StrokeAppearance const& appearance) = 0;

	virtual void draw(
		Rect const& rect,
		Brush const& brush,
		StrokeAppearance const& appearance) = 0;

	virtual void draw(
		RoundedRect const& roundedRect,
		Brush const& brush,
		StrokeAppearance const& appearance) = 0;

	virtual void draw(
		Ellipse const& ellipse,
		Brush const& brush,
		StrokeAppearance const& appearance) = 0;

	/* ================= Fill Rendering ================= */
	virtual void fill(
		Triangle const& triangle,
		Brush const& brush) = 0;

	virtual void fill(
		Rect const& rect,
		Brush const& brush) = 0;

	virtual void fill(
		RoundedRect const& roundedRect,
		Brush const& brush) = 0;

	virtual void fill(
		Ellipse const& ellipse,
		Brush const& brush) = 0;

	virtual void fill(
		Text const& text,
		Brush const& brush) = 0;

	/* ================= Effect Rendering ================= */
	virtual void renderEffect(
		Line const& line,
		Effect const& effect) = 0;

	virtual void renderEffect(
		Triangle const& triangle,
		Effect const& effect) = 0;

	virtual void renderEffect(
		Rect const& rect,
		Effect const& effect) = 0;

	virtual void renderEffect(
		RoundedRect const& roundedRect,
		Effect const& effect) = 0;

	virtual void renderEffect(
		Ellipse const& ellipse,
		Effect const& effect) = 0;

	virtual void renderEffect(
		Text const& text,
		Effect const& effect) = 0;

	virtual void renderEffect(
		std::shared_ptr<Canvas> canvas,
		Effect const& effect);
};
};	// namespace kke
