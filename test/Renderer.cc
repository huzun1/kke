#include "Renderer.hpp"

#include <D2D1.h>
#include <d2d1_1.h>

#include <memory>

#include "oreik/brush/SolidColorBrush.hpp"
#include "oreik/common/Ellipse.hpp"

application::Renderer::Renderer(application::D2D1& d2d1)
	: d2d1(d2d1) {
	this->engine = std::make_unique<oreik::Engine>(d2d1.getDeviceContext());
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
	this->engine->drawLine({90, 90}, {700, 700}, oreik::SolidColorBrush({0.0f, 0.0f, 1.0f, 1.0f}), 3.0f);
	this->engine->drawRect({20, 20, 100, 100}, oreik::SolidColorBrush({0.0f, 1.0f, 0.0f, 1.0f}), 10.0f);
	this->engine->fillRect({60, 20, 160, 100}, oreik::SolidColorBrush({0.0f, 1.0f, 1.0f, 1.0f}));
	this->engine->fillRounded({20, 60, 160, 220}, 10.0f, oreik::SolidColorBrush({1.0f, 0.0f, 1.0f, 0.5f}));
	this->engine->fillEllipse(oreik::Ellipse(300, 300, 30), oreik::SolidColorBrush({1.0, 0.0f, 0.0f, 1.0f}));
}
