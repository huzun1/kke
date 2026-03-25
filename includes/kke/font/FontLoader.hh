#pragma once

#include <dwrite.h>
#include <dwrite_3.h>
#include <dxgi1_2.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

#include <string>

#include "kke/font/FontWeight.hh"

#pragma comment(lib, "dwrite.lib")

namespace kke {
class FontLoader {
	Microsoft::WRL::ComPtr<IDWriteFactory5> writeFactory = nullptr;
	Microsoft::WRL::ComPtr<IDWriteInMemoryFontFileLoader> fontFileLoader = nullptr;
	Microsoft::WRL::ComPtr<IDWriteFontSetBuilder1> fontSetBuilder = nullptr;
	Microsoft::WRL::ComPtr<IDWriteFontCollection1> fontCollection = nullptr;

public:
	~FontLoader();

	void preInit();

	void loadFont(const void* data, size_t size);

	void init();

	IDWriteTextFormat* createTextFormat(std::wstring const& fontFamily, int32_t fontSize, kke::FontWeight weight);

	IDWriteTextLayout* createTextLayout(std::wstring const& text, IDWriteTextFormat* textFormat);
};
};	// namespace kke
