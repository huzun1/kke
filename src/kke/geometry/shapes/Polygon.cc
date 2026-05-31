#include "Polygon.hh"

using namespace kke;

Polygon::Polygon(std::vector<Point> points) : points(std::move(points)) {
}

void Polygon::addPoint(Point point) {
	points.push_back(std::move(point));
}

std::vector<Point> const& Polygon::getPoints() const {
	return points;
}

Boundary Polygon::getBounding() const {
	if (points.empty()) {
		return Boundary{0, 0, 0, 0};
	}
	float minX = points[0].x;
	float maxX = points[0].x;
	float minY = points[0].y;
	float maxY = points[0].y;
	for (const auto& point : points) {
		if (point.x < minX) {
			minX = point.x;
		}
		if (point.x > maxX) {
			maxX = point.x;
		}
		if (point.y < minY) {
			minY = point.y;
		}
		if (point.y > maxY) {
			maxY = point.y;
		}
	}
	return Boundary{minX, minY, maxX - minX, maxY - minY};
}
