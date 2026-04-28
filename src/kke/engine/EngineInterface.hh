#pragma once

#include <memory>
#include <optional>

#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/effect/EffectCompose.hh"
#include "kke/appearance/resource/texture/Texture.hh"
#include "kke/appearance/view/LayerMode.hh"
#include "kke/engine/Sources.hh"

namespace kke {
class EngineInterface {
public:
	virtual ~EngineInterface() = default;

	/* =============== Render Statement ================ */
	virtual void clear() = 0;

	/*================ Transform Control ================ */
	virtual void pushTransform(TransformSource const& transform) = 0;

	virtual void popTransform() = 0;

	/* ================= Layer Control ================== */
	virtual void pushLayer(MaskSource const& mask, LayerMode mode = LayerMode::NORMAL) = 0;

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
		StrokeSource const& source,
		Brush const& brush,
		StrokeAppearance const& appearance) = 0;

	/* ================= Fill Rendering ================= */
	virtual void fill(
		FillSource const& source,
		Brush const& brush) = 0;

	/* ================ Texture Rendering ================ */
	virtual std::shared_ptr<Texture> uploadTexture(
		void const* data, size_t size) = 0;

	virtual void draw(
		std::shared_ptr<Texture> texture,
		Rect const& destRect,
		float opacity = 1.0f,
		std::optional<Rect> srcRect = std::nullopt) = 0;

	/* ================= Effect Rendering ================= */
	virtual void renderEffect(
		EffectSource const& source,
		Effect const& effect) = 0;

	virtual void renderEffect(
		EffectSource const& source,
		EffectCompose const& effect) = 0;
};
};	// namespace kke
