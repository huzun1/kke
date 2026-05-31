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

/**
 * @brief Represents a transform source.
 *
 * This variant encapsulates all supported transformation types
 * that can be applied during rendering.
 */
using TransformSource = std::variant<Translation, Scaling, Rotation>;

/**
 * @brief Represents a mask source for layer operations.
 *
 * A mask can be defined either as a single geometry or a composed geometry.
 */
using MaskSource = std::variant<Geometry, GeometryCompose>;

using EffectClipSource = MaskSource;

/**
 * @brief Represents a stroke rendering source.
 *
 * Defines the geometric input used for stroke rendering,
 * including simple primitives and composed geometries.
 */
using StrokeSource = std::variant<Line, Geometry, GeometryCompose>;

/**
 * @brief Represents a fill rendering source.
 *
 * Defines the input used for fill operations, including
 * geometric shapes and text.
 */
using FillSource = std::variant<Geometry, GeometryCompose, Text>;

/**
 * @brief Represents a source for effect rendering.
 *
 * Effects can be applied to various types of renderable inputs,
 * including geometry, text, and offscreen canvases.
 */
using EffectSource = std::variant<Line, Geometry, GeometryCompose, Text, std::shared_ptr<Canvas>>;

}; // namespace kke
