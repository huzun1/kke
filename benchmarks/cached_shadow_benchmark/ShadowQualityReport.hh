#pragma once

#include <cstdint>
#include <span>
#include <string_view>

namespace kke::benchmark {
class ShadowQualityReport {
  public:
	static bool write(
		std::span<uint8_t const> reference,
		std::span<uint8_t const> pixels,
		std::string_view scene,
		std::string_view mode
	);
};
} // namespace kke::benchmark
