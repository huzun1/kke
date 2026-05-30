#pragma once

#include <variant>

#include "kke/geometry/curved/Ellipse.hh"
#include "kke/geometry/curved/RoundedRect.hh"
#include "kke/geometry/shapes/Polygon.hh"
#include "kke/geometry/shapes/Rect.hh"
#include "kke/geometry/shapes/Triangle.hh"

namespace kke {
using Geometry = std::variant<Triangle, Rect, RoundedRect, Ellipse, Polygon>;
};
