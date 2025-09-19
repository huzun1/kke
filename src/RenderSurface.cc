#include <d2d1_1.h>
#include <wrl/client.h>

#include <oreik/RenderSurface.hpp>

oreik::RenderSurface::RenderSurface(Microsoft::WRL::ComPtr<ID2D1Bitmap1> renderTarget)
	: renderTarget(renderTarget) {
}

void oreik::RenderSurface::setLocking(bool locking) {
	this->locking = locking;
}

bool oreik::RenderSurface::isLocking() const {
	return locking;
}

ID2D1Bitmap1* oreik::RenderSurface::getRenderTarget() {
	return renderTarget.Get();
}

oreik::RenderSurface oreik::RenderSurface::createSurface(ID2D1DeviceContext* context) {
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

	return oreik::RenderSurface(renderTarget);
}
