#include "RenderPass.hh"
#include "kke/engine/d2d/context/D2dContext.hh"

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
}

void RenderPass::endDraw(D2dEngineContext& context) {
	if (!lastRenderTarget) {
		// TODO: log error
		return;
	}

	D2dContext* d2dContext = context.getD2dContext();

	ID2D1DeviceContext* deviceContext = d2dContext->getDeviceContext();

	deviceContext->SetTarget(lastRenderTarget);
	deviceContext->DrawImage(d2dContext->getTargetCommandList().Get());

	deviceContext->EndDraw();
}

void RenderPass::clear(D2dEngineContext& context) {
	D2dContext* d2dContext = context.getD2dContext();
	ID2D1DeviceContext* deviceContext = d2dContext->getDeviceContext();
	deviceContext->Clear();
}

Microsoft::WRL::ComPtr<ID2D1Bitmap> RenderPass::createBitmapCopy(ID2D1DeviceContext* deviceContext, ID2D1Bitmap* source) {
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmapCopy;

	float dpiX, dpiY;
	source->GetDpi(&dpiX, &dpiY);

	auto properties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_NONE,
											  source->GetPixelFormat(), dpiX, dpiY);

	deviceContext->CreateBitmap(
		source->GetPixelSize(),
		nullptr,
		0.0f,
		properties,
		&bitmapCopy);

	return bitmapCopy;
}
