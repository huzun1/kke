#include <d2d1_1.h>
#include <d2d1helper.h>
#include <wrl/client.h>

#include <kke/RenderSurface.hh>

#include "internal/HResult.hh"

using namespace kke;
using namespace Microsoft::WRL;
using kke::internal::throwIfFailed;

RenderSurface::RenderSurface(ComPtr<ID2D1Bitmap1> renderTarget)
	: renderTarget(renderTarget) {
}

void RenderSurface::setLocking(bool locking) {
	this->locking = locking;
}

bool RenderSurface::isLocking() const {
	return locking;
}

ID2D1Bitmap1* RenderSurface::getRenderTarget() const {
	return renderTarget.Get();
}

std::shared_ptr<RenderSurface> RenderSurface::createSurface(ID2D1DeviceContext* context) {
	ComPtr<ID2D1Bitmap1> renderTarget;

	D2D1_SIZE_U screenSize = context->GetPixelSize();
	float dpiX, dpiY;
	context->GetDpi(&dpiX, &dpiY);
	D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, context->GetPixelFormat(), dpiX, dpiY);
	throwIfFailed(
		context->CreateBitmap(
			screenSize,
			nullptr,
			0,
			properties,
			renderTarget.GetAddressOf()),
		"Failed to create render surface bitmap");

	return std::make_shared<RenderSurface>(renderTarget);
}
