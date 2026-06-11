#pragma once

#include <optional>

#include "kke/appearance/resource/effect/EffectCompose.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/engine/d2d/renderer/CommandListSnapshotter.hh"
#include "kke/engine/d2d/renderer/RenderPass.hh"
#include "kke/engine/d2d/renderer/effect/EffectSourceRenderer.hh"
#include "kke/engine/d2d/renderer/effect/EffectPaddingEstimator.hh"
#include "kke/engine/d2d/renderer/effect/renderers/BlurEffectRenderer.hh"
#include "kke/engine/d2d/renderer/effect/renderers/ColorMatrixEffectRenderer.hh"
#include "kke/engine/d2d/renderer/effect/renderers/DirectionalBlurEffectRenderer.hh"
#include "kke/engine/d2d/renderer/effect/renderers/ShadowEffectRenderer.hh"
#include "kke/engine/d2d/renderer/view/ViewLayerController.hh"

namespace kke {
class EffectRenderer {
	EffectSourceRenderer sourceRenderer;
	CommandListSnapshotter commandListSnapshotter;
	BlurEffectRenderer blurEffectRenderer;
	DirectionalBlurEffectRenderer directionalBlurEffectRenderer;
	ColorMatrixEffectRenderer colorMatrixEffectRenderer;
	ShadowEffectRenderer shadowEffectRenderer;

  public:
	void beginDraw();

	void render(
		D2dEngineContext& context,
		RenderPass& renderPass,
		Effect const& effect,
		std::optional<EffectClipSource> clip,
		ViewLayerController& viewLayerController
	);

	void render(
		D2dEngineContext& context,
		RenderPass& renderPass,
		EffectCompose const& effectCompose,
		std::optional<EffectClipSource> clip,
		ViewLayerController& viewLayerController
	);

	void render(
		D2dEngineContext& context,
		EffectSource const& source,
		EffectSourceAppearance const& sourceAppearance,
		Effect const& effect,
		std::optional<EffectClipSource> clip,
		ViewLayerController& viewLayerController
	);

	void render(
		D2dEngineContext& context,
		EffectSource const& source,
		EffectSourceAppearance const& sourceAppearance,
		EffectCompose const& effectCompose,
		std::optional<EffectClipSource> clip,
		ViewLayerController& viewLayerController
	);

  private:
	Microsoft::WRL::ComPtr<ID2D1Image> apply(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage,
		Effect const& effect
	);

	Microsoft::WRL::ComPtr<ID2D1Image> apply(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> sourceImage,
		EffectCompose const& effectCompose
	);

	void renderClipEffect(
		D2dEngineContext& context,
		RenderPass& renderPass,
		Effect const& effect,
		EffectClipSource const& clip,
		ViewLayerController& viewLayerController
	);

	void renderClipEffect(
		D2dEngineContext& context,
		RenderPass& renderPass,
		EffectCompose const& effectCompose,
		EffectClipSource const& clip,
		ViewLayerController& viewLayerController
	);

	D2D1_RECT_F resolveEffectBounds(
		ID2D1Bitmap* renderTarget,
		Effect const& effect,
		EffectClipSource const& clip
	) const;

	D2D1_RECT_F resolveEffectBounds(
		ID2D1Bitmap* renderTarget,
		EffectCompose const& effectCompose,
		EffectClipSource const& clip
	) const;

	void updateClipEffectSnapshot(
		D2dEngineContext const& context,
		ID2D1DeviceContext* deviceContext,
		Microsoft::WRL::ComPtr<ID2D1Bitmap1> snapshotBitmap,
		Microsoft::WRL::ComPtr<ID2D1Image> effectImage,
		Point const& targetOffset,
		EffectClipSource const& clip,
		ViewLayerController& viewLayerController
	);

	void drawImage(
		D2dEngineContext const& context,
		Microsoft::WRL::ComPtr<ID2D1Image> image,
		std::optional<EffectClipSource> const& clip,
		ViewLayerController& viewLayerController
	);

	void drawImage(
		D2dEngineContext const& context,
		Microsoft::WRL::ComPtr<ID2D1Image> image,
		Point const& targetOffset,
		std::optional<EffectClipSource> const& clip,
		ViewLayerController& viewLayerController
	);
};
} // namespace kke
