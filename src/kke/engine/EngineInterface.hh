#pragma once

#include <memory>
#include <optional>

#include "kke/appearance/Text.hh"
#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/resource/Canvas.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/effect/Effect.hh"
#include "kke/appearance/resource/effect/EffectCompose.hh"
#include "kke/appearance/resource/texture/Texture.hh"
#include "kke/appearance/transform/Rotation.hh"
#include "kke/appearance/transform/Scaling.hh"
#include "kke/appearance/transform/Translation.hh"
#include "kke/appearance/view/LayerMode.hh"
#include "kke/geometry/Maskable.hh"
#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/primitives/Line.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"

namespace kke {
class EngineInterface {
public:
	/* =============== Render Statement ================ */
	virtual void clear() = 0;

	/*================ Transform Control ================ */
	virtual void pushTransform(Translation const& translate) = 0;

	virtual void pushTransform(Scaling const& scale) = 0;

	virtual void pushTransform(Rotation const& rotate) = 0;

	virtual void popTransform() = 0;

	/* ================= Layer Control ================== */
	virtual void pushLayer(Maskable const& mask, LayerMode mode = LayerMode::NORMAL) = 0;

	virtual void popLayer() = 0;

	/* ================= Canvas Control ================= */
	virtual std::shared_ptr<Canvas> createCanvas() = 0;

	virtual void pushCanvas(std::shared_ptr<Canvas> canvas) = 0;

	virtual void popCanvas() = 0;

	virtual void draw(
		std::shared_ptr<Canvas> canvas,
		float opacity = 1.0f) = 0;

	/* ================= Measurement =================== */
	virtual Scale getViewportSize() = 0;

	virtual Scale measureTextSize(Text const& text) = 0;

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

	/* ================ Texture Rendering ================ */
	virtual std::shared_ptr<Texture> uploadTexture(
		void const* data, size_t size) = 0;

	virtual void discardTexture(
		std::shared_ptr<Texture> texture) = 0;

	virtual void draw(
		std::shared_ptr<Texture> texture,
		Rect const& destRect,
		float opacity = 1.0f,
		std::optional<Rect> srcRect = std::nullopt) = 0;

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

	/* ================ Effect Compose Rendering ================ */
	virtual void renderEffect(
		Line const& line,
		EffectCompose const& effect) = 0;

	virtual void renderEffect(
		Triangle const& triangle,
		EffectCompose const& effect) = 0;

	virtual void renderEffect(
		Rect const& rect,
		EffectCompose const& effect) = 0;

	virtual void renderEffect(
		RoundedRect const& roundedRect,
		EffectCompose const& effect) = 0;

	virtual void renderEffect(
		Text const& text,
		EffectCompose const& effect) = 0;

	virtual void renderEffect(
		std::shared_ptr<Canvas> canvas,
		EffectCompose const& effect) = 0;
};
};	// namespace kke
