#pragma once

#include <string>

#include "kke/appearance/Text.hh"
#include "kke/appearance/resource/font/FontAppearance.hh"
#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
class FontFactory {
public:
	static Microsoft::WRL::ComPtr<IDWriteTextFormat> createTextFormat(
		IDWriteFactory5* writeFactory,
		IDWriteFontCollection1* fontCollection,
		FontAppearance const& appearance);

	static Microsoft::WRL::ComPtr<IDWriteTextLayout> createTextLayout(
		IDWriteFactory5* writeFactory,
		IDWriteTextFormat* textFormat,
		Text const& text);

private:
	static DWRITE_FONT_WEIGHT toDWriteWeight(FontWeight weight);
};
}	// namespace kke
