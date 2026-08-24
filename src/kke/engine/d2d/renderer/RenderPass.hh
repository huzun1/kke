#pragma once

#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/engine/d2d/d2d1_headers.hh"
#include "kke/engine/d2d/renderer/CommandListSnapshotter.hh"

namespace kke {
class RenderPass {
	CommandListSnapshotter commandListSnapshotter;
	Microsoft::WRL::ComPtr<ID2D1Bitmap> preservedBaseBitmap;
	Microsoft::WRL::ComPtr<ID2D1Device> preservedBaseDevice;
	Microsoft::WRL::ComPtr<ID2D1Image> cachedTargetSnapshot;
	ID2D1Bitmap* lastRenderTarget = nullptr;
	bool shouldPreserveRenderTarget = true;
	bool shouldFlattenNextTargetSnapshot = true;
	bool isPreservedBaseBitmapReady = false;

  public:
	void beginDraw(D2dEngineContext& context, ID2D1Bitmap* renderTarget);

	void endDraw(D2dEngineContext& context);

	void clear(D2dEngineContext& context);

	Microsoft::WRL::ComPtr<ID2D1Image> cycleTargetSnapshot(
		D2dEngineContext& context,
		SnapshotOpacityMode opacityMode = SnapshotOpacityMode::PreserveAlpha
	);

	void invalidateCachedTargetSnapshot();

  private:
	Microsoft::WRL::ComPtr<ID2D1Bitmap> acquirePreservedBaseBitmap(ID2D1DeviceContext* deviceContext
	);

	bool
	isPreservedBaseBitmapCompatible(ID2D1DeviceContext* deviceContext, ID2D1Bitmap* source) const;

	Microsoft::WRL::ComPtr<ID2D1Bitmap>
	createPreservedBaseBitmap(ID2D1DeviceContext* deviceContext, ID2D1Bitmap* source);
};
}; // namespace kke
