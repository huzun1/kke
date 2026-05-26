#pragma once

#include <cstdint>
#include <string>

#include "kke/appearance/Text.hh"
#include "kke/appearance/resource/font/FontAppearance.hh"

namespace kke {
class FontHasher {
public:
	static uint64_t hash(FontAppearance const& appearance);

	static uint64_t hash(Text const& text);

	static std::wstring getTextString(Text const& text);

private:
	static std::wstring toWideString(std::string const& text);
};
}	// namespace kke
