#pragma once

#include <memory>
#include <optional>

#include "kke/appearance/brush/Brush.hh"
#include "kke/appearance/Canvas.hh"
#include "kke/appearance/effect/Effect.hh"
#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/text/Text.hh"
#include "kke/appearance/transform/Rotate.hh"
#include "kke/appearance/transform/Scale.hh"
#include "kke/appearance/transform/Translate.hh"
#include "kke/appearance/view/LayerMode.hh"
#include "kke/geometry/Polygon.hh"
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
	virtual void clear() = 0;

	/*================ Transform Control ================ */
	virtual void pushTransform(Translate const& translate) = 0;

	virtual void pushTransform(Scale const& scale) = 0;

	virtual void pushTransform(Rotate const& rotate) = 0;

	virtual void popTransform() = 0;

	/* ================= Layer Control ================== */
	virtual void pushLayer(Polygon const& mask, LayerMode mode = LayerMode::NORMAL) = 0;

	virtual void popLayer() = 0;

	/* ================= Canvas Control ================= */
	virtual std::shared_ptr<Canvas> createCanvas(std::optional<Scale2f> scale = std::nullopt) = 0;

	virtual void pushCanvas(std::shared_ptr<Canvas> canvas) = 0;

	virtual void popCanvas() = 0;

	virtual void draw(std::shared_ptr<Canvas> canvas) = 0;

	/* ================= Measurement =================== */
	virtual Scale2f getViewportSize() = 0;

	virtual Scale2f measureTextSize(Text const& text) = 0;

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
		Effect const& effect) = 0;
};
};	// namespace kke
