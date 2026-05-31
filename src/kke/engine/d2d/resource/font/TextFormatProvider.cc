#include "TextFormatProvider.hh"

#include "factory/FontFactory.hh"
#include "hash/FontHasher.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

ComPtr<IDWriteTextFormat> TextFormatProvider::get(
	IDWriteFactory5* writeFactory,
	IDWriteFontCollection1* fontCollection,
	FontAppearance const& appearance
) {
	uint64_t key = FontHasher::hash(appearance);

	ComPtr<IDWriteTextFormat> cachedFormat = storage.get(key);
	if (cachedFormat) {
		return cachedFormat;
	}

	ComPtr<IDWriteTextFormat> createdFormat =
		FontFactory::createTextFormat(writeFactory, fontCollection, appearance);
	if (!createdFormat) {
		return nullptr;
	}

	storage.put(key, createdFormat);
	return createdFormat;
}

void TextFormatProvider::clear() {
	storage.clear();
}
