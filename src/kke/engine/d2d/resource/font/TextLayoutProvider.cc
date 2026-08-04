#include "TextLayoutProvider.hh"

#include "factory/FontFactory.hh"
#include "hash/FontHasher.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

TextLayoutProvider::TextLayoutProvider(uint32_t limit) : storage(limit) {
}

ComPtr<IDWriteTextLayout> TextLayoutProvider::get(
	IDWriteFactory5* writeFactory,
	IDWriteFontCollection1* fontCollection,
	TextFormatProvider& textFormatProvider,
	Text const& text
) {
	uint64_t key = FontHasher::hash(text);

	ComPtr<IDWriteTextLayout> cachedLayout = storage.get(key);
	if (cachedLayout) {
		return cachedLayout;
	}

	ComPtr<IDWriteTextFormat> textFormat =
		textFormatProvider.get(writeFactory, fontCollection, text.fontAppearance);
	if (!textFormat) {
		return nullptr;
	}

	ComPtr<IDWriteTextLayout> createdLayout =
		FontFactory::createTextLayout(writeFactory, textFormat.Get(), text);
	if (!createdLayout) {
		return nullptr;
	}

	storage.put(key, createdLayout);
	return createdLayout;
}

void TextLayoutProvider::clear() {
	storage.clear();
}
