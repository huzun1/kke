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

	float dpiX, dpiY;
	referenceTarget->GetDpi(&dpiX, &dpiY);

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> snapshotBitmap = acquireSnapshotBitmap(
		deviceContext,
		referenceTarget->GetPixelSize(),
		referenceTarget->GetPixelFormat(),
		dpiX,
		dpiY
	);
	if (!snapshotBitmap) {
		return nullptr;
	}

	Microsoft::WRL::ComPtr<ID2D1Image> previousTarget;
	deviceContext->GetTarget(&previousTarget);

	D2D1_MATRIX_3X2_F previousTransform;
	deviceContext->GetTransform(&previousTransform);

	deviceContext->SetTarget(snapshotBitmap.Get());
	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	deviceContext->Clear();
	deviceContext->DrawImage(source);

	HRESULT flushResult = deviceContext->Flush();

	deviceContext->SetTransform(previousTransform);
	deviceContext->SetTarget(previousTarget.Get());

	if (FAILED(flushResult)) {
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

	float width = std::max(1.0f, std::ceil(sourceBounds.right - sourceBounds.left));
	float height = std::max(1.0f, std::ceil(sourceBounds.bottom - sourceBounds.top));

	float dpiX, dpiY;
	referenceTarget->GetDpi(&dpiX, &dpiY);

	D2D1_SIZE_U pixelSize{
		static_cast<UINT32>(width),
		static_cast<UINT32>(height)
	};

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> snapshotBitmap = acquireSnapshotBitmap(
		deviceContext,
		pixelSize,
		referenceTarget->GetPixelFormat(),
		dpiX,
		dpiY
	);
	if (!snapshotBitmap) {
		return nullptr;
	}

	Microsoft::WRL::ComPtr<ID2D1Image> previousTarget;
	deviceContext->GetTarget(&previousTarget);

	D2D1_MATRIX_3X2_F previousTransform;
	deviceContext->GetTransform(&previousTransform);

	deviceContext->SetTarget(snapshotBitmap.Get());
	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	deviceContext->Clear();
	deviceContext->DrawImage(source, {-sourceBounds.left, -sourceBounds.top});

	HRESULT flushResult = deviceContext->Flush();

	deviceContext->SetTransform(previousTransform);
	deviceContext->SetTarget(previousTarget.Get());

	if (FAILED(flushResult)) {
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
