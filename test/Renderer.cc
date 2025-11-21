#include "Renderer.hh"

#include <D2D1.h>
#include <d2d1_1.h>
#include <d2d1_2.h>
#include <wincodec.h>

#include <cmath>
#include <memory>
#include <format>

#include "FpsCounter.hh"
#include "kke/brush/LinearGradientBrush.hh"
#include "kke/brush/SolidColorBrush.hh"
#include "kke/common/Color.hh"
#include "kke/common/geometry/Ellipse.hh"
#include "kke/common/geometry/Rect.hh"
#include "kke/common/geometry/RoundedRect.hh"
#include "kke/font/FontData.hh"
#include "kke/ui/DivBuilder.hh"
#include "kke/ui/Utilities.hh"
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

	// Initialize UI components
	initializeUI();
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
	// === UI Builder Component Rendering ===
	if (uiComponent) {
		uiComponent->render(this->engine.get());
	}

	// Draw FPS
	counter.frame();
	this->engine->drawText(
		{10, 10},
		L"FPS (1%/Avg/99%): " + std::to_wstring(static_cast<int>(counter.p1_fps())) + L"/" +
			std::to_wstring(static_cast<int>(counter.average_fps())) + L"/" +
			std::to_wstring(static_cast<int>(counter.p99_fps())),
		kke::FontWeight::NORMAL,
		L"Space Grotesk",
		16,
		kke::SolidColorBrush({0.0f, 0.0f, 0.0f, 1.0f}));
}

void application::Renderer::initializeUI() {
	using namespace kke::ui;

	// Create a UI component using the builder pattern
	// This matches the example provided by the user
	std::string text = "World";

	uiComponent = div()
		.flex()
		.flex_col()
		.gap(16.0f)
		.bg(rgb(0x2d3748))
		.width(600.0f)
		.padding(32.0f)
		.margin(100.0f, 50.0f, 0.0f, 50.0f)
		.justify_center()
		.items_center()
		.shadow_lg()
		.border(2.0f)
		.border_color(rgb(0x4299e1))
		.rounded_xl()
		.child(
			div()
				.text_size(24.0f)
				.text_color(rgb(0xffffff))
				.font_family(L"Space Grotesk")
				.font_bold()
				.child(std::format("Hello, {}!", text))
		)
		.child(
			div()
				.flex()
				.gap(16.0f)
				.child(div().size(64.0f).bg(red()).rounded_lg().shadow())
				.child(div().size(64.0f).bg(green()).rounded_lg().shadow())
				.child(div().size(64.0f).bg(blue()).rounded_lg().shadow())
				.child(div().size(64.0f).bg(yellow()).rounded_lg().shadow())
				.child(div().size(64.0f).bg(magenta()).rounded_lg().shadow())
				.child(div().size(64.0f).bg(cyan()).rounded_lg().shadow())
		)
		.child(
			div()
				.padding(16.0f, 32.0f, 16.0f, 32.0f)
				.bg(rgb(0x48bb78))
				.text_color(white())
				.rounded_md()
				.text_size(16.0f)
				.shadow_sm()
				.child("Click Meaaaaaaaaaaaaa\naaaaaaaaaaaaaaa!")
		)
		.build();

	// Measure text sizes before layout calculation
	uiComponent->measureTextIfNeeded(this->engine.get());

	// Calculate layout for 1280x720 window
	uiComponent->calculateLayout(1280.0f, 720.0f);
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
