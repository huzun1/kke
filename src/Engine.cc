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

void oreik::Engine::drawLine(oreik::Point2f start, oreik::Point2f end, oreik::Brush const& brush, float strokeWidth) {
	deviceContext->DrawLine(start.point2f(), end.point2f(), resourceAllocator.aquireOrCreate(brush), strokeWidth);
}

void oreik::Engine::drawRect(oreik::Rect const& rect, oreik::Brush const& brush, float strokeWidth) {
	deviceContext->DrawRectangle(rect.rectF(), resourceAllocator.aquireOrCreate(brush), strokeWidth);
}

void oreik::Engine::drawRounded(oreik::Rect const& rect, float radius, oreik::Brush const& brush, float strokeWidth) {
	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(rect.rectF(), radius, radius);
	deviceContext->DrawRoundedRectangle(roundedRect, resourceAllocator.aquireOrCreate(brush), strokeWidth);
}

void oreik::Engine::drawEllipse(oreik::Ellipse const& ellipse, oreik::Brush const& brush, float strokeWidth) {
	deviceContext->DrawEllipse(ellipse.ellipse(), resourceAllocator.aquireOrCreate(brush), strokeWidth);
}

void oreik::Engine::fillRect(oreik::Rect const& rect, oreik::Brush const& brush) {
	deviceContext->FillRectangle(rect.rectF(), resourceAllocator.aquireOrCreate(brush));
}

void oreik::Engine::fillRounded(oreik::Rect const& rect, float radius, oreik::Brush const& brush) {
	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(rect.rectF(), radius, radius);
	deviceContext->FillRoundedRectangle(roundedRect, resourceAllocator.aquireOrCreate(brush));
}

void oreik::Engine::fillEllipse(oreik::Ellipse const& ellipse, oreik::Brush const& brush) {
	deviceContext->FillEllipse(ellipse.ellipse(), resourceAllocator.aquireOrCreate(brush));
}
