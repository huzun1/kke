#pragma once

#include <memory>
#include <variant>

#include "kke/appearance/Text.hh"
#include "kke/appearance/resource/Canvas.hh"
#include "kke/appearance/resource/GeometryCompose.hh"
#include "kke/appearance/transform/Rotation.hh"
#include "kke/appearance/transform/Scaling.hh"
#include "kke/appearance/transform/Translation.hh"
#include "kke/geometry/Geometry.hh"
#include "kke/geometry/primitives/Line.hh"

namespace kke {
using TransformSource = std::variant<Translation, Scaling, Rotation>;

using MaskSource = std::variant<Geometry, GeometryCompose>;

using StrokeSource = std::variant<Line, Geometry, GeometryCompose>;

using FillSource = std::variant<Geometry, GeometryCompose, Text>;

using EffectSource = std::variant<Line, Geometry,
								  GeometryCompose, Text,
								  std::shared_ptr<Canvas>>;
};	// namespace kke
