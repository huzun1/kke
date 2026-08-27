#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "kke/appearance/Scale.hh"
#include "kke/appearance/resource/font/Font.hh"

#include "PageId.hh"
#include "pages/ShadowAndClippingPage.hh"
#include "pages/TransformPage.hh"

namespace kke {
class D2dEngine;
}
namespace kke::example::overlay {
class PageController {
	kke::D2dEngine& engine;
	std::vector<uint8_t> fontData;
	std::shared_ptr<kke::Font> font;
	std::array<bool, 4> functionKeyStates{};
	TransformPage transformPage;
	ShadowAndClippingPage shadowAndClippingPage;
	PageId activePage = PageId::Fill;

  public:
	explicit PageController(kke::D2dEngine& engine);

	void initialize();

	void render(kke::Scale viewportSize);

  private:
	void updateActivePage();

	void renderActivePage(kke::Scale viewportSize);

	void renderGuide();

	void loadSystemFont();
};
} // namespace kke::example::overlay
