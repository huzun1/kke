#include "FontCollectionWrapper.hh"

std::expected<ComPtr<IDWriteTextFormat>, std::string> FontCollectionWrapper::createTextFormat(std::wstring const& fontFamily, float fontSize, kke::FontWeight weight) {
	ComPtr<IDWriteTextFormat> textFormat;

	auto dwriteWeight = convertFontWeight(weight);
	if (!dwriteWeight) {
		return std::unexpected(dwriteWeight.error());
	}

	HRESULT result = writeFactory->CreateTextFormat(
		fontFamily.c_str(),
		fontCollection.Get(),
		dwriteWeight.value(),
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"",  // locale
		&textFormat);
	if (FAILED(result)) {
		return std::unexpected(std::string("couldn't create text format: {:x}", result));
	}
	return textFormat;
}

std::expected<ComPtr<IDWriteTextLayout>, std::string> FontCollectionWrapper::createTextLayout(std::wstring const& text, IDWriteTextFormat* textFormat) {
	ComPtr<IDWriteTextLayout> textLayout;
	if (FAILED(writeFactory->CreateTextLayout(
			text.c_str(),
			text.length(),
			textFormat,
			FLT_MAX,
			FLT_MAX,
			&textLayout))) {
		return std::unexpected(std::string("couldn't create text layout"));
	}
	return textLayout;
}

std::expected<DWRITE_FONT_WEIGHT, std::string> FontCollectionWrapper::convertFontWeight(kke::FontWeight weight) {
	DWRITE_FONT_WEIGHT dwriteWeight = DWRITE_FONT_WEIGHT_NORMAL;
	switch (weight) {
		case kke::FontWeight::LIGHT:
			return DWRITE_FONT_WEIGHT_LIGHT;
		case kke::FontWeight::NORMAL:
			return DWRITE_FONT_WEIGHT_NORMAL;
		case kke::FontWeight::MEDIUM:
			return DWRITE_FONT_WEIGHT_MEDIUM;
		case kke::FontWeight::SEMI_BOLD:
			return DWRITE_FONT_WEIGHT_SEMI_BOLD;
		case kke::FontWeight::BOLD:
			return DWRITE_FONT_WEIGHT_BOLD;
		default:
			return std::unexpected(std::string("invalid font weight"));
	}
}
