#include "FontWeight.hh"

namespace kke {
DWRITE_FONT_WEIGHT toDWriteWeight(FontWeight weight) {
	switch (weight) {
		case FontWeight::LIGHT:
			return DWRITE_FONT_WEIGHT_LIGHT;
		case FontWeight::NORMAL:
			return DWRITE_FONT_WEIGHT_NORMAL;
		case FontWeight::MEDIUM:
			return DWRITE_FONT_WEIGHT_MEDIUM;
		case FontWeight::SEMI_BOLD:
			return DWRITE_FONT_WEIGHT_SEMI_BOLD;
		case FontWeight::BOLD:
			return DWRITE_FONT_WEIGHT_BOLD;
		default:
			return DWRITE_FONT_WEIGHT_NORMAL;
	}
}
}  // namespace kke