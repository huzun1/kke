# Appearance API

This guide gives a quick map of the public types under `src/kke/appearance` and when to use them.

## Core Value Types

Use these small data types when describing colors, sizes, text, and layer behavior.

- `kke::Color`: RGBA color stored as normalized floats. `KKE_COLOR_RGB24` and `KKE_COLOR_RGBA32` are convenience macros for hex literals.
- `kke::Scale`: two-component size or scale factor. Arithmetic operators work component-wise.
- `kke::Text`: renderable text source made of text content, a position, and `FontAppearance`.
- `kke::LayerMode`: controls whether a pushed layer clips inside the mask or outside it.

## Brushes And Paint

Brushes define how fills, strokes, and effect sources are rasterized.

- `kke::SolidColorBrush`: paints with a single `Color`.
- `kke::LinearGradientBrush`: paints along a line from `startPoint` to `endPoint` using a list of colors.
- `kke::Brush`: variant that currently accepts solid-color and linear-gradient brushes.
- `kke::StrokeAppearance`: holds stroke width for outline drawing.

## Text And Font Resources

Use these types together when drawing text.

- `kke::FontAppearance`: font family, size, and weight.
- `kke::FontWeight`: lightweight enum for common typographic weights.
- `kke::Font`: backend-owned uploaded font resource returned by the engine.

See also: [Text](text.md)

## Texture Resources

These types cover texture upload and drawing.

- `kke::Texture`: backend-owned uploaded texture resource.
- `kke::RawTextureData`: raw pixel pointer, dimensions, and row stride for upload.
- `kke::TextureDrawAppearance`: opacity, interpolation mode, and optional source rectangle for drawing.
- `kke::TextureInterpolation`: `Nearest` for crisp sampling, `Linear` for smoother scaling.
- `kke::TextureType`: encoded image kind for APIs that need an explicit format.

See also: [Textures](textures.md)

## Geometry Composition And Canvas

These types help when a single draw source is not enough.

- `kke::GeometryCompose`: stores multiple geometry values as one composite source.
- `kke::Canvas`: backend-owned offscreen render target that can be drawn later or reused as an effect source.

See also: [Layers, Canvas, and Effects](layers-canvas-effects.md)

## Transforms

Push these transform values around the drawing you want to affect.

- `kke::Translation`: shifts the source by a fixed offset.
- `kke::Scaling`: scales the source around a pivot point.
- `kke::Rotation`: rotates the source around a pivot point.

## Effects

Effects are value types that describe a post-process applied by the engine.

- `kke::Effect`: variant of all built-in effect types.
- `kke::EffectCompose`: ordered chain of effects.
- `kke::EffectIdentifier` and `kke::EffectKind`: helper for inspecting which concrete effect is stored in an `Effect`.
- `kke::EffectSourceAppearance`: controls the brush, fill/stroke mode, and stroke width used to rasterize the source before applying an effect.
- `kke::EffectSourceDrawMode`: selects fill or stroke rasterization.
- `kke::ShadowEffect` and `kke::ShadowMode`: shadow offset, blur, color, and source/shadow output mode.
- `kke::BlurEffect`, `kke::BlurAppearance`, `kke::BlurMode`, `kke::BlurBorderMode`, `kke::BlurOptimization`: general-purpose blur controls.
- `kke::DirectionalBlurEffect`: directional blur with angle and blur tuning.
- `kke::ColorMatrix`, `kke::ColorMatrixEffect`, `kke::ColorMatrixAlphaMode`: 4x5 color transform support.

See also: [Layers, Canvas, and Effects](layers-canvas-effects.md)
See also: [Effects](effects.md)

## Practical Rule Of Thumb

- Reach for the small structs first. Most appearance APIs are plain value types intended to be constructed inline.
- Use `Brush`, `TextureDrawAppearance`, and `EffectSourceAppearance` when the source stays the same but the visual treatment changes.
- Use `EffectCompose` when you want multiple passes without inventing a new source type.
