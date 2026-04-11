#pragma once

#include <dwrite.h>
#include <dwrite_3.h>
#include <wrl/client.h>

#include <cstdint>
#include <vector>

using Microsoft::WRL::ComPtr;

namespace kke {
class DWriteFontService {
private:
	ComPtr<IDWriteFactory5> writeFactory;
	ComPtr<IDWriteInMemoryFontFileLoader> fontFileLoader;
	ComPtr<IDWriteFontSetBuilder1> fontSetBuilder;

	std::vector<std::vector<uint8_t>> fontDataStorage;
	bool isRegistered = false;

public:
	DWriteFontService(ComPtr<IDWriteFactory5> writeFactory,
					  ComPtr<IDWriteInMemoryFontFileLoader> fontFileLoader,
					  ComPtr<IDWriteFontSetBuilder1> fontSetBuilder);

	~DWriteFontService();

	void addFont(const void* data, size_t size);

	ComPtr<IDWriteFontCollection> createCollection();
};
}  // namespace kke
