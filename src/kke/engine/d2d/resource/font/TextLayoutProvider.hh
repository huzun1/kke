#pragma once

#include "TextFormatProvider.hh"
#include "kke/appearance/Text.hh"
#include "kke/engine/d2d/d2d1_headers.hh"
#include "kke/engine/d2d/resource/KeyCacheStorage.hh"

namespace kke {
class TextLayoutProvider {
	KeyCacheStorage<IDWriteTextLayout> storage;

public:
	Microsoft::WRL::ComPtr<IDWriteTextLayout> get(
		IDWriteFactory5* writeFactory,
		IDWriteFontCollection1* fontCollection,
		TextFormatProvider& textFormatProvider,
		Text const& text);

	void clear();
};
}	// namespace kke
