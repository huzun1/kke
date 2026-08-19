#include "FontProvider.hh"

#include <optional>

#include "hash/FontHasher.hh"
#include "kke/utils/DebugLog.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

FontProvider::FontProvider() {
	initialize();
}

FontProvider::~FontProvider() {
	if (writeFactory && fontFileLoader) {
		writeFactory->UnregisterFontFileLoader(fontFileLoader.Get());
	}
}

std::shared_ptr<D2dFont> FontProvider::uploadFont(void const* data, size_t size) {
	if (!data || size == 0) {
		return nullptr;
	}

	std::shared_ptr<D2dFont> font = std::make_shared<D2dFont>(data, size);
	fonts.push_back(font);

	ComPtr<IDWriteFontFile> fontFile;
	HRESULT result = fontFileLoader->CreateInMemoryFontFileReference(
		writeFactory.Get(),
		font->getData().data(),
		static_cast<UINT32>(font->getData().size()),
		nullptr,
		&fontFile
	);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][FontProvider] CreateInMemoryFontFileReference failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		fonts.pop_back();
		return nullptr;
	}

	fontFiles.push_back(fontFile);
	rebuildFontCollection();
	clearTextCaches();
	return font;
}

ComPtr<IDWriteTextFormat> FontProvider::createTextFormat(FontAppearance const& appearance) {
	return textFormatProvider.get(writeFactory.Get(), fontCollection.Get(), appearance);
}

ComPtr<IDWriteTextLayout> FontProvider::createTextLayout(Text const& text) {
	return textLayoutProvider
		.get(writeFactory.Get(), fontCollection.Get(), textFormatProvider, text);
}

Scale FontProvider::measureTextSize(Text const& text) {
	uint64_t textKey = FontHasher::hash(text);
	if (std::optional<Scale> cachedSize = textMeasurementCache.get(textKey)) {
		return *cachedSize;
	}

	ComPtr<IDWriteTextLayout> textLayout = createTextLayout(text);
	if (!textLayout) {
		return {0.0f, 0.0f};
	}

	DWRITE_TEXT_METRICS metrics = {};
	HRESULT result = textLayout->GetMetrics(&metrics);
	if (FAILED(result)) {
		return {0.0f, 0.0f};
	}

	Scale size{metrics.widthIncludingTrailingWhitespace, metrics.height};
	textMeasurementCache.put(textKey, size);
	return size;
}

std::vector<std::shared_ptr<D2dFont>> const& FontProvider::getFonts() const {
	return fonts;
}

void FontProvider::initialize() {
	HRESULT result = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory5),
		reinterpret_cast<IUnknown**>(writeFactory.GetAddressOf())
	);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][FontProvider] DWriteCreateFactory failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return;
	}

	result = writeFactory->CreateInMemoryFontFileLoader(&fontFileLoader);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][FontProvider] CreateInMemoryFontFileLoader failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return;
	}

	result = writeFactory->RegisterFontFileLoader(fontFileLoader.Get());
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][FontProvider] RegisterFontFileLoader failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return;
	}

	result = writeFactory->CreateFontSetBuilder(&fontSetBuilder);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][FontProvider] CreateFontSetBuilder failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
	}
}

void FontProvider::rebuildFontCollection() {
	if (!writeFactory) {
		return;
	}

	ComPtr<IDWriteFontSetBuilder1> builder;
	HRESULT result = writeFactory->CreateFontSetBuilder(&builder);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][FontProvider] CreateFontSetBuilder failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return;
	}

	for (ComPtr<IDWriteFontFile> const& fontFile : fontFiles) {
		result = builder->AddFontFile(fontFile.Get());
		if (FAILED(result)) {
			kke::debug::log(
				"[kke][FontProvider] AddFontFile failed: 0x%08x",
				static_cast<unsigned int>(result)
			);
			return;
		}
	}

	ComPtr<IDWriteFontSet> fontSet;
	result = builder->CreateFontSet(&fontSet);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][FontProvider] CreateFontSet failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return;
	}

	result = writeFactory->CreateFontCollectionFromFontSet(fontSet.Get(), &fontCollection);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][FontProvider] CreateFontCollectionFromFontSet failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
	}
}

void FontProvider::clearTextCaches() {
	textMeasurementCache.clear();
	textLayoutProvider.clear();
	textFormatProvider.clear();
}
