#include "CommandListSnapshotter.hh"

#include <algorithm>
#include <cmath>

using namespace kke;

void CommandListSnapshotter::beginFrame() {
	snapshotBitmapCacheIndex = 0;
}

Microsoft::WRL::ComPtr<ID2D1Bitmap1> CommandListSnapshotter::snapshot(
	ID2D1DeviceContext* deviceContext, ID2D1Image* source, ID2D1Bitmap* referenceTarget
) {
	if (!deviceContext || !source || !referenceTarget) {
		return nullptr;
	}

	ID2D1DeviceContext* snapshotContext = acquireSnapshotDeviceContext(deviceContext);
	if (!snapshotContext) {
		return nullptr;
	}

	float dpiX, dpiY;
	referenceTarget->GetDpi(&dpiX, &dpiY);

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> snapshotBitmap = acquireSnapshotBitmap(
		snapshotContext,
		referenceTarget->GetPixelSize(),
		referenceTarget->GetPixelFormat(),
		dpiX,
		dpiY
	);
	if (!snapshotBitmap) {
		return nullptr;
	}

	Microsoft::WRL::ComPtr<ID2D1Image> previousTarget;
	snapshotContext->GetTarget(&previousTarget);

	D2D1_MATRIX_3X2_F previousTransform;
	snapshotContext->GetTransform(&previousTransform);

	snapshotContext->BeginDraw();
	snapshotContext->SetTarget(snapshotBitmap.Get());
	snapshotContext->SetTransform(D2D1::Matrix3x2F::Identity());
	snapshotContext->Clear();
	snapshotContext->DrawImage(source);

	HRESULT drawResult = snapshotContext->EndDraw();

	snapshotContext->SetTransform(previousTransform);
	snapshotContext->SetTarget(previousTarget.Get());

	if (FAILED(drawResult)) {
		return nullptr;
	}

	return snapshotBitmap;
}

Microsoft::WRL::ComPtr<ID2D1Bitmap1> CommandListSnapshotter::snapshotRegion(
	ID2D1DeviceContext* deviceContext,
	ID2D1Image* source,
	ID2D1Bitmap* referenceTarget,
	D2D1_RECT_F const& sourceBounds
) {
	if (!deviceContext || !source || !referenceTarget) {
		return nullptr;
	}

	ID2D1DeviceContext* snapshotContext = acquireSnapshotDeviceContext(deviceContext);
	if (!snapshotContext) {
		return nullptr;
	}

	float width = std::max(1.0f, std::ceil(sourceBounds.right - sourceBounds.left));
	float height = std::max(1.0f, std::ceil(sourceBounds.bottom - sourceBounds.top));

	float dpiX, dpiY;
	referenceTarget->GetDpi(&dpiX, &dpiY);

	D2D1_SIZE_U pixelSize{
		static_cast<UINT32>(width),
		static_cast<UINT32>(height)
	};

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> snapshotBitmap = acquireSnapshotBitmap(
		snapshotContext,
		pixelSize,
		referenceTarget->GetPixelFormat(),
		dpiX,
		dpiY
	);
	if (!snapshotBitmap) {
		return nullptr;
	}

	Microsoft::WRL::ComPtr<ID2D1Image> previousTarget;
	snapshotContext->GetTarget(&previousTarget);

	D2D1_MATRIX_3X2_F previousTransform;
	snapshotContext->GetTransform(&previousTransform);

	snapshotContext->BeginDraw();
	snapshotContext->SetTarget(snapshotBitmap.Get());
	snapshotContext->SetTransform(D2D1::Matrix3x2F::Identity());
	snapshotContext->Clear();
	snapshotContext->DrawImage(source, {-sourceBounds.left, -sourceBounds.top});

	HRESULT drawResult = snapshotContext->EndDraw();

	snapshotContext->SetTransform(previousTransform);
	snapshotContext->SetTarget(previousTarget.Get());

	if (FAILED(drawResult)) {
		return nullptr;
	}

	return snapshotBitmap;
}

Microsoft::WRL::ComPtr<ID2D1Bitmap1> CommandListSnapshotter::acquireSnapshotBitmap(
	ID2D1DeviceContext* deviceContext,
	D2D1_SIZE_U pixelSize,
	D2D1_PIXEL_FORMAT pixelFormat,
	float dpiX,
	float dpiY
) {
	if (!deviceContext) {
		return nullptr;
	}

	if (snapshotBitmapCacheIndex < snapshotBitmapCache.size()) {
		SnapshotBitmapCacheEntry& cachedEntry = snapshotBitmapCache[snapshotBitmapCacheIndex];
		if (
			cachedEntry.bitmap &&
			cachedEntry.pixelSize.width == pixelSize.width &&
			cachedEntry.pixelSize.height == pixelSize.height &&
			cachedEntry.pixelFormat.format == pixelFormat.format &&
			cachedEntry.pixelFormat.alphaMode == pixelFormat.alphaMode &&
			cachedEntry.dpiX == dpiX &&
			cachedEntry.dpiY == dpiY
		) {
			snapshotBitmapCacheIndex++;
			return cachedEntry.bitmap;
		}
	}

	D2D1_BITMAP_PROPERTIES1 properties =
		D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, pixelFormat, dpiX, dpiY);

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> snapshotBitmap;
	HRESULT createResult =
		deviceContext->CreateBitmap(pixelSize, nullptr, 0, properties, &snapshotBitmap);
	if (FAILED(createResult) || !snapshotBitmap) {
		return nullptr;
	}

	SnapshotBitmapCacheEntry cacheEntry{snapshotBitmap, pixelSize, pixelFormat, dpiX, dpiY};
	if (snapshotBitmapCacheIndex < snapshotBitmapCache.size()) {
		snapshotBitmapCache[snapshotBitmapCacheIndex] = cacheEntry;
	} else {
		snapshotBitmapCache.push_back(cacheEntry);
	}

	snapshotBitmapCacheIndex++;
	return snapshotBitmap;
}

ID2D1DeviceContext* CommandListSnapshotter::acquireSnapshotDeviceContext(
	ID2D1DeviceContext* deviceContext
) {
	if (!deviceContext) {
		return nullptr;
	}

	Microsoft::WRL::ComPtr<ID2D1Device> device;
	deviceContext->GetDevice(&device);
	if (!device) {
		return nullptr;
	}

	if (snapshotDeviceContext && snapshotDevice.Get() == device.Get()) {
		return snapshotDeviceContext.Get();
	}

	snapshotBitmapCache.clear();
	snapshotBitmapCacheIndex = 0;
	snapshotDevice = device;
	snapshotDeviceContext.Reset();

	HRESULT createResult =
		snapshotDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &snapshotDeviceContext);
	if (FAILED(createResult) || !snapshotDeviceContext) {
		snapshotDevice.Reset();
		return nullptr;
	}

	return snapshotDeviceContext.Get();
}
