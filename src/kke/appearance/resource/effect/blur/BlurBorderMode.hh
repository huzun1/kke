#pragma once

namespace kke {
enum class BlurBorderMode {
	SOFT,  // If the crop rectangle falls on fractional pixel coordinates, the effect applies antialiasing which results in a soft edge.
	HARD   // If the crop rectangle falls on fractional pixel coordinates, the effect clamps which results in a hard edge.
};
};
