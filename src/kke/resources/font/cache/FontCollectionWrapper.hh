#pragma once

#include <dwrite.h>
#include <dwrite_3.h>
#include <dxgi1_2.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

#include <expected>
#include <string>

#include "kke/resources/font/FontWeight.hh"

#pragma comment(lib, "dwrite.lib")

using namespace Microsoft::WRL;

class FontCollectionWrapper {
	ComPtr<IDWriteFactory5> writeFactory = nullptr;
	ComPtr<IDWriteFontCollection1> fontCollection = nullptr;

public:
	std::expected<ComPtr<IDWriteTextFormat>, std::string> createTextFormat(std::wstring const& fontFamily, float fontSize, kke::FontWeight weight);

	std::expected<ComPtr<IDWriteTextLayout>, std::string> createTextLayout(std::wstring const& text, IDWriteTextFormat* textFormat);

private:
	std::expected<DWRITE_FONT_WEIGHT, std::string> convertFontWeight(kke::FontWeight weight);
};
