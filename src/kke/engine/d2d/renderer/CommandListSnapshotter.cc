#include "CommandListSnapshotter.hh"

using namespace kke;

void CommandListSnapshotter::beginFrame() {
	for (SnapshotBitmapCacheEntry& cacheEntry : snapshotBitmapCache) {
		cacheEntry.isInUseThisFrame = false;
	}
}

Microsoft::WRL::ComPtr<ID2D1Bitmap1> CommandListSnapshotter::snapshot(
	ID2D1DeviceContext* deviceContext,
	ID2D1Image* source,
	ID2D1Bitmap* referenceTarget,
	SnapshotOpacityMode opacityMode
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
	clearSnapshotTarget(snapshotContext, opacityMode);
	snapshotContext->DrawImage(source);

	HRESULT drawResult = snapshotContext->EndDraw();

	snapshotContext->SetTransform(previousTransform);
	snapshotContext->SetTarget(previousTarget.Get());

	if (FAILED(drawResult)) {
		return nullptr;
	}

	return snapshotBitmap;
}

Microsoft::WRL::ComPtr<ID2D1Bitmap1> CommandListSnapshotter::snapshotComposite(
	ID2D1DeviceContext* deviceContext,
	ID2D1Image* background,
	ID2D1Image* foreground,
	ID2D1Bitmap* referenceTarget,
	SnapshotOpacityMode opacityMode
) {
	if (!deviceContext || !referenceTarget || (!background && !foreground)) {
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
	clearSnapshotTarget(snapshotContext, opacityMode);
	if (background) {
		snapshotContext->DrawImage(background);
	}
	if (foreground) {
		snapshotContext->DrawImage(foreground);
	}

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

	for (SnapshotBitmapCacheEntry& cachedEntry : snapshotBitmapCache) {
		if (
			!cachedEntry.isInUseThisFrame &&
			cachedEntry.bitmap &&
			cachedEntry.pixelSize.width == pixelSize.width &&
			cachedEntry.pixelSize.height == pixelSize.height &&
			cachedEntry.pixelFormat.format == pixelFormat.format &&
			cachedEntry.pixelFormat.alphaMode == pixelFormat.alphaMode &&
			cachedEntry.dpiX == dpiX &&
			cachedEntry.dpiY == dpiY
		) {
			cachedEntry.isInUseThisFrame = true;
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

	SnapshotBitmapCacheEntry cacheEntry{
		snapshotBitmap,
		pixelSize,
		pixelFormat,
		dpiX,
		dpiY,
		true
	};
	snapshotBitmapCache.push_back(cacheEntry);
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

void CommandListSnapshotter::clearSnapshotTarget(
	ID2D1DeviceContext* deviceContext,
	SnapshotOpacityMode opacityMode
) {
	if (opacityMode == SnapshotOpacityMode::FlattenToOpaqueBlack) {
		deviceContext->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));
		return;
	}

	deviceContext->Clear();
}
