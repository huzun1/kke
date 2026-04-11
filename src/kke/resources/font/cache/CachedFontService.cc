#include "CachedFontService.hh"

#include "kke/utils/Hasher.hh"

CachedFontService::CachedFontService(std::shared_ptr<FontCollectionWrapper> fontCollectionWrapper) : fontCollectionWrapper(fontCollectionWrapper) {
}

std::expected<ComPtr<IDWriteTextFormat>, std::string> CachedFontService::getOrCreateTextFormat(std::wstring const& fontFamily, float fontSize, kke::FontWeight weight) {
    kke::Hasher hasher;
    hasher.combine(fontFamily);
    hasher.combine(fontSize);
    hasher.combine(weight);
    uint64_t key = hasher.get();

    auto cached = textFormatCache.get(key);
    if (cached) {
        return cached;
    }

    auto textFormat = fontCollectionWrapper->createTextFormat(fontFamily, fontSize, weight);
    if (!textFormat) {
        return std::unexpected(textFormat.error());
    }

    textFormatCache.put(key, textFormat.value());

    return textFormat.value();
}

std::expected<ComPtr<IDWriteTextLayout>, std::string> CachedFontService::getOrCreateTextLayout(std::wstring const& text, IDWriteTextFormat* textFormat) {
    kke::Hasher hasher;
    hasher.combine(text);
    hasher.combine((uintptr_t)textFormat);
    uint64_t key = hasher.get();

    auto cached = textLayoutCache.get(key);
    if (cached) {
        return cached;
    }

    auto textLayout = fontCollectionWrapper->createTextLayout(text, textFormat);
    if (!textLayout) {
        return std::unexpected(textLayout.error());
    }

    textLayoutCache.put(key, textLayout.value());

    return textLayout.value();
}
