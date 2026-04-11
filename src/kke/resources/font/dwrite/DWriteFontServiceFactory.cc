#include "DWriteFontServiceFactory.hh"

#include <wrl/client.h>

std::expected<std::shared_ptr<kke::DWriteFontService>, std::string> DWriteFontServiceFactory::create(ComPtr<IDWriteFactory5> writeFactory) {
    ComPtr<IDWriteFontSetBuilder1> fontSetBuilder;
	if (FAILED(writeFactory->CreateFontSetBuilder(&fontSetBuilder))) {
		return std::unexpected("couldn't create a font set builder");
	}

    ComPtr<IDWriteInMemoryFontFileLoader> fontFileLoader;
	if (FAILED(writeFactory->CreateInMemoryFontFileLoader(&fontFileLoader))) {
		return std::unexpected("couldn't create in memory font file loader");
	}

	if (FAILED(writeFactory->RegisterFontFileLoader(fontFileLoader.Get()))) {
		return std::unexpected("couldn't register font file loader");
	}

	return std::make_shared<kke::DWriteFontService>(writeFactory, fontFileLoader, fontSetBuilder);
}
