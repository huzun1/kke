#include <d2d1_1.h>
#include <wrl/client.h>

#include <kke/RenderSurface.hh>

kke::RenderSurface::RenderSurface(Microsoft::WRL::ComPtr<ID2D1Bitmap1> renderTarget)
	: renderTarget(renderTarget) {
}

void kke::RenderSurface::setLocking(bool locking) {
	this->locking = locking;
}

bool kke::RenderSurface::isLocking() const {
	return locking;
}

ID2D1Bitmap1* kke::RenderSurface::getRenderTarget() {
	return renderTarget.Get();
}

kke::RenderSurface kke::RenderSurface::createSurface(ID2D1DeviceContext* context) {
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> renderTarget;

	D2D1_SIZE_U screenSize = context->GetPixelSize();
	float dpiX, dpiY;
	context->GetDpi(&dpiX, &dpiY);
	D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, context->GetPixelFormat(), dpiX, dpiY);
	context->CreateBitmap(
		screenSize,
		nullptr,
		0,
		properties,
		&renderTarget);

	return kke::RenderSurface(renderTarget);
}
