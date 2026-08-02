# Layers, Canvas, and Effects

This guide covers the stateful parts of the rendering API: clipping, offscreen rendering, transforms, and effects.

## Layers And Clipping

Use `pushLayer(mask)` and `popLayer()` to clip subsequent drawing.

```cpp
kke::RoundedRect clipRect{
	kke::Rect{{76.0f, 164.0f}, {372.0f, 472.0f}},
	28.0f
};

engine.pushLayer(clipRect);
engine.fill(kke::Rect{{40.0f, 140.0f}, {400.0f, 500.0f}}, fillBrush);
engine.popLayer();
```

You can also use composed geometry as the mask.

## Inverted Layers

Use `LayerMode::Inverted` when you want to draw everywhere except inside the mask.

```cpp
engine.pushLayer(clipRect, kke::LayerMode::Inverted);
engine.fill(kke::Rect{{0.0f, 0.0f}, {1280.0f, 720.0f}}, overlayBrush);
engine.popLayer();
```

## Offscreen Canvas

Use canvas rendering when you want to build content once and reuse it later.

```cpp
std::shared_ptr<kke::Canvas> canvas = engine.createCanvas();

engine.beginCanvas(canvas);
engine.fill(kke::RoundedRect{kke::Rect{{920.0f, 420.0f}, {1140.0f, 610.0f}}, 24.0f}, fillBrush);
engine.endCanvas();
engine.finishCanvas(canvas);

engine.draw(canvas, 1.0f);
```

`endCanvas` only restores the previous render target. Call `finishCanvas` when recording is
complete; only a finished canvas can be drawn or used as an effect source. An unfinished canvas
can be passed to `beginCanvas` again to continue recording.

Canvases can also be used as effect sources.

## Transforms

Use `pushTransform` and `popTransform` around the drawing you want to affect.

```cpp
engine.pushTransform(kke::Translation{{48.0f, 44.0f}});
engine.pushTransform(kke::Scaling{{1010.0f, 300.0f}, {1.20f, 0.85f}});

engine.fill(kke::Ellipse{{1010.0f, 300.0f}, 72.0f}, shapeBrush);

engine.popTransform();
engine.popTransform();
```

Supported transform sources are:

- `kke::Translation`
- `kke::Scaling`
- `kke::Rotation`

## Rendering Effects From A Source

Use the source-based `renderEffect` overload when you want an effect around a specific shape, text run, or canvas.

```cpp
kke::EffectSourceAppearance glowAppearance;
glowAppearance.brush = kke::SolidColorBrush({0.20f, 0.80f, 0.46f, 0.9f});

engine.renderEffect(
	kke::Ellipse{{320.0f, 240.0f}, 78.0f},
	glowAppearance,
	kke::BlurEffect{{18.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED}});
```

## Chaining Effects

Use `kke::EffectCompose` when one effect is not enough.

```cpp
kke::EffectCompose compose;
compose.addEffect(kke::ShadowEffect{{0.0f, 18.0f}, 24.0f, {0.02f, 0.04f, 0.08f, 0.22f}, kke::ShadowMode::ShadowWithSource});
compose.addEffect(kke::BlurEffect{{4.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED, kke::BlurMode::Normal}});

engine.renderEffect(compose, kke::Rect{{110.0f, 100.0f}, {1130.0f, 620.0f}});
```

## Effect Source Appearance

`EffectSourceAppearance` lets you control how the source is rendered before the effect is applied.

```cpp
kke::EffectSourceAppearance appearance;
appearance.brush = kke::SolidColorBrush({1.0f, 0.62f, 0.28f, 0.94f});
appearance.drawMode = kke::EffectSourceDrawMode::Fill;
appearance.strokeWidth = 3.0f;
```

This is especially useful when the same geometry should be used with different effect looks.
