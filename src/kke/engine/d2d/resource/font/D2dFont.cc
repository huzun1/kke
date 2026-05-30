#include "D2dFont.hh"

#include <cstring>

using namespace kke;

D2dFont::D2dFont(void const* data, size_t size)
	: data(size) {
	std::memcpy(this->data.data(), data, size);
}

std::span<uint8_t const> D2dFont::getData() const {
	return data;
}
