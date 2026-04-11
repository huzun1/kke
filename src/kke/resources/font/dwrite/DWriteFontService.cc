#include "DWriteFontService.hh"

#include <stdexcept>

namespace kke {
DWriteFontService::DWriteFontService(ComPtr<IDWriteFactory5> writeFactory,
									 ComPtr<IDWriteInMemoryFontFileLoader> fontFileLoader,
									 ComPtr<IDWriteFontSetBuilder1> fontSetBuilder)
	: writeFactory(std::move(writeFactory)),
	  fontFileLoader(std::move(fontFileLoader)),
	  fontSetBuilder(std::move(fontSetBuilder)) {
}

DWriteFontService::~DWriteFontService() {
	if (isRegistered && writeFactory && fontFileLoader) {
		writeFactory->UnregisterFontFileLoader(fontFileLoader.Get());
	}
}

void DWriteFontService::addFont(const void* data, size_t size) {
	if (!data || size == 0) {
		return;
	}

	const auto* bytes = static_cast<const uint8_t*>(data);
	fontDataStorage.emplace_back(bytes, bytes + size);
	const auto& blob = fontDataStorage.back();

	Microsoft::WRL::ComPtr<IDWriteFontFile> fontFile;
	if (FAILED(fontFileLoader->CreateInMemoryFontFileReference(
			writeFactory.Get(),
			blob.data(),
			blob.size(),
			nullptr,
			&fontFile))) {
		return;
	}

	fontSetBuilder->AddFontFile(fontFile.Get());
}

ComPtr<IDWriteFontCollection> DWriteFontService::createCollection() {
	if (!fontSetBuilder || !writeFactory) {
		throw std::runtime_error("font wrapper is not initialized");
	}

	Microsoft::WRL::ComPtr<IDWriteFontSet> fontSet;
	if (FAILED(fontSetBuilder->CreateFontSet(&fontSet))) {
		throw std::runtime_error("couldn't create a font set from the builder");
	}

	ComPtr<IDWriteFontCollection1> fontCollection;
	if (FAILED(writeFactory->CreateFontCollectionFromFontSet(fontSet.Get(), &fontCollection))) {
		throw std::runtime_error("couldn't create a font collection from the font set");
	}

	return fontCollection;
}

}  // namespace kke
