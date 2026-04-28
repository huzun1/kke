#include "RenderPass.hh"

using namespace kke;

void RenderPass::beginDraw(D2dContext& context, ID2D1Bitmap* renderTarget) {
	Microsoft::WRL::ComPtr<ID2D1CommandList> targetCommandList;

	ID2D1DeviceContext* deviceContext = context.getDeviceContext();

	deviceContext->BeginDraw();

	deviceContext->CreateCommandList(&targetCommandList);
	deviceContext->SetTarget(targetCommandList.Get());

	// render the original render target
	Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmapCopy = createBitmapCopy(deviceContext, renderTarget);
	deviceContext->DrawBitmap(bitmapCopy.Get());

	context.setTargetCommandList(targetCommandList);
}

void RenderPass::endDraw(D2dContext& context) {
	if (!lastRenderTarget) {
		// TODO: log error
		return;
	}

	ID2D1DeviceContext* deviceContext = context.getDeviceContext();

	deviceContext->SetTarget(lastRenderTarget);
	deviceContext->DrawImage(context.getTargetCommandList().Get());

	deviceContext->EndDraw();
}

void RenderPass::clear(D2dContext& context) {
	ID2D1DeviceContext* deviceContext = context.getDeviceContext();
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
