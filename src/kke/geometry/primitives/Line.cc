#include "Line.hh"

#include <algorithm>

using namespace kke;

Line::Line() : start(0, 0), end(0, 0), thickness(0.0f) {
}

Line::Line(kke::Point const& start, kke::Point const& end, float thickness)
	: start(start), end(end), thickness(thickness) {
}

Boundary Line::getBounding() const {
	Boundary boundary;
	boundary.left = std::min(start.x, end.x) - thickness / 2.0f;
	boundary.right = std::max(start.x, end.x) + thickness / 2.0f;
	boundary.top = std::min(start.y, end.y) - thickness / 2.0f;
	boundary.bottom = std::max(start.y, end.y) + thickness / 2.0f;
	return boundary;
}
