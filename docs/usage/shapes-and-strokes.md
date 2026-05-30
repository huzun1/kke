# Shapes and Strokes

This guide covers the most direct path for drawing geometry.

## Filling Shapes

Use `fill(source, brush)` for solid or gradient-filled geometry.

Simple shapes can be passed directly:

```cpp
kke::Brush panelFill = kke::SolidColorBrush({0.14f, 0.18f, 0.24f, 1.0f});

engine.fill(
	kke::Rect{{80.0f, 120.0f}, {360.0f, 320.0f}},
	panelFill);

engine.fill(
	kke::Ellipse{{520.0f, 220.0f}, 72.0f},
	kke::SolidColorBrush({0.33f, 0.78f, 0.94f, 0.95f}));
```

Common geometry types in the current codebase:

- `kke::Rect`
- `kke::Ellipse`
- `kke::Triangle`
- `kke::RoundedRect`
- `kke::Polygon`

## Drawing Outlines

Use `draw(source, brush, strokeAppearance)` for strokes.

```cpp
engine.draw(
	kke::RoundedRect{kke::Rect{{420.0f, 140.0f}, {720.0f, 340.0f}}, 24.0f},
	kke::SolidColorBrush({0.96f, 0.97f, 0.99f, 1.0f}),
	{3.0f});
```

`StrokeAppearance` currently exposes a `width` field:

```cpp
kke::StrokeAppearance stroke {4.0f};
engine.draw(kke::Ellipse{{880.0f, 240.0f}, 64.0f}, brush, stroke);
```

## Drawing Lines

`StrokeSource` also accepts `kke::Line`.

```cpp
engine.draw(
	kke::Line{{100.0f, 100.0f}, {300.0f, 220.0f}},
	kke::SolidColorBrush({1.0f, 1.0f, 1.0f, 0.8f}),
	{2.0f});
```

## Composed Geometry

Use `kke::GeometryCompose` when you want a single source made from multiple shapes.

```cpp
kke::GeometryCompose badge;
badge.add(kke::Ellipse{{970.0f, 252.0f}, 72.0f});
badge.add(kke::RoundedRect{kke::Rect{{900.0f, 252.0f}, {1120.0f, 430.0f}}, 34.0f});
badge.add(kke::Rect{{932.0f, 396.0f}, {1104.0f, 494.0f}});

engine.fill(badge, kke::SolidColorBrush({0.20f, 0.80f, 0.46f, 0.9f}));
engine.draw(badge, kke::SolidColorBrush({0.95f, 0.97f, 0.99f, 1.0f}), {3.0f});
```

## Useful Patterns

- Use `fill` first and `draw` second when you want a clean outlined shape.
- Reuse the same geometry source for both base rendering and effects.
- Prefer passing `kke::Rect` or `kke::Ellipse` directly unless you really need an explicit `kke::Geometry` variable.
