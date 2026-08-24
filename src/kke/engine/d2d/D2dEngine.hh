#pragma once

#include <memory>
#include <optional>

#include "kke/engine/Engine.hh"
#include "kke/engine/d2d/D2dLayerStatistics.hh"

struct ID2D1Bitmap;

namespace kke {
class D2dContext;
class D2dEngine : public Engine {
	struct Impl;

	std::unique_ptr<Impl> impl;

  public:
	D2dEngine();

	~D2dEngine() override;

	/* =============== Render Statement ================ */
	void beginDraw(D2dContext const& context, ID2D1Bitmap* renderTarget);

	void endDraw();

	D2dLayerStatistics const& getLayerStatistics() const;

	void clear() override;

	/*================ Transform Control ================ */
	void pushTransform(TransformSource const& translate) override;

	void popTransform() override;

	/* ================= Layer Control ================== */
	void pushLayer(MaskSource const& mask, LayerMode mode = LayerMode::Normal) override;

	void popLayer() override;

	/* ================= Canvas Control ================= */
	std::shared_ptr<Canvas> createCanvas() override;

	bool beginCanvas(std::shared_ptr<Canvas> canvas) override;

	bool endCanvas() override;

	bool finishCanvas(std::shared_ptr<Canvas> canvas) override;

	void draw(std::shared_ptr<Canvas> canvas, float opacity = 1.0f) override;

	/* ============== Raster Surface Control ============ */
	std::shared_ptr<RasterSurface>
	createRasterSurface(Scale const& logicalSize, float rasterScale) override;

	bool
	beginRasterSurface(std::shared_ptr<RasterSurface> surface, Point const& logicalOrigin) override;

	bool endRasterSurface() override;

	void draw(std::shared_ptr<RasterSurface> surface, Rect const& destination, float opacity = 1.0f)
		override;

	/* ================= Measurement =================== */
	kke::Scale getViewportSize() override;

	kke::Scale measureTextSize(Text const& text) override;

	/* ================= Stroke Rendering ================= */
	void
	draw(StrokeSource const& line, Brush const& brush, StrokeAppearance const& appearance) override;

	/* ================= Face Rendering ================== */
	void fill(FillSource const& source, Brush const& brush) override;

	/* ================= Font Resource ================= */
	std::shared_ptr<Font> uploadFont(void const* data, size_t size) override;

	/* ================ Texture Rendering ================ */
	std::shared_ptr<Texture> uploadTexture(void const* data, size_t size) override;

	std::shared_ptr<Texture> uploadTexture(RawTextureData const& data) override;

	void draw(
		std::shared_ptr<Texture> texture,
		Rect const& destRect,
		TextureDrawAppearance const& appearance = TextureDrawAppearance{}
	) override;

	/* ================= Effect Rendering ================== */
	std::shared_ptr<TargetSnapshot> captureTargetSnapshot() override;

	void renderEffect(
		Effect const& effect,
		std::optional<EffectClipSource> clip = std::nullopt,
		float opacity = 1.0f
	) override;

	std::shared_ptr<Canvas> captureEffect(
		Effect const& effect, std::optional<EffectClipSource> clip = std::nullopt
	) override;

	std::optional<CapturedEffect> captureEffect(
		Effect const& effect, EffectClipSource const& clip, EffectCaptureOptions const& options
	) override;

	std::optional<CapturedEffect> captureEffect(
		std::shared_ptr<TargetSnapshot> const& source,
		Effect const& effect,
		EffectClipSource const& clip,
		EffectCaptureOptions const& options
	) override;

	void renderEffect(
		EffectSource const& source,
		EffectSourceAppearance const& sourceAppearance,
		Effect const& effect,
		std::optional<EffectClipSource> clip = std::nullopt
	) override;

	void renderEffect(
		EffectCompose const& effect, std::optional<EffectClipSource> clip = std::nullopt
	) override;

	void renderEffect(
		EffectSource const& source,
		EffectSourceAppearance const& sourceAppearance,
		EffectCompose const& effect,
		std::optional<EffectClipSource> clip = std::nullopt
	) override;

  private:
	void assertD2dContext() const;
};
} // namespace kke
