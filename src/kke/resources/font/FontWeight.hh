#pragma once

#include <dwrite.h>

namespace kke {
enum class FontWeight {
	LIGHT,
	NORMAL,
	MEDIUM,
	SEMI_BOLD,
	BOLD
};

DWRITE_FONT_WEIGHT toDWriteWeight(FontWeight weight);

}  // namespace kke