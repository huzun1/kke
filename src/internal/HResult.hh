#pragma once

#include <winerror.h>

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace kke::internal {
inline std::runtime_error makeHResultError(std::string_view message, HRESULT result) {
	std::ostringstream stream;
	stream << message << " (HRESULT=0x"
		   << std::uppercase
		   << std::hex
		   << static_cast<uint32_t>(result)
		   << ")";
	return std::runtime_error(stream.str());
}

inline void throwIfFailed(HRESULT result, std::string_view message) {
	if (FAILED(result)) {
		throw makeHResultError(message, result);
	}
}
}  // namespace kke::internal
