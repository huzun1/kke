#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "D2dFont.hh"
#include "TextFormatProvider.hh"
#include "TextLayoutProvider.hh"
#include "kke/appearance/Text.hh"
#include "kke/appearance/resource/font/FontAppearance.hh"
#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
class FontProvider {
	Microsoft::WRL::ComPtr<IDWriteFactory5> writeFactory;
	Microsoft::WRL::ComPtr<IDWriteInMemoryFontFileLoader> fontFileLoader;
	Microsoft::WRL::ComPtr<IDWriteFontSetBuilder1> fontSetBuilder;
	Microsoft::WRL::ComPtr<IDWriteFontCollection1> fontCollection;
	std::vector<Microsoft::WRL::ComPtr<IDWriteFontFile>> fontFiles;
	std::vector<std::shared_ptr<D2dFont>> fonts;
	TextFormatProvider textFormatProvider;
	TextLayoutProvider textLayoutProvider;

  public:
	FontProvider();

	~FontProvider();

	std::shared_ptr<D2dFont> uploadFont(void const* data, size_t size);

	Microsoft::WRL::ComPtr<IDWriteTextFormat> createTextFormat(FontAppearance const& appearance);

	Microsoft::WRL::ComPtr<IDWriteTextLayout> createTextLayout(Text const& text);

	std::vector<std::shared_ptr<D2dFont>> const& getFonts() const;

  private:
	void initialize();

	void rebuildFontCollection();

	void clearTextCaches();
};
} // namespace kke
