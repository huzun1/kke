#include "PageController.hh"

#include <array>
#include <filesystem>
#include <fstream>

#include <Windows.h>

#include "kke/appearance/Text.hh"
#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/appearance/resource/effect/EffectSourceAppearance.hh"
#include "kke/appearance/resource/effect/shadow/ShadowEffect.hh"
#include "kke/appearance/resource/effect/shadow/ShadowMode.hh"
#include "kke/appearance/resource/font/FontWeight.hh"
#include "kke/engine/d2d/D2dEngine.hh"

#include "pages/FillPage.hh"
#include "pages/FullBlurPage.hh"

namespace kke::example::overlay {
PageController::PageController(kke::D2dEngine& engine) : engine(engine) {
}

void PageController::initialize() {
	loadSystemFont();
}

void PageController::render(kke::Scale viewportSize) {
	updateActivePage();
	renderActivePage(viewportSize);
	renderGuide();
}

void PageController::updateActivePage() {
	constexpr std::array<int, 4> functionKeys{VK_F6, VK_F7, VK_F8, VK_F9};
	constexpr std::array<PageId, 4> pages{
		PageId::Fill,
		PageId::Transform,
		PageId::ShadowAndClipping,
		PageId::FullBlur,
	};

	for (size_t index = 0; index < functionKeys.size(); ++index) {
		bool isPressed = (GetAsyncKeyState(functionKeys[index]) & 0x8000) != 0;
		if (isPressed && !functionKeyStates[index]) {
			activePage = pages[index];
		}
		functionKeyStates[index] = isPressed;
	}
}

void PageController::renderActivePage(kke::Scale viewportSize) {
	switch (activePage) {
	case PageId::Fill:
		FillPage::render(engine, viewportSize);
		break;
	case PageId::Transform:
		transformPage.render(engine, viewportSize);
		break;
	case PageId::ShadowAndClipping:
		shadowAndClippingPage.render(engine, viewportSize);
		break;
	case PageId::FullBlur:
		FullBlurPage::render(engine, viewportSize);
		break;
	}
}

void PageController::renderGuide() {
	kke::Brush titleBrush = kke::SolidColorBrush({0.96f, 0.98f, 1.0f, 1.0f});
	kke::Brush activeBrush = kke::SolidColorBrush({0.30f, 0.82f, 1.0f, 1.0f});
	kke::Brush inactiveBrush = kke::SolidColorBrush({0.70f, 0.75f, 0.82f, 1.0f});
	kke::ShadowEffect textShadow{
		{3.0f, 4.0f},
		4.0f,
		{0.0f, 0.0f, 0.0f, 0.86f},
		kke::ShadowMode::ShadowWithSource
	};
	kke::EffectSourceAppearance textAppearance;
	textAppearance.brush = titleBrush;

	engine.renderEffect(
		kke::Text{L"kke overlay", {40.0f, 34.0f}, {"Segoe UI", 22.0f, kke::FontWeight::BOLD}},
		textAppearance,
		textShadow
	);

	constexpr std::array<wchar_t const*, 4> descriptions{
		L"F6  Fill, border, gradient, text",
		L"F7  Rotate, scaling, translate",
		L"F8  Shadow and clipping",
		L"F9  Full-frame blur",
	};
	constexpr std::array<PageId, 4> pages{
		PageId::Fill,
		PageId::Transform,
		PageId::ShadowAndClipping,
		PageId::FullBlur,
	};

	for (size_t index = 0; index < descriptions.size(); ++index) {
		textAppearance.brush = pages[index] == activePage ? activeBrush : inactiveBrush;
		engine.renderEffect(
			kke::Text{
				descriptions[index],
				{40.0f, 72.0f + static_cast<float>(index) * 25.0f},
				{"Segoe UI",
				 16.0f,
				 pages[index] == activePage ? kke::FontWeight::BOLD : kke::FontWeight::NORMAL}
			},
			textAppearance,
			textShadow
		);
	}
}

void PageController::loadSystemFont() {
	wchar_t windowsDirectory[MAX_PATH]{};
	UINT length = GetWindowsDirectoryW(windowsDirectory, MAX_PATH);
	if (length == 0 || length >= MAX_PATH) {
		return;
	}

	std::filesystem::path fontPath(windowsDirectory);
	fontPath /= L"Fonts";
	fontPath /= L"segoeui.ttf";
	std::ifstream stream(fontPath, std::ios::binary | std::ios::ate);
	if (!stream) {
		return;
	}

	std::streamsize fontSize = stream.tellg();
	if (fontSize <= 0) {
		return;
	}
	fontData.resize(static_cast<size_t>(fontSize));
	stream.seekg(0, std::ios::beg);
	if (!stream.read(reinterpret_cast<char*>(fontData.data()), fontSize)) {
		fontData.clear();
		return;
	}

	font = engine.uploadFont(fontData.data(), fontData.size());
}
} // namespace kke::example::overlay
