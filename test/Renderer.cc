#include "Renderer.hh"

#include <D2D1.h>
#include <d2d1_1.h>
#include <d2d1_2.h>
#include <wincodec.h>

#include <cmath>
#include <memory>

#include "FpsCounter.hh"
#include "kke/brush/LinearGradientBrush.hh"
#include "kke/brush/SolidColorBrush.hh"
#include "kke/common/Color.hh"
#include "kke/common/geometry/Ellipse.hh"
#include "kke/common/geometry/Rect.hh"
#include "kke/common/geometry/RoundedRect.hh"
#include "kke/font/FontData.hh"
#include "resources/resources.h"

static FpsCounter counter;
application::Renderer::Renderer(application::D2D1& d2d1)
	: d2d1(d2d1) {
	this->engine = std::make_unique<kke::Engine>(d2d1.getFactory(), d2d1.getDeviceContext());

	// Load font resources
	auto [fontData, fontDataSize] = loadResource(FONT_SPACE_GROTESK);
	std::vector<FontData> fonts;
	if (fontData) {
		fonts.push_back({fontData, fontDataSize});
		printf("font loaded: %p, %lld\n", fontData, fontDataSize);
	}

	// Initialize engine with loaded fonts
	this->engine->init(fonts);

	auto [data, size] = loadResource(TEXTURE_DYCONTRAST);
	if (data) {
		dyconTexId = this->engine->loadTexture(data, size);
		printf("resource loaded: %p, %lld\n", data, size);
		printf("texture loaded: %lld\n", dyconTexId);
	}
}

void application::Renderer::render() {
	// preRender();

	ID2D1DeviceContext* deviceContext = d2d1.getDeviceContext();
	ID2D1Bitmap1* renderTarget = d2d1.getRenderTarget();

	this->engine->begin(renderTarget);
	renderFrame();
	ID2D1Image* output;
	this->engine->end(&output);

	// render the output
	deviceContext->SetTarget(renderTarget);
	deviceContext->BeginDraw();
	deviceContext->DrawImage(output);
	deviceContext->EndDraw();

	output->Release();
}

void application::Renderer::preRender() {
	ID2D1DeviceContext* deviceContext = d2d1.getDeviceContext();
	ID2D1Bitmap1* renderTarget = d2d1.getRenderTarget();

	deviceContext->SetTarget(renderTarget);
	deviceContext->BeginDraw();

	D2D1_RECT_F rect{90, 90, 200, 200};
	D2D1_COLOR_F color{1.0f, 0.0f, 0.0f, 1.0f};
	ID2D1SolidColorBrush* brush;
	deviceContext->CreateSolidColorBrush(&color, nullptr, &brush);
	deviceContext->FillRectangle(rect, brush);

	deviceContext->EndDraw();
}

void application::Renderer::renderFrame() {
	// Generic Rendering Test
	this->engine->drawRect(kke::Rect{20, 20, 100, 100}, kke::SolidColorBrush({0.0f, 1.0f, 0.0f, 1.0f}), 10.0f);
	this->engine->fillRect(kke::Rect{60, 20, 160, 100}, kke::SolidColorBrush({0.0f, 1.0f, 1.0f, 1.0f}));
	this->engine->fillRounded(kke::RoundedRect({20, 60, 160, 220}, 3.0f), kke::SolidColorBrush({1.0f, 0.0f, 1.0f, 0.5f}));
	this->engine->fillEllipse(kke::Ellipse(300, 300, 30), kke::SolidColorBrush({1.0, 0.0f, 0.0f, 1.0f}));

	// Scale Test
	static int angle = 0;
	angle++;
	kke::Rect rotRect{300, 90, 500, 200};
	this->engine->pushRotate(rotRect.center(), angle);
	this->engine->fillRounded({rotRect, 3.0f}, kke::SolidColorBrush({0.0f, 0.0f, 1.0f, 1.0f}));
	this->engine->popTransform();

	// Rotate Test
	static float theta = 0;
	theta += 0.01f;
	kke::Rect scaleRect{200, 400, 400, 600};
	this->engine->pushScale(scaleRect.center(), {std::sin(theta) * 0.5f + 1.0f, std::sin(theta) * 0.5f + 1.0f});
	this->engine->fillRounded({scaleRect, 3.0f}, kke::SolidColorBrush({0.0f, 1.0f, 0.0f, 1.0f}));
	this->engine->popTransform();

	// Shadow Test
	kke::RoundedRect rect{{200.0f, 200.0f, 400.0f, 400.0f}, 15.0f};
	static float gradientAngle = 0.0f;
	gradientAngle += 0.5f;

	auto brush = kke::LinearGradientBrush({kke::Color4f(KKE_COLOR_16(0x205034)),
										   kke::Color4f(KKE_COLOR_16(0x680620)), kke::Color4f(KKE_COLOR_16(0xFF06FF))},
										  {rect.x1, rect.y1},
										  {rect.x2, rect.y2});
	brush.setAngle(gradientAngle);
	this->engine->drawRoundedShadow(rect, brush, 10.0f);
	this->engine->fillRounded(rect, brush);

	this->engine->drawRectShadow({600, 300, 800, 400}, kke::SolidColorBrush({1.0f, 0.0f, 0.0f, 1.0f}), 10.0f);

	// Texture Test
	static int dyconAngle = 0;
	dyconAngle += 3;
	kke::Rect dyconRect{500, 500, 700, 700};
	this->engine->pushRotate(dyconRect.center(), dyconAngle);
	this->engine->drawTexture(dyconTexId, dyconRect);
	this->engine->popTransform();

	// Text Test
	this->engine->drawText(
		{50, 300},
		L"Hello, kke!",
		kke::FontWeight::BOLD,
		L"Space Grotesk",
		32,
		kke::SolidColorBrush({0.0f, 0.0f, 0.0f, 1.0f}));

	this->engine->drawTextShadow(
		{50, 350},
		L"Hello, kke with Shadow!",
		kke::FontWeight::BOLD,
		L"Space Grotesk",
		32,
		kke::SolidColorBrush({1.0f, 1.0f, 1.0f, 1.0f}),
		5.0f);
	this->engine->drawText(
		{50, 350},
		L"Hello, kke with Shadow! ggggggggggggggyyyyyyyyyyyyy",
		kke::FontWeight::BOLD,
		L"Space Grotesk",
		32,
		kke::SolidColorBrush({0.0f, 0.0f, 0.0f, 1.0f}));

	// Draw FPS
	counter.frame();
	this->engine->drawText(
		{10, 10},
		L"FPS: " + std::to_wstring(static_cast<int>(counter.fps())),
		kke::FontWeight::NORMAL,
		L"Space Grotesk",
		16,
		kke::SolidColorBrush({1.0f, 1.0f, 1.0f, 1.0f}));

	// Ranged-blur Test
	float offset = 50 + std::sin(theta) * 50.0f;
	kke::RoundedRect blurRect{{0.0f + offset, 50.0f + offset, 300.0f + offset, 300.0f + offset}, 10.0f};
	this->engine->flush(); // Ensure all previous drawing commands are executed
	this->engine->blur(30.0f, &blurRect);
	this->engine->drawRounded(blurRect, kke::SolidColorBrush(KKE_COLOR_24(0x000000FF)), 2.0f);
}

std::pair<void*, size_t> application::Renderer::loadResource(int resourceId) {
	const HRSRC resourceInfo = FindResourceA(NULL, MAKEINTRESOURCEA(resourceId), "WAVE");
	if (!resourceInfo) {
		return {nullptr, 0};
	}
	const HGLOBAL resourceData = LoadResource(NULL, resourceInfo);
	if (!resourceData) {
		return {nullptr, 0};
	}
	const LPVOID resourcePtr = LockResource(resourceData);
	const DWORD resourceSize = SizeofResource(NULL, resourceInfo);
	return {static_cast<char*>(resourcePtr), resourceSize};
}
