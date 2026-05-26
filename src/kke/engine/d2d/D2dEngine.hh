#pragma once

#include <memory>
#include <optional>

#include "kke/appearance/Text.hh"
#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/resource/effect/Effect.hh"
#include "kke/appearance/resource/effect/EffectCompose.hh"
#include "kke/appearance/resource/texture/Texture.hh"
#include "kke/engine/Engine.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/engine/d2d/renderer/Measurer.hh"
#include "kke/engine/d2d/renderer/RenderPass.hh"
#include "kke/engine/d2d/renderer/canvas/CanvasService.hh"
#include "kke/engine/d2d/renderer/painting/FaceRenderer.hh"
#include "kke/engine/d2d/renderer/painting/StrokeRenderer.hh"
#include "kke/engine/d2d/renderer/view/MatrixState.hh"
#include "kke/engine/d2d/renderer/view/ViewLayerController.hh"

namespace kke {
class D2dEngine : public Engine {
	std::unique_ptr<D2dEngineContext> engineContext;

	RenderPass renderPass;
	MatrixState matrixState;
	ViewLayerController viewLayerController;
	Measurer measurer;
	CanvasService canvasService;
	FaceRenderer faceRenderer;
	StrokeRenderer strokeRenderer;

public:
	D2dEngine();

	/* =============== Render Statement ================ */
	void beginDraw(D2dContext const& context, ID2D1Bitmap* renderTarget);

	void endDraw();

	void clear() override;

	/*================ Transform Control ================ */
	void pushTransform(TransformSource const& translate) override;

	void popTransform() override;

	/* ================= Layer Control ================== */
	void pushLayer(MaskSource const& mask, LayerMode mode = LayerMode::Normal) override;

	void popLayer() override;

	/* ================= Canvas Control ================= */
	std::shared_ptr<Canvas> createCanvas() override;

	void pushCanvas(std::shared_ptr<Canvas> canvas) override;

	void popCanvas() override;

	void draw(std::shared_ptr<Canvas> canvas, float opacity = 1.0f) override;

	/* ================= Measurement =================== */
	kke::Scale getViewportSize() override;

	kke::Scale measureTextSize(
		Text const& text) override;

	/* ================= Stroke Rendering ================= */
	void draw(
		StrokeSource const& line,
		Brush const& brush,
		StrokeAppearance const& appearance) override;

	/* ================= Face Rendering ================== */
	void fill(
		FillSource const& source,
		Brush const& brush) override;

	/* ================ Texture Rendering ================ */
	std::shared_ptr<Texture> uploadTexture(
		void const* data,
		size_t size) override;

	void draw(
		std::shared_ptr<Texture> texture,
		Rect const& destRect,
		float opacity = 1.0f,
		std::optional<Rect> srcRect = std::nullopt) override;

	/* ================= Effect Rendering ================== */
	void renderEffect(
		Effect const& effect) override;

	void renderEffect(
		EffectSource const& source,
		Effect const& effect) override;

	void renderEffect(
		EffectCompose const& effect) override;

	void renderEffect(
		EffectSource const& source,
		EffectCompose const& effect) override;

private:
	void assertD2dContext() const;
};
};	// namespace kke
