#include "LinearGradientBrush.hh"

#include <utility>

using namespace kke;

LinearGradientBrush::LinearGradientBrush(
	std::vector<kke::Color> colors,
	kke::Point const& startPoint,
	kke::Point const& endPoint,
	float angle
)
	: colors(std::move(colors)), startPoint(startPoint), endPoint(endPoint), angle(angle) {
}

std::vector<kke::Color> const& LinearGradientBrush::getColors() const {
	return colors;
}

kke::Point const& LinearGradientBrush::getStartPoint() const {
	return startPoint;
}

kke::Point const& LinearGradientBrush::getEndPoint() const {
	return endPoint;
}

float LinearGradientBrush::getAngle() const {
	return angle;
}
