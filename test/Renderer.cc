#include "Renderer.hpp"

#include <D2D1.h>
#include <d2d1_1.h>

#include <memory>

application::Renderer::Renderer(application::D2D1& d2d1)
	: d2d1(d2d1) {
	this->engine = std::make_unique<oreik::Engine>(d2d1.getDeviceContext());
}

void application::Renderer::render() {
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

void application::Renderer::renderFrame() {
	this->engine->drawRect({20, 20, 100, 100}, {0.0f, 1.0f, 0.0f, 1.0f}, 10.0f);
}
