# Text

This guide covers basic text rendering and text effects.

## Basic Text Fill

Text is rendered with `fill(source, brush)`, where `source` is `kke::Text`.

```cpp
engine.fill(
	kke::Text{
		L"Space Grotesk via DWrite",
		{120.0f, 210.0f},
		{"Space Grotesk", 54.0f, kke::FontWeight::BOLD}
	},
	kke::SolidColorBrush({0.95f, 0.97f, 0.99f, 1.0f}));
```

`kke::Text` contains:

- the text value as `std::string` or `std::wstring`
- a `Point` position
- a `FontAppearance`

## Measuring Text

Use `measureTextSize` when you need to place text dynamically.

```cpp
kke::Text fpsText{
	L"FPS 60.0",
	{0.0f, 0.0f},
	{"Space Grotesk", 18.0f, kke::FontWeight::BOLD}
};

kke::Scale textSize = engine.measureTextSize(fpsText);
```

This is useful for overlays, labels, and UI-like panels.

## Uploading Fonts

The current engine API provides font upload through raw bytes:

```cpp
std::shared_ptr<kke::Font> font = engine.uploadFont(fontData, fontSize);
```

Once uploaded, the backend can resolve the font family used in `FontAppearance`.

## Applying Effects To Text

Text can also be used as an `EffectSource`.

```cpp
kke::EffectSourceAppearance appearance;
appearance.brush = kke::SolidColorBrush({0.45f, 0.79f, 1.0f, 1.0f});

engine.renderEffect(
	kke::Text{
		L"ShadowWithSource text sample",
		{120.0f, 310.0f},
		{"Space Grotesk", 32.0f, kke::FontWeight::MEDIUM}
	},
	appearance,
	kke::ShadowEffect{{6.0f, 8.0f}, 5.0f, {0.02f, 0.06f, 0.14f, 0.74f}, kke::ShadowMode::ShadowWithSource});
```

## Useful Patterns

- Use `std::wstring` when working with wide text content.
- Measure first when the text position depends on viewport size.
- For text glow or shadow, use `renderEffect` with a `Text` source instead of drawing a manual duplicate.
