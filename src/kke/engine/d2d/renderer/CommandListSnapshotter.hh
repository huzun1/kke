#pragma once

#include <vector>

#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
enum class SnapshotOpacityMode {
	PreserveAlpha,
	FlattenToOpaqueBlack
};

class CommandListSnapshotter {
	struct SnapshotBitmapCacheEntry {
		Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;
		D2D1_SIZE_U pixelSize;
		D2D1_PIXEL_FORMAT pixelFormat;
		float dpiX;
		float dpiY;
		bool isInUseThisFrame = false;
	};

	Microsoft::WRL::ComPtr<ID2D1Device> snapshotDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> snapshotDeviceContext;
	std::vector<SnapshotBitmapCacheEntry> snapshotBitmapCache;
	size_t snapshotBitmapCacheIndex = 0;

  public:
	void beginFrame();

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> snapshot(
		ID2D1DeviceContext* deviceContext,
		ID2D1Image* source,
		ID2D1Bitmap* referenceTarget,
		SnapshotOpacityMode opacityMode = SnapshotOpacityMode::PreserveAlpha
	);

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> snapshotComposite(
		ID2D1DeviceContext* deviceContext,
		ID2D1Image* background,
		ID2D1Image* foreground,
		ID2D1Bitmap* referenceTarget,
		SnapshotOpacityMode opacityMode = SnapshotOpacityMode::PreserveAlpha
	);

  private:
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> acquireSnapshotBitmap(
		ID2D1DeviceContext* deviceContext,
		D2D1_SIZE_U pixelSize,
		D2D1_PIXEL_FORMAT pixelFormat,
		float dpiX,
		float dpiY
	);

	ID2D1DeviceContext* acquireSnapshotDeviceContext(ID2D1DeviceContext* deviceContext);

	static void clearSnapshotTarget(
		ID2D1DeviceContext* deviceContext,
		SnapshotOpacityMode opacityMode
	);
};
}; // namespace kke
