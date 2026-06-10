#include "RenderPass.hh"
#include <d2d1.h>

#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/utils/DebugLog.hh"

using namespace kke;

void RenderPass::beginDraw(D2dEngineContext& context, ID2D1Bitmap* renderTarget) {
	D2dContext* d2dContext = context.getD2dContext();
	ID2D1DeviceContext* deviceContext = d2dContext->getDeviceContext();

	Microsoft::WRL::ComPtr<ID2D1CommandList> targetCommandList;

	deviceContext->BeginDraw();

	deviceContext->CreateCommandList(&targetCommandList);
	deviceContext->SetTarget(targetCommandList.Get());

	// render the original render target
	Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmapCopy = createBitmapCopy(deviceContext, renderTarget);
	deviceContext->DrawBitmap(bitmapCopy.Get());

	d2dContext->setTargetCommandList(targetCommandList);

	lastRenderTarget = renderTarget;
}

void RenderPass::endDraw(D2dEngineContext& context) {
	if (!lastRenderTarget) {
		kke::debug::log("[kke][RenderPass] endDraw called without a render target");
		return;
	}

	D2dContext* d2dContext = context.getD2dContext();

	ID2D1DeviceContext* deviceContext = d2dContext->getDeviceContext();

	deviceContext->SetTarget(lastRenderTarget);
	deviceContext->Clear();

	// FIXME: make target command list abstract
	ID2D1CommandList* targetCommandList = d2dContext->getTargetCommandList().Get();
	targetCommandList->Close();
	deviceContext->DrawImage(targetCommandList);

	deviceContext->EndDraw();
}

void RenderPass::clear(D2dEngineContext& context) {
	D2dContext* d2dContext = context.getD2dContext();
	ID2D1DeviceContext* deviceContext = d2dContext->getDeviceContext();
	deviceContext->Clear();
}

Microsoft::WRL::ComPtr<ID2D1Image> RenderPass::cycleTargetCommandList(D2dEngineContext& context) {
	D2dContext* d2dContext = context.getD2dContext();
	Microsoft::WRL::ComPtr<ID2D1CommandList> currentTargetCommandList =
		d2dContext->getTargetCommandList();
	if (!currentTargetCommandList) {
		return nullptr;
	}

	HRESULT closeResult = currentTargetCommandList->Close();
	if (FAILED(closeResult)) {
		return nullptr;
	}

	ID2D1DeviceContext* deviceContext = d2dContext->getDeviceContext();

	Microsoft::WRL::ComPtr<ID2D1CommandList> nextTargetCommandList;
	HRESULT createResult = deviceContext->CreateCommandList(&nextTargetCommandList);
	if (FAILED(createResult) || !nextTargetCommandList) {
		return nullptr;
	}

	deviceContext->SetTarget(nextTargetCommandList.Get());
	d2dContext->setTargetCommandList(nextTargetCommandList);
	return currentTargetCommandList;
}

Microsoft::WRL::ComPtr<ID2D1Bitmap>
RenderPass::createBitmapCopy(ID2D1DeviceContext* deviceContext, ID2D1Bitmap* source) {
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmapCopy;

	float dpiX, dpiY;
	source->GetDpi(&dpiX, &dpiY);

	D2D1_SIZE_U pixelSize = source->GetPixelSize();
	D2D1_BITMAP_PROPERTIES1 properties =
		D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_NONE, source->GetPixelFormat(), dpiX, dpiY);

	deviceContext->CreateBitmap(pixelSize, nullptr, 0.0f, properties, &bitmapCopy);

	D2D1_POINT_2U origin = D2D1::Point2U(0, 0);
	D2D1_RECT_U sourceRect = D2D1::RectU(0, 0, pixelSize.width, pixelSize.height);
	bitmapCopy->CopyFromBitmap(&origin, source, &sourceRect);

	return bitmapCopy;
}
