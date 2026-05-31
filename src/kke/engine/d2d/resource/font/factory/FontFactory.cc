#include "FontFactory.hh"

#include <cstdio>

#include "kke/engine/d2d/resource/font/hash/FontHasher.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

ComPtr<IDWriteTextFormat> FontFactory::createTextFormat(
	IDWriteFactory5* writeFactory,
	IDWriteFontCollection1* fontCollection,
	FontAppearance const& appearance
) {
	if (!writeFactory) {
		return nullptr;
	}

	std::wstring fontFamily(appearance.fontFamily.begin(), appearance.fontFamily.end());
	ComPtr<IDWriteTextFormat> textFormat;
	HRESULT result = writeFactory->CreateTextFormat(
		fontFamily.c_str(),
		fontCollection,
		toDWriteWeight(appearance.weight),
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		appearance.fontSize,
		L"",
		&textFormat
	);
	if (FAILED(result)) {
		std::printf(
			"[kke][FontFactory] CreateTextFormat failed: 0x%08lx\n",
			static_cast<unsigned long>(result)
		);
		return nullptr;
	}

	return textFormat;
}

ComPtr<IDWriteTextLayout> FontFactory::createTextLayout(
	IDWriteFactory5* writeFactory, IDWriteTextFormat* textFormat, Text const& text
) {
	if (!writeFactory || !textFormat) {
		return nullptr;
	}

	std::wstring textString = FontHasher::getTextString(text);
	ComPtr<IDWriteTextLayout> textLayout;
	HRESULT result = writeFactory->CreateTextLayout(
		textString.c_str(),
		static_cast<UINT32>(textString.size()),
		textFormat,
		100000.0f,
		100000.0f,
		&textLayout
	);
	if (FAILED(result)) {
		std::printf(
			"[kke][FontFactory] CreateTextLayout failed: 0x%08lx\n",
			static_cast<unsigned long>(result)
		);
		return nullptr;
	}

	return textLayout;
}

DWRITE_FONT_WEIGHT FontFactory::toDWriteWeight(FontWeight weight) {
	switch (weight) {
	case FontWeight::LIGHT:
		return DWRITE_FONT_WEIGHT_LIGHT;
	case FontWeight::NORMAL:
		return DWRITE_FONT_WEIGHT_NORMAL;
	case FontWeight::MEDIUM:
		return DWRITE_FONT_WEIGHT_MEDIUM;
	case FontWeight::SEMI_BOLD:
		return DWRITE_FONT_WEIGHT_SEMI_BOLD;
	case FontWeight::BOLD:
		return DWRITE_FONT_WEIGHT_BOLD;
	default:
		return DWRITE_FONT_WEIGHT_NORMAL;
	}
}
