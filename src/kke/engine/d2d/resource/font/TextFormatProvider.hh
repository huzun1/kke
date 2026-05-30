#pragma once

#include "kke/appearance/resource/font/FontAppearance.hh"
#include "kke/engine/d2d/d2d1_headers.hh"
#include "kke/engine/d2d/resource/KeyCacheStorage.hh"

namespace kke {
class TextFormatProvider {
	KeyCacheStorage<IDWriteTextFormat> storage;

public:
	Microsoft::WRL::ComPtr<IDWriteTextFormat> get(
		IDWriteFactory5* writeFactory,
		IDWriteFontCollection1* fontCollection,
		FontAppearance const& appearance);

	void clear();
};
}	// namespace kke
