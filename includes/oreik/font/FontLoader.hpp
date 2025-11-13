#pragma once

#include <dwrite.h>
#include <dwrite_3.h>
#include <dxgi1_2.h>
#include <dxgi1_4.h>

#include <string>

#include "oreik/font/FontWeight.hpp"

#pragma comment(lib, "dwrite.lib")

namespace oreik {
class FontLoader {
	IDWriteFactory5* writeFactory = nullptr;
	IDWriteInMemoryFontFileLoader* fontFileLoader = nullptr;
	IDWriteFontSetBuilder1* fontSetBuilder = nullptr;
	IDWriteFontCollection1* fontCollection = nullptr;

public:
	void preInit();

	void loadFont(const void* data, size_t size);

	void init();

	IDWriteTextFormat* createTextFormat(std::wstring const& fontFamily, int32_t fontSize, oreik::FontWeight weight);

	IDWriteTextLayout* createTextLayout(std::wstring const& text, IDWriteTextFormat* textFormat);
};
};	// namespace oreik
