#include "SolidColorBrush.hh"

using namespace kke;

SolidColorBrush::SolidColorBrush(Color color) : color(color) {
}

Color const& SolidColorBrush::getColor() const {
	return color;
}
