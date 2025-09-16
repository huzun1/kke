#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dcommon.h>

#include <oreik/Engine.hpp>

#include "oreik/ResourceAllocator.hpp"

oreik::Engine::Engine(ID2D1DeviceContext* deviceContext)
	: deviceContext(deviceContext), resourceAllocator(deviceContext) {
}

void oreik::Engine::begin(ID2D1Bitmap* screen) {
	this->screenBitmap = screen;

	// Create a bitmap of the same size as the current screen
	// and use it as the target
	D2D1_SIZE_U screenSize = screen->GetPixelSize();
	float dpiX, dpiY;
	screen->GetDpi(&dpiX, &dpiY);

	D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET, screen->GetPixelFormat(), dpiX, dpiY);
	deviceContext->CreateBitmap(
		screenSize,
		nullptr,
		0,
		properties,
		&renderTarget);
	D2D1_POINT_2U destPoint(0, 0);
	D2D1_RECT_U srcRectangle(0, 0, screenSize.width, screenSize.height);

	deviceContext->BeginDraw();
	deviceContext->SetTarget(renderTarget);
	deviceContext->Clear();

	renderTarget->CopyFromBitmap(&destPoint, screen, &srcRectangle);
}

void oreik::Engine::end(ID2D1Image** output) {
	deviceContext->EndDraw();
	*output = renderTarget;
}

void oreik::Engine::drawRect(D2D1_RECT_F rect, oreik::Brush const& brush, float strokeWidth) {
	deviceContext->DrawRectangle(rect, resourceAllocator.aquireOrCreate(brush), strokeWidth);
}
