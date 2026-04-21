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
	void beginDraw();

	void endDraw();

	void clear() override;

	/*================ Transform Control ================ */
	void pushTransform(Translation const& translate) override;

	void pushTransform(Scaling const& scale) override;

	void pushTransform(Rotation const& rotate) override;

	void popTransform() override;

	/* ================= Layer Control ================== */
	void pushLayer(Maskable const& mask, LayerMode mode = LayerMode::NORMAL) override;

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
		Line const& line,
		Brush const& brush,
		StrokeAppearance const& appearance) override;

	void draw(
		Triangle const& triangle,
		Brush const& brush,
		StrokeAppearance const& appearance) override;

	void draw(
		Rect const& rect,
		Brush const& brush,
		StrokeAppearance const& appearance) override;

	void draw(
		RoundedRect const& roundedRect,
		Brush const& brush,
		StrokeAppearance const& appearance) override;

	/* ================= Face Rendering ================== */
	void fill(
		Triangle const& triangle,
		Brush const& brush) override;

	void fill(
		Rect const& rect,
		Brush const& brush) override;

	void fill(
		RoundedRect const& roundedRect,
		Brush const& brush) override;

	void fill(
		Text const& text,
		Brush const& brush) override;

	/* ================= Effect Rendering ================== */
	void renderEffect(
		Line const& line,
		Effect const& effect) override;

	void renderEffect(
		Triangle const& triangle,
		Effect const& effect) override;

	void renderEffect(
		Rect const& rect,
		Effect const& effect) override;

	void renderEffect(
		RoundedRect const& roundedRect,
		Effect const& effect) override;

	void renderEffect(
		Text const& text,
		Effect const& effect) override;

	void renderEffect(
		std::shared_ptr<Canvas> canvas,
		Effect const& effect) override;

	/* ================ Effect Compose Rendering ================ */
	void renderEffect(
		Line const& line,
		EffectCompose const& effect) override;

	void renderEffect(
		Triangle const& triangle,
		EffectCompose const& effect) override;

	void renderEffect(
		Rect const& rect,
		EffectCompose const& effect) override;

	void renderEffect(
		RoundedRect const& roundedRect,
		EffectCompose const& effect) override;

	void renderEffect(
		Text const& text,
		EffectCompose const& effect) override;

	void renderEffect(
		std::shared_ptr<Canvas> canvas,
		EffectCompose const& effect) override;
};
};	// namespace kke
