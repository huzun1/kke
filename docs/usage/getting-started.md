# Getting Started

This guide shows the basic flow for using the engine from application code.

## Typical Frame Flow

With the current backend, the normal flow is:

1. Create a `kke::D2dEngine`.
2. Call `beginDraw(...)` with a valid `D2dContext` and render target.
3. Clear the target.
4. Issue draw calls such as `fill`, `draw`, and `renderEffect`.
5. Call `endDraw()`.

```cpp
kke::D2dEngine engine;

engine.beginDraw(context, renderTarget);
engine.clear();

engine.fill(
	kke::Rect{{80.0f, 80.0f}, {320.0f, 220.0f}},
	kke::SolidColorBrush({0.20f, 0.40f, 0.80f, 1.0f}));

engine.endDraw();
```

## Source Types You Will Use Most

Most public APIs accept one of the source variants defined in `src/kke/engine/Sources.hh`.

- `FillSource`: geometry, composed geometry, or text
- `StrokeSource`: lines, geometry, or composed geometry
- `MaskSource`: geometry or composed geometry
- `EffectSource`: lines, geometry, composed geometry, text, or canvas

In practice, simple shapes are usually passed directly:

```cpp
engine.fill(kke::Rect{{0.0f, 0.0f}, {200.0f, 120.0f}}, brush);
engine.draw(kke::Ellipse{{220.0f, 120.0f}, 40.0f}, outlineBrush, {3.0f});
```

## Common Headers

Depending on what you draw, these headers are the ones you will reach for most often:

- `kke/engine/d2d/D2dEngine.hh`
- `kke/appearance/resource/brush/impl/SolidColorBrush.hh`
- `kke/appearance/painting/StrokeAppearance.hh`
- `kke/geometry/shapes/Rect.hh`
- `kke/geometry/curved/Ellipse.hh`
- `kke/appearance/Text.hh`

## Where To Go Next

- For shape drawing, see [Shapes and Strokes](shapes-and-strokes.md).
- For text drawing, see [Text](text.md).
- For texture drawing, see [Textures](textures.md).
- For clipping, offscreen rendering, and effects, see [Layers, Canvas, and Effects](layers-canvas-effects.md).
