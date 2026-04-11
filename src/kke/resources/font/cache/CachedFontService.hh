#pragma once

#include <expected>
#include <memory>

#include "kke/resources/CacheStorage.hh"
#include "kke/resources/font/cache/FontCollectionWrapper.hh"

using namespace Microsoft::WRL;

class CachedFontService {
    std::shared_ptr<FontCollectionWrapper> fontCollectionWrapper;
   
    kke::CacheStorage<IDWriteTextFormat> textFormatCache;
    kke::CacheStorage<IDWriteTextLayout> textLayoutCache;

public:
    CachedFontService(std::shared_ptr<FontCollectionWrapper> fontCollectionWrapper);

    std::expected<ComPtr<IDWriteTextFormat>, std::string> getOrCreateTextFormat(std::wstring const& fontFamily, float fontSize, kke::FontWeight weight);

    std::expected<ComPtr<IDWriteTextLayout>, std::string> getOrCreateTextLayout(std::wstring const& text, IDWriteTextFormat* textFormat);
};
