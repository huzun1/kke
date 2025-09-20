#include <d2d1.h>

#include <oreik/brush/SolidColorBrush.hpp>
#include <oreik/internal/Hasher.hpp>

oreik::SolidColorBrush::SolidColorBrush(oreik::Color4f color)
	: color(color) {
}

void oreik::SolidColorBrush::create(ID2D1DeviceContext* context, ID2D1Brush** output) const {
	D2D1_COLOR_F brushColor{color.r, color.g, color.b, color.a};
	context->CreateSolidColorBrush(&brushColor, nullptr, reinterpret_cast<ID2D1SolidColorBrush**>(output));
}

uint64_t oreik::SolidColorBrush::hash() const {
	oreik::Hasher hash;
	hash.combine(static_cast<uint32_t>(oreik::BrushType::SOLID_BRUSH));
	hash.combine(color.r);
	hash.combine(color.g);
	hash.combine(color.b);
	hash.combine(color.a);
	return hash.get();
}
