#include "oreik/font/FontLoader.hpp"

#include <dwrite.h>

#include <stdexcept>

using namespace oreik;

void FontLoader::preInit() {
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), reinterpret_cast<IUnknown**>(&writeFactory)))) {
		throw std::runtime_error("couldn't create dwrite factory");
	}
	if (FAILED(writeFactory->CreateFontSetBuilder(&fontSetBuilder))) {
		throw std::runtime_error("couldn't create a font set builder");
	}
	if (FAILED(writeFactory->CreateInMemoryFontFileLoader(&fontFileLoader))) {
		throw std::runtime_error("couldn't create a font set builder");
	}
}

void FontLoader::loadFont(const void* data, size_t size) {
	IDWriteFontFile* fontFile;
	if (FAILED(fontFileLoader->CreateInMemoryFontFileReference(writeFactory, data, size, nullptr, &fontFile))) {
		return;
	}
	fontSetBuilder->AddFontFile(fontFile);
}

void FontLoader::init() {
	IDWriteFontSet* fontSet;
	if (FAILED(fontSetBuilder->CreateFontSet(&fontSet))) {
		throw std::runtime_error("couldn't create a font set from the builder");
	}
	if (FAILED(writeFactory->CreateFontCollectionFromFontSet(fontSet, &fontCollection))) {
		throw std::runtime_error("couldn't create a font collection from the font set");
	}
}

IDWriteTextFormat* FontLoader::createTextFormat(std::wstring const& fontFamily, int32_t fontSize, oreik::FontWeight weight) {
	IDWriteTextFormat* textFormat;
	DWRITE_FONT_WEIGHT dwriteWeight = DWRITE_FONT_WEIGHT_NORMAL;
	switch (weight) {
		case oreik::FontWeight::LIGHT:
			dwriteWeight = DWRITE_FONT_WEIGHT_LIGHT;
			break;
		case oreik::FontWeight::NORMAL:
			dwriteWeight = DWRITE_FONT_WEIGHT_NORMAL;
			break;
		case oreik::FontWeight::MEDIUM:
			dwriteWeight = DWRITE_FONT_WEIGHT_MEDIUM;
			break;
		case oreik::FontWeight::BOLD:
			dwriteWeight = DWRITE_FONT_WEIGHT_BOLD;
			break;
		default:
			throw std::runtime_error("unsupported font weight");
	}
	if (FAILED(writeFactory->CreateTextFormat(
			fontFamily.c_str(),
			fontCollection,
			dwriteWeight,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			static_cast<FLOAT>(fontSize),
			L"",  // locale
			&textFormat))) {
		throw std::runtime_error("couldn't create text format");
	}
	return textFormat;
}

IDWriteTextLayout* FontLoader::createTextLayout(std::wstring const& text, IDWriteTextFormat* textFormat) {
	IDWriteTextLayout* textLayout;
	if (FAILED(writeFactory->CreateTextLayout(
			text.c_str(),
			text.length(),
			textFormat,
			FLT_MAX,
			FLT_MAX,
			&textLayout))) {
		throw std::runtime_error("couldn't create text layout");
	}
	return textLayout;
}
