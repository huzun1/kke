#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include "kke/appearance/resource/font/Font.hh"

namespace kke {
class D2dFont : public Font {
	std::vector<uint8_t> data;

public:
	D2dFont(void const* data, size_t size);

	std::span<uint8_t const> getData() const;
};
}	// namespace kke
