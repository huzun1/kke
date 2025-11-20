#pragma once

#include <cstdint>
#include <cstring>
#include <type_traits>

namespace kke {

class Hasher {
public:
	explicit Hasher(uint64_t seed = 0)
		: value(seed) {
	}

	template <typename T>
	void combine(const T& v) {
		if constexpr (std::is_integral_v<T>) {
			mix(static_cast<uint64_t>(v));
		} else if constexpr (std::is_floating_point_v<T>) {
			uint64_t bits = 0;
			static_assert(sizeof(T) <= sizeof(uint64_t));
			std::memcpy(&bits, &v, sizeof(T));
			mix(bits);
		} else {
			std::hash<T> hasher;
			mix(static_cast<uint64_t>(hasher(v)));
		}
	}

	uint64_t get() const {
		return value;
	}

private:
	uint64_t value;

	void mix(uint64_t hv) {
		value ^= hv + 0x9e3779b97f4a7c15ULL + (value << 6) + (value >> 2);
	}
};
}  // namespace kke
