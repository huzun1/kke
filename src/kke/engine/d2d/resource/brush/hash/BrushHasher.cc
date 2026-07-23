#include "BrushHasher.hh"

#include <variant>

#include "BrushHashTag.hh"
#include "kke/appearance/Color.hh"
#include "kke/appearance/ColorQuantizer.hh"
#include "kke/utils/Hasher.hh"

using namespace kke;

uint64_t BrushHasher::hash(Brush const& brush) {
	return std::visit([](auto const& brushVariant) { return hash(brushVariant); }, brush);
}

uint64_t BrushHasher::hash(SolidColorBrush const& brush) {
	Color const& color = brush.getColor();

	Hasher hasher;
	hasher.combine(BrushHashTag::SolidColor);
	hasher.combine(ColorQuantizer::quantizeComponent(color.r));
	hasher.combine(ColorQuantizer::quantizeComponent(color.g));
	hasher.combine(ColorQuantizer::quantizeComponent(color.b));
	hasher.combine(ColorQuantizer::quantizeComponent(color.a));
	return hasher.get();
}

uint64_t BrushHasher::hash(LinearGradientBrush const& brush) {
	Hasher hasher;
	hasher.combine(BrushHashTag::LinearGradient);

	for (Color const& color : brush.getColors()) {
		hasher.combine(ColorQuantizer::quantizeComponent(color.r));
		hasher.combine(ColorQuantizer::quantizeComponent(color.g));
		hasher.combine(ColorQuantizer::quantizeComponent(color.b));
		hasher.combine(ColorQuantizer::quantizeComponent(color.a));
	}

	hasher.combine(brush.getStartPoint().x);
	hasher.combine(brush.getStartPoint().y);
	hasher.combine(brush.getEndPoint().x);
	hasher.combine(brush.getEndPoint().y);
	hasher.combine(brush.getAngle());
	return hasher.get();
}
