#include "Line.hh"

#include <algorithm>

using namespace kke;

Line::Line() : start(0, 0), end(0, 0) {
}

Line::Line(kke::Point2f const& start, kke::Point2f const& end) : start(start), end(end) {
}

Boundary Line::getBounding() const {
    Boundary boundary;
    boundary.left = std::min(start.x, end.x);
    boundary.right = std::max(start.x, end.x);
    boundary.top = std::min(start.y, end.y);
    boundary.bottom = std::max(start.y, end.y);
    return boundary;
}
