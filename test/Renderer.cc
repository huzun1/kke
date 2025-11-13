#include "Renderer.hpp"

#include <D2D1.h>
#include <d2d1_1.h>
#include <d2d1_2.h>
#include <wincodec.h>

#include <cmath>
#include <memory>

#include "oreik/brush/LinearGradientBrush.hpp"
#include "oreik/brush/SolidColorBrush.hpp"
#include "oreik/common/Color.hpp"
#include "oreik/common/geometry/Ellipse.hpp"
#include "oreik/common/geometry/Rect.hpp"
#include "oreik/common/geometry/RoundedRect.hpp"
#include "resources/resources.h"

application::Renderer::Renderer(application::D2D1& d2d1)
	: d2d1(d2d1) {
	this->engine = std::make_unique<oreik::Engine>(d2d1.getDeviceContext());
	this->engine->init({});

	auto [data, size] = loadResource(TEXTURE_DYCONTRAST);
	if (data) {
		dyconTexId = this->engine->loadTexture(data, size);
		printf("resource loaded: %p, %lld\n", data, size);
		printf("texture loaded: %lld\n", dyconTexId);
	}
}

void application::Renderer::render() {
	preRender();

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
	this->engine->drawRect(oreik::Rect{20, 20, 100, 100}, oreik::SolidColorBrush({0.0f, 1.0f, 0.0f, 1.0f}), 10.0f);
	this->engine->fillRect(oreik::Rect{60, 20, 160, 100}, oreik::SolidColorBrush({0.0f, 1.0f, 1.0f, 1.0f}));
	this->engine->fillRounded(oreik::RoundedRect({20, 60, 160, 220}, 3.0f), oreik::SolidColorBrush({1.0f, 0.0f, 1.0f, 0.5f}));
	this->engine->fillEllipse(oreik::Ellipse(300, 300, 30), oreik::SolidColorBrush({1.0, 0.0f, 0.0f, 1.0f}));

	// Scale Test
	static int angle = 0;
	angle++;
	oreik::Rect rotRect{300, 90, 500, 200};
	this->engine->pushRotate(rotRect.center(), angle);
	this->engine->fillRounded({rotRect, 3.0f}, oreik::SolidColorBrush({0.0f, 0.0f, 1.0f, 1.0f}));
	this->engine->popTransform();

	// Rotate Test
	static float theta = 0;
	theta += 0.01f;
	oreik::Rect scaleRect{200, 400, 400, 600};
	this->engine->pushScale(scaleRect.center(), {std::sin(theta) * 0.5f + 1.0f, std::sin(theta) * 0.5f + 1.0f});
	this->engine->fillRounded({scaleRect, 3.0f}, oreik::SolidColorBrush({0.0f, 1.0f, 0.0f, 1.0f}));
	this->engine->popTransform();

	// Shadow Test
	oreik::RoundedRect rect{{200.0f, 200.0f, 400.0f, 400.0f}, 15.0f};
	static float gradientAngle = 0.0f;
	gradientAngle += 0.5f;

	auto brush = oreik::LinearGradientBrush({oreik::Color4f(0x205034),
											 oreik::Color4f(0x680620), oreik::Color4f(0xFF06FF)},
											{rect.x1, rect.y1},
											{rect.x2, rect.y2});
	brush.setAngle(gradientAngle);
	this->engine->drawRoundedShadow(rect, brush, 10.0f);
	this->engine->fillRounded(rect, brush);

	this->engine->drawRectShadow({600, 300, 800, 400}, oreik::SolidColorBrush({1.0f, 0.0f, 0.0f, 1.0f}), 10.0f);

	// Texture Test
	static int dyconAngle = 0;
	dyconAngle += 3;
	oreik::Rect dyconRect{500, 500, 700, 700};
	this->engine->pushRotate(dyconRect.center(), dyconAngle);
	this->engine->drawTexture(dyconTexId, dyconRect);
	this->engine->popTransform();

	// Text Test
	this->engine->drawText(
		{50, 300},
		L"Hello, Oreik!",
		oreik::FontWeight::BOLD,
		L"Segoe UI",
		32,
		oreik::SolidColorBrush({0.0f, 0.0f, 0.0f, 1.0f}));

	this->engine->drawTextShadow(
		{50, 350},
		L"Hello, Oreik with Shadow!",
		oreik::FontWeight::BOLD,
		L"Segoe UI",
		32,
		oreik::SolidColorBrush({1.0f, 1.0f, 1.0f, 1.0f}),
		5.0f);
	this->engine->drawText(
		{50, 350},
		L"Hello, Oreik with Shadow!",
		oreik::FontWeight::BOLD,
		L"Segoe UI",
		32,
		oreik::SolidColorBrush({0.0f, 0.0f, 0.0f, 1.0f}));
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
