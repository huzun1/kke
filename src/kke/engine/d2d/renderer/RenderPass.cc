#include "RenderPass.hh"
#include <d2d1.h>

#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/utils/DebugLog.hh"

using namespace kke;

void RenderPass::beginDraw(D2dEngineContext& context, ID2D1Bitmap* renderTarget) {
	D2dContext* d2dContext = context.getD2dContext();
	ID2D1DeviceContext* deviceContext = d2dContext->getDeviceContext();

	Microsoft::WRL::ComPtr<ID2D1CommandList> targetCommandList;
	commandListSnapshotter.beginFrame();

	deviceContext->BeginDraw();

	deviceContext->CreateCommandList(&targetCommandList);
	deviceContext->SetTarget(targetCommandList.Get());
	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

	d2dContext->setTargetCommandList(targetCommandList);

	lastRenderTarget = renderTarget;
	cachedTargetSnapshot.Reset();
	shouldPreserveRenderTarget = true;
	shouldFlattenNextTargetSnapshot = true;
	isPreservedBaseBitmapReady = false;
}

void RenderPass::endDraw(D2dEngineContext& context) {
	if (!lastRenderTarget) {
		kke::debug::log("[kke][RenderPass] endDraw called without a render target");
		return;
	}

	D2dContext* d2dContext = context.getD2dContext();

	ID2D1DeviceContext* deviceContext = d2dContext->getDeviceContext();

	deviceContext->SetTarget(lastRenderTarget);
	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

	// FIXME: make target command list abstract
	ID2D1CommandList* targetCommandList = d2dContext->getTargetCommandList().Get();
	targetCommandList->Close();
	if (!shouldPreserveRenderTarget) {
		deviceContext->Clear();
	}
	deviceContext->DrawImage(targetCommandList);

	deviceContext->EndDraw();
}

void RenderPass::clear(D2dEngineContext& context) {
	D2dContext* d2dContext = context.getD2dContext();
	ID2D1DeviceContext* deviceContext = d2dContext->getDeviceContext();
	deviceContext->Clear();
	shouldPreserveRenderTarget = false;
	shouldFlattenNextTargetSnapshot = true;
	isPreservedBaseBitmapReady = false;
}

Microsoft::WRL::ComPtr<ID2D1Image>
RenderPass::cycleTargetSnapshot(D2dEngineContext& context, SnapshotOpacityMode opacityMode) {
	D2dContext* d2dContext = context.getD2dContext();
	Microsoft::WRL::ComPtr<ID2D1CommandList> currentTargetCommandList =
		d2dContext->getTargetCommandList();
	if (!currentTargetCommandList || !lastRenderTarget) {
		return nullptr;
	}

	ID2D1DeviceContext* deviceContext = d2dContext->getDeviceContext();
	D2D1_MATRIX_3X2_F activeTransform;
	deviceContext->GetTransform(&activeTransform);
	Microsoft::WRL::ComPtr<ID2D1Image> snapshotImage = cachedTargetSnapshot;
	HRESULT closeResult = currentTargetCommandList->Close();
	if (FAILED(closeResult)) {
		return nullptr;
	}

	if (!snapshotImage) {
		SnapshotOpacityMode effectiveOpacityMode = opacityMode;
		if (shouldFlattenNextTargetSnapshot) {
			effectiveOpacityMode = SnapshotOpacityMode::FlattenToOpaqueBlack;
		} else if (effectiveOpacityMode == SnapshotOpacityMode::FlattenToOpaqueBlack) {
			effectiveOpacityMode = SnapshotOpacityMode::PreserveAlpha;
		}

		if (shouldPreserveRenderTarget) {
			Microsoft::WRL::ComPtr<ID2D1Bitmap> baseBitmap =
				acquirePreservedBaseBitmap(deviceContext);
			snapshotImage = commandListSnapshotter.snapshotComposite(
				deviceContext,
				baseBitmap.Get(),
				currentTargetCommandList.Get(),
				lastRenderTarget,
				effectiveOpacityMode
			);
		} else {
			snapshotImage = commandListSnapshotter.snapshot(
				deviceContext,
				currentTargetCommandList.Get(),
				lastRenderTarget,
				effectiveOpacityMode
			);
		}
	}
	if (!snapshotImage) {
		return nullptr;
	}

	Microsoft::WRL::ComPtr<ID2D1CommandList> nextTargetCommandList;
	HRESULT createResult = deviceContext->CreateCommandList(&nextTargetCommandList);
	if (FAILED(createResult) || !nextTargetCommandList) {
		return nullptr;
	}

	deviceContext->SetTarget(nextTargetCommandList.Get());
	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	deviceContext->DrawImage(snapshotImage.Get());
	deviceContext->SetTransform(activeTransform);
	d2dContext->setTargetCommandList(nextTargetCommandList);
	cachedTargetSnapshot.Reset();
	shouldPreserveRenderTarget = false;
	shouldFlattenNextTargetSnapshot = false;
	return snapshotImage;
}

void RenderPass::invalidateCachedTargetSnapshot() {
	cachedTargetSnapshot.Reset();
}

Microsoft::WRL::ComPtr<ID2D1Bitmap>
RenderPass::acquirePreservedBaseBitmap(ID2D1DeviceContext* deviceContext) {
	if (!shouldPreserveRenderTarget || !deviceContext || !lastRenderTarget) {
		return nullptr;
	}

	if (isPreservedBaseBitmapReady) {
		return preservedBaseBitmap;
	}

	if (!isPreservedBaseBitmapCompatible(deviceContext, lastRenderTarget)) {
		preservedBaseBitmap = createPreservedBaseBitmap(deviceContext, lastRenderTarget);
		if (!preservedBaseBitmap) {
			preservedBaseDevice.Reset();
			return nullptr;
		}
		deviceContext->GetDevice(&preservedBaseDevice);
	}

	D2D1_SIZE_U pixelSize = lastRenderTarget->GetPixelSize();
	D2D1_POINT_2U destinationPoint{0, 0};
	D2D1_RECT_U sourceRect{0, 0, pixelSize.width, pixelSize.height};
	HRESULT copyResult =
		preservedBaseBitmap->CopyFromBitmap(&destinationPoint, lastRenderTarget, &sourceRect);
	if (FAILED(copyResult)) {
		preservedBaseBitmap.Reset();
		preservedBaseDevice.Reset();
		return nullptr;
	}
	isPreservedBaseBitmapReady = true;
	return preservedBaseBitmap;
}

bool RenderPass::isPreservedBaseBitmapCompatible(
	ID2D1DeviceContext* deviceContext, ID2D1Bitmap* source
) const {
	if (!preservedBaseBitmap || !preservedBaseDevice || !deviceContext || !source) {
		return false;
	}

	Microsoft::WRL::ComPtr<ID2D1Device> activeDevice;
	deviceContext->GetDevice(&activeDevice);
	if (activeDevice.Get() != preservedBaseDevice.Get()) {
		return false;
	}

	D2D1_SIZE_U cachedPixelSize = preservedBaseBitmap->GetPixelSize();
	D2D1_SIZE_U sourcePixelSize = source->GetPixelSize();
	D2D1_PIXEL_FORMAT cachedPixelFormat = preservedBaseBitmap->GetPixelFormat();
	D2D1_PIXEL_FORMAT sourcePixelFormat = source->GetPixelFormat();
	float cachedDpiX, cachedDpiY;
	float sourceDpiX, sourceDpiY;
	preservedBaseBitmap->GetDpi(&cachedDpiX, &cachedDpiY);
	source->GetDpi(&sourceDpiX, &sourceDpiY);
	return cachedPixelSize.width == sourcePixelSize.width &&
		   cachedPixelSize.height == sourcePixelSize.height &&
		   cachedPixelFormat.format == sourcePixelFormat.format &&
		   cachedPixelFormat.alphaMode == sourcePixelFormat.alphaMode && cachedDpiX == sourceDpiX &&
		   cachedDpiY == sourceDpiY;
}

Microsoft::WRL::ComPtr<ID2D1Bitmap>
RenderPass::createPreservedBaseBitmap(ID2D1DeviceContext* deviceContext, ID2D1Bitmap* source) {
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;

	float dpiX, dpiY;
	source->GetDpi(&dpiX, &dpiY);

	D2D1_SIZE_U pixelSize = source->GetPixelSize();
	D2D1_BITMAP_PROPERTIES1 properties =
		D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_NONE, source->GetPixelFormat(), dpiX, dpiY);

	HRESULT createResult =
		deviceContext->CreateBitmap(pixelSize, nullptr, 0.0f, properties, &bitmap);
	if (FAILED(createResult)) {
		return nullptr;
	}
	return bitmap;
}
