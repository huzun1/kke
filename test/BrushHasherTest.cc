#include <vector>

#include "kke/appearance/Color.hh"
#include "kke/appearance/resource/brush/impl/LinearGradientBrush.hh"
#include "kke/engine/d2d/resource/brush/hash/BrushHasher.hh"

int main() {
	std::vector<kke::Color> colors{
		{1.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f, 1.0f},
	};
	kke::LinearGradientBrush horizontal(colors, {0.0f, 0.0f}, {100.0f, 0.0f});
	kke::LinearGradientBrush vertical(colors, {0.0f, 0.0f}, {0.0f, 100.0f});

	if (kke::BrushHasher::hashGradientStops(horizontal) !=
		kke::BrushHasher::hashGradientStops(vertical)) {
		return 1;
	}
	if (kke::BrushHasher::hash(kke::Brush(horizontal)) ==
		kke::BrushHasher::hash(kke::Brush(vertical))) {
		return 1;
	}

	colors[1] = {0.0f, 1.0f, 0.0f, 1.0f};
	kke::LinearGradientBrush differentColors(colors, {0.0f, 0.0f}, {100.0f, 0.0f});
	if (kke::BrushHasher::hashGradientStops(horizontal) ==
		kke::BrushHasher::hashGradientStops(differentColors)) {
		return 1;
	}

	return 0;
}
