# Effects

This guide explains what each built-in effect is for, which parameters matter most, and how to choose between them.

## Quick Map

- `kke::ShadowEffect`: creates a drop shadow or glow-like soft offset around a source.
- `kke::BlurEffect`: applies a general blur in all directions.
- `kke::DirectionalBlurEffect`: applies blur mainly along one direction.
- `kke::ColorMatrixEffect`: remaps color and alpha through a 4x5 matrix.
- `kke::EffectCompose`: chains multiple effects in order.

## Effect Sources

The source-based `renderEffect` overload first rasterizes a source, then applies the effect.

Typical sources are:

- shapes such as `Rect`, `Ellipse`, or `RoundedRect`
- `GeometryCompose`
- `Text`
- `Canvas`

Use `kke::EffectSourceAppearance` to control how that source is rasterized before the effect runs.

```cpp
kke::EffectSourceAppearance appearance;
appearance.brush = kke::SolidColorBrush({0.92f, 0.60f, 0.24f, 1.0f});
appearance.drawMode = kke::EffectSourceDrawMode::Fill;
appearance.strokeWidth = 3.0f;
```

`strokeWidth` matters only when `drawMode` is `Stroke`.

## ShadowEffect

Use `kke::ShadowEffect` when you want separation from the background, soft depth, or a glow-like silhouette.

```cpp
engine.renderEffect(
	kke::RoundedRect{kke::Rect{{160.0f, 120.0f}, {460.0f, 280.0f}}, 24.0f},
	{{kke::SolidColorBrush({1.0f, 1.0f, 1.0f, 1.0f})}},
	kke::ShadowEffect{{10.0f, 12.0f}, 8.0f, {0.0f, 0.0f, 0.0f, 0.30f}, kke::ShadowMode::ShadowWithSource});
```

Important fields:

- `offset`: moves the shadow relative to the source.
- `blurStandardDeviation`: controls softness. Larger values spread the shadow further.
- `color`: tint of the generated shadow.
- `mode`: decides whether the source, the shadow, or only the outer shadow is kept.

Choose the mode like this:

- `ShadowWithSource`: most common. Draws the original source and the shadow together.
- `ShadowOnly`: keeps only the shadow result, including overlap with the source silhouette.
- `OuterShadowOnly`: keeps only the outer part of the shadow, which is useful for halo or outline-like looks.

Useful patterns:

- Use a small offset and a colored shadow for glow-like emphasis.
- Use a larger downward offset and a dark translucent color for card-style depth.

## BlurEffect

Use `kke::BlurEffect` when you want an evenly softened source.

```cpp
engine.renderEffect(
	kke::Ellipse{{420.0f, 260.0f}, 74.0f},
	{{kke::SolidColorBrush({0.28f, 0.82f, 0.66f, 0.95f})}},
	kke::BlurEffect{{18.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED, kke::BlurMode::Normal}});
```

Important fields:

- `radius`: blur strength, stored in `BlurAppearance`.
- `borderMode`: how the backend treats the blurred edge near crop boundaries.
- `optimization`: quality versus speed hint.
- `mode`: whether to keep the whole blurred result or only the outside.

Choose the mode like this:

- `BlurMode::Normal`: full blur result. Best for softening a source.
- `BlurMode::OuterOnly`: drops the interior and keeps only the blurred outside. Good for glow rings and soft outlines.

Choose the border mode like this:

- `BlurBorderMode::SOFT`: safer default for natural-looking edges.
- `BlurBorderMode::HARD`: use when you want a firmer clipped edge.

Choose the optimization like this:

- `SPEED`: use when effect cost matters more than precision.
- `BALANCED`: good default.
- `QUALITY`: use when large, visible blur fidelity matters most.

## DirectionalBlurEffect

Use `kke::DirectionalBlurEffect` when motion or smear should read more strongly in one direction than another.

```cpp
engine.renderEffect(
	kke::Rect{{220.0f, 340.0f}, {520.0f, 420.0f}},
	{{kke::SolidColorBrush({0.36f, 0.58f, 1.0f, 0.88f})}},
	kke::DirectionalBlurEffect{16.0f, 24.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED});
```

Important fields:

- `standardDeviation`: blur strength.
- `angle`: direction in degrees.
- `borderMode` and `optimization`: same role as in `BlurEffect`.

When to choose it over `BlurEffect`:

- Use `BlurEffect` for general softness.
- Use `DirectionalBlurEffect` for speed lines, streaked highlights, or motion-biased glow.

## ColorMatrixEffect

Use `kke::ColorMatrixEffect` when you want to recolor, tint, grayscale, fade, or otherwise remap pixels mathematically.

```cpp
kke::ColorMatrixEffect effect;
effect.colorMatrix.values = {
	0.2126f, 0.2126f, 0.2126f, 0.0f,
	0.7152f, 0.7152f, 0.7152f, 0.0f,
	0.0722f, 0.0722f, 0.0722f, 0.0f,
	0.0f,    0.0f,    0.0f,    1.0f,
	0.0f,    0.0f,    0.0f,    0.0f
};

engine.renderEffect(effect, kke::Rect{{760.0f, 140.0f}, {1120.0f, 420.0f}});
```

Important fields:

- `colorMatrix.values`: 20 floats representing a 4x5 matrix.
- `alphaMode`: tells the backend whether source colors should be treated as premultiplied or straight alpha.
- `clampOutput`: clamps final values into a valid color range.

Practical notes:

- Start from the identity matrix and change only the rows you need.
- Use `Premultiplied` when matching normal render-target behavior.
- Enable `clampOutput` when experimenting with stronger transforms that might overshoot.

## EffectCompose

Use `kke::EffectCompose` when one pass is not enough.

```cpp
kke::EffectCompose compose;
compose.addEffect(kke::ShadowEffect{{0.0f, 10.0f}, 16.0f, {0.04f, 0.08f, 0.14f, 0.28f}, kke::ShadowMode::ShadowWithSource});
compose.addEffect(kke::BlurEffect{{3.0f, kke::BlurBorderMode::SOFT, kke::BlurOptimization::BALANCED, kke::BlurMode::Normal}});

engine.renderEffect(compose, kke::Rect{{120.0f, 120.0f}, {980.0f, 580.0f}});
```

The order matters.

- Earlier effects feed into later effects.
- `Shadow -> Blur` softens the shadowed result.
- `Blur -> ColorMatrix` recolors the already-blurred image.

As a rule, place shape-defining effects first and color-finishing effects later.

## Which One Should I Pick?

- Use `ShadowEffect` for depth, glow, and separation.
- Use `BlurEffect` for softening in all directions.
- Use `DirectionalBlurEffect` for motion-biased blur.
- Use `ColorMatrixEffect` for pixel remapping and recoloring.
- Use `EffectCompose` when the look depends on multiple passes.

## Related Guides

- [Layers, Canvas, and Effects](layers-canvas-effects.md)
- [Appearance API](appearance-api.md)
