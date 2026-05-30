#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include "kke/appearance/Text.hh"
#include "kke/appearance/painting/StrokeAppearance.hh"
#include "kke/appearance/resource/effect/Effect.hh"
#include "kke/appearance/resource/effect/EffectCompose.hh"
#include "kke/appearance/resource/font/Font.hh"
#include "kke/appearance/resource/texture/RawTextureData.hh"
#include "kke/appearance/resource/texture/TextureDrawAppearance.hh"
#include "kke/appearance/resource/texture/Texture.hh"
#include "kke/engine/Engine.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/engine/d2d/renderer/Measurer.hh"
#include "kke/engine/d2d/renderer/RenderPass.hh"
#include "kke/engine/d2d/renderer/canvas/CanvasService.hh"
#include "kke/engine/d2d/renderer/effect/EffectRenderer.hh"
#include "kke/engine/d2d/renderer/painting/FaceRenderer.hh"
#include "kke/engine/d2d/renderer/painting/StrokeRenderer.hh"
#include "kke/engine/d2d/renderer/painting/TextureRenderer.hh"
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
	TextureRenderer textureRenderer;
	EffectRenderer effectRenderer;

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

	/* ================= Font Resource ================= */
	std::shared_ptr<Font> uploadFont(
		void const* data,
		size_t size) override;

	/* ================ Texture Rendering ================ */
	std::shared_ptr<Texture> uploadTexture(
		void const* data,
		size_t size) override;

	std::shared_ptr<Texture> uploadTexture(
		RawTextureData const& data) override;

	void draw(
		std::shared_ptr<Texture> texture,
		Rect const& destRect,
		TextureDrawAppearance const& appearance = TextureDrawAppearance{}) override;

	/* ================= Effect Rendering ================== */
	void renderEffect(
		Effect const& effect,
		std::optional<EffectClipSource> clip = std::nullopt) override;

	void renderEffect(
		EffectSource const& source,
		EffectSourceAppearance const& sourceAppearance,
		Effect const& effect,
		std::optional<EffectClipSource> clip = std::nullopt) override;

	void renderEffect(
		EffectCompose const& effect,
		std::optional<EffectClipSource> clip = std::nullopt) override;

	void renderEffect(
		EffectSource const& source,
		EffectSourceAppearance const& sourceAppearance,
		EffectCompose const& effect,
		std::optional<EffectClipSource> clip = std::nullopt) override;

private:
	void assertD2dContext() const;
};
}	// namespace kke
