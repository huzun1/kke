#include <d2d1.h>

#include <oreik/brush/SolidColorBrush.hpp>
#include <oreik/internal/Hasher.hpp>

oreik::SolidColorBrush::SolidColorBrush(oreik::Color4f color)
	: color(color) {
}

ID2D1Brush* oreik::SolidColorBrush::create(ID2D1DeviceContext* context) const {
	ID2D1SolidColorBrush* result;
	D2D1_COLOR_F brushColor{color.r, color.g, color.b, color.a};
	context->CreateSolidColorBrush(&brushColor, nullptr, &result);

	return result;
}

uint64_t oreik::SolidColorBrush::hash() const {
	oreik::Hasher hasher;
	hasher.combine(color.r);
	hasher.combine(color.g);
	hasher.combine(color.b);
	hasher.combine(color.a);
	return hasher.get();
}
