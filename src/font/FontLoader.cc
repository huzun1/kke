#include "kke/font/FontLoader.hh"

#include <dwrite.h>
#include <winnt.h>

#include <cfloat>
#include <stdexcept>

#include "../internal/HResult.hh"

using Microsoft::WRL::ComPtr;
using kke::internal::throwIfFailed;

using namespace kke;

FontLoader::~FontLoader() {
	if (writeFactory && fontFileLoader) {
		writeFactory->UnregisterFontFileLoader(fontFileLoader.Get());
	}
}

void FontLoader::preInit() {
	throwIfFailed(
		DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory5),
			reinterpret_cast<IUnknown**>(writeFactory.ReleaseAndGetAddressOf())),
		"Failed to create DirectWrite factory");
	throwIfFailed(
		writeFactory->CreateFontSetBuilder(fontSetBuilder.ReleaseAndGetAddressOf()),
		"Failed to create DirectWrite font set builder");
	throwIfFailed(
		writeFactory->CreateInMemoryFontFileLoader(fontFileLoader.ReleaseAndGetAddressOf()),
		"Failed to create in-memory font file loader");
	throwIfFailed(
		writeFactory->RegisterFontFileLoader(fontFileLoader.Get()),
		"Failed to register in-memory font file loader");
}

void FontLoader::loadFont(const void* data, size_t size) {
	ComPtr<IDWriteFontFile> fontFile;
	throwIfFailed(
		fontFileLoader->CreateInMemoryFontFileReference(
			writeFactory.Get(),
			data,
			size,
			nullptr,
			fontFile.ReleaseAndGetAddressOf()),
		"Failed to create in-memory font reference");
	throwIfFailed(
		fontSetBuilder->AddFontFile(fontFile.Get()),
		"Failed to add font to font set builder");
}

void FontLoader::init() {
	ComPtr<IDWriteFontSet> fontSet;
	throwIfFailed(
		fontSetBuilder->CreateFontSet(fontSet.ReleaseAndGetAddressOf()),
		"Failed to create DirectWrite font set");
	throwIfFailed(
		writeFactory->CreateFontCollectionFromFontSet(fontSet.Get(), fontCollection.ReleaseAndGetAddressOf()),
		"Failed to create DirectWrite font collection");
}

IDWriteTextFormat* FontLoader::createTextFormat(std::wstring const& fontFamily, int32_t fontSize, kke::FontWeight weight) {
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
			throw std::runtime_error("Unsupported font weight.");
	}

	ComPtr<IDWriteTextFormat> textFormat;
	throwIfFailed(
		writeFactory->CreateTextFormat(
			fontFamily.c_str(),
			fontCollection.Get(),
			dwriteWeight,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			static_cast<FLOAT>(fontSize),
			L"",
			textFormat.ReleaseAndGetAddressOf()),
		"Failed to create DirectWrite text format");
	return textFormat.Detach();
}

IDWriteTextLayout* FontLoader::createTextLayout(std::wstring const& text, IDWriteTextFormat* textFormat) {
	ComPtr<IDWriteTextLayout> textLayout;
	throwIfFailed(
		writeFactory->CreateTextLayout(
			text.c_str(),
			static_cast<UINT32>(text.length()),
			textFormat,
			FLT_MAX,
			FLT_MAX,
			textLayout.ReleaseAndGetAddressOf()),
		"Failed to create DirectWrite text layout");
	return textLayout.Detach();
}
