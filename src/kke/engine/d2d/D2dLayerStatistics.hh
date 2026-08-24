#pragma once

#include <cstddef>

namespace kke {

struct D2dLayerStatistics {
	std::size_t axisAlignedClipCount = 0;
	std::size_t geometricLayerCount = 0;
	std::size_t maximumDepth = 0;
};

} // namespace kke
