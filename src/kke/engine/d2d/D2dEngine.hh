#pragma once

#include <memory>

#include "kke/appearance/Text.hh"
#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/resource/effect/Effect.hh"
#include "kke/appearance/resource/effect/EffectCompose.hh"
#include "kke/engine/EngineInterface.hh"
#include "kke/engine/d2d/D2dContext.hh"
#include "kke/engine/d2d/renderer/RenderPass.hh"
#include "kke/engine/d2d/renderer/painting/FaceRenderer.hh"
#include "kke/engine/d2d/renderer/painting/StrokeRenderer.hh"
#include "kke/engine/d2d/renderer/view/MatrixState.hh"
#include "kke/engine/d2d/renderer/view/ViewLayerController.hh"
#include "kke/engine/d2d/renderer/canvas/CanvasService.hh"

namespace kke {
class D2dEngine : public EngineInterface {
	std::unique_ptr<D2dContext> context;

	RenderPass renderPass;
	MatrixState matrixState;
	ViewLayerController viewLayerController;
	CanvasService canvasService;
	FaceRenderer faceRenderer;
	StrokeRenderer strokeRenderer;

public:
	/* =============== Render Statement ================ */
	void beginDraw(D2dContext const& context, ID2D1Image* renderTarget);

	void endDraw();

	void clear() override;

	/*================ Transform Control ================ */
	void pushTransform(TransformSource const& translate) override;

	void popTransform() override;

	/* ================= Layer Control ================== */
	void pushLayer(MaskSource const& mask, LayerMode mode = LayerMode::NORMAL) override;

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

	/* ================= Effect Rendering ================== */
	void renderEffect(
		EffectSource const& source,
		Effect const& effect) override;

	void renderEffect(
		EffectSource const& source,
		EffectCompose const& effect) override;
};
};	// namespace kke
