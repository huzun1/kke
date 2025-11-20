#include "kke/font/FontLoader.hh"

#include <dwrite.h>

#include <stdexcept>

using namespace kke;

FontLoader::~FontLoader() {
	if (writeFactory && fontFileLoader) {
		writeFactory->UnregisterFontFileLoader(fontFileLoader.Get());
	}
}

void FontLoader::preInit() {
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), reinterpret_cast<IUnknown**>(writeFactory.GetAddressOf())))) {
		throw std::runtime_error("couldn't create dwrite factory");
	}
	if (FAILED(writeFactory->CreateFontSetBuilder(&fontSetBuilder))) {
		throw std::runtime_error("couldn't create a font set builder");
	}
	if (FAILED(writeFactory->CreateInMemoryFontFileLoader(&fontFileLoader))) {
		throw std::runtime_error("couldn't create in memory font file loader");
	}
	if (FAILED(writeFactory->RegisterFontFileLoader(fontFileLoader.Get()))) {
		throw std::runtime_error("couldn't register font file loader");
	}
}

void FontLoader::loadFont(const void* data, size_t size) {
	IDWriteFontFile* fontFile;
	if (FAILED(fontFileLoader->CreateInMemoryFontFileReference(writeFactory.Get(), data, size, nullptr, &fontFile))) {
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

IDWriteTextFormat* FontLoader::createTextFormat(std::wstring const& fontFamily, int32_t fontSize, kke::FontWeight weight) {
	IDWriteTextFormat* textFormat;
	DWRITE_FONT_WEIGHT dwriteWeight = DWRITE_FONT_WEIGHT_NORMAL;
	switch (weight) {
		case kke::FontWeight::LIGHT:
			dwriteWeight = DWRITE_FONT_WEIGHT_LIGHT;
			break;
		case kke::FontWeight::NORMAL:
			dwriteWeight = DWRITE_FONT_WEIGHT_NORMAL;
			break;
		case kke::FontWeight::MEDIUM:
			dwriteWeight = DWRITE_FONT_WEIGHT_MEDIUM;
			break;
		case kke::FontWeight::SEMI_BOLD:
			dwriteWeight = DWRITE_FONT_WEIGHT_SEMI_BOLD;
			break;
		case kke::FontWeight::BOLD:
			dwriteWeight = DWRITE_FONT_WEIGHT_BOLD;
			break;
		default:
			throw std::runtime_error("unsupported font weight");
	}
	if (FAILED(writeFactory->CreateTextFormat(
			fontFamily.c_str(),
			fontCollection.Get(),
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
