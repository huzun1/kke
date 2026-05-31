# KKE on Direct2D

<p align="center">
  <img src="/images/kke.png" width=200 />
</p>

KKE is a Windows 2D rendering engine built around a Direct2D / DirectWrite / WIC backend. The current implementation centers on `kke::D2dEngine` and supports shapes, text, textures, layers, offscreen canvases, and effects through one API surface.

## Quick Showcase

```cpp
engine().draw(
	mountainTexture(),
	{{180.0f, 90.0f}, {1100.0f, 650.0f}},
	{0.95f, kke::TextureInterpolation::Linear}
);

kke::RoundedRect blurRect({{400.0f, 200.0f}, {600.0f, 400.0f}}, 20.0f);
engine().renderEffect(kke::BlurEffect(30.0f), blurRect);

kke::StrokeAppearance blurRectStrokeStyle{0.5f};
engine().renderEffect(
	blurRect,
	kke::EffectSourceAppearance{},
	kke::ShadowEffect{
		.color = {0.0f, 0.0f, 0.0f, 0.3f},
		.mode = kke::ShadowMode::OuterShadowOnly
	}
);
engine().draw(blurRect, kke::SolidColorBrush({1.0f, 1.0f, 1.0f, 0.6f}), blurRectStrokeStyle);
engine().fill(blurRect, kke::SolidColorBrush({1.0f, 1.0f, 1.0f, 0.2f}));

kke::Text title{
	L"KKE Renderer",
	{425.0f, 280.0f},
	{"Space Grotesk", 24.0f, kke::FontWeight::BOLD}
};
engine().renderEffect(
	title,
	kke::EffectSourceAppearance{},
	kke::ShadowEffect{
		.color = {0.0f, 0.0f, 0.0f, 0.3f},
		.mode = kke::ShadowMode::OuterShadowOnly
	}
);
engine().fill(title, kke::SolidColorBrush({1.0f, 1.0f, 1.0f, 1.0f}));
```

![showcase](/images/showcase.png)

## Features

- `C++20`
- Windows / Direct2D / DirectWrite / WIC
- `kke::D2dEngine` based API
- Shape rendering: `fill`, `draw`
- Text rendering and font upload
- Text size measurement
- Texture upload and rendering
- Layer rendering: `pushLayer`
- Offscreen canvas rendering: `pushCanvas`
- Effect rendering: `renderEffect`
- Included demos: blur, shadow, text, texture, canvas, layer, transform

## Project Layout

- `src/kke/engine/Engine.hh`: public engine interface
- `src/kke/engine/Sources.hh`: variant-based source types used by the API
- `src/kke/engine/d2d`: Direct2D backend implementation
- `src/kke/geometry`: geometry and primitive types
- `src/kke/appearance`: brushes, text, transforms, effects, and other draw parameters
- `test`: Windows test app and renderer demos

## Build

A minimal CMake flow looks like this:

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

This produces the `kke` static library and the `kke_test` sample application.

When `kke` is consumed as a subproject through `FetchContent` or `add_subdirectory`, the test application is not built by default.

## CMake Package Usage

After installing the library, consumers can use `find_package`:

```cmake
find_package(kke CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE kke::kke)
```

## Usage Docs

The docs are organized by usage domain instead of backend internals:

- [Getting Started](docs/usage/getting-started.md)
- [Shapes and Strokes](docs/usage/shapes-and-strokes.md)
- [Text](docs/usage/text.md)
- [Textures](docs/usage/textures.md)
- [Layers, Canvas, and Effects](docs/usage/layers-canvas-effects.md)

## Notes

- `kke::D2dEngine` adds `beginDraw` and `endDraw` on top of `kke::Engine` to define the Direct2D frame boundary.
- In the current tests, simple shapes such as `kke::Rect` and `kke::Ellipse` are passed directly to `fill` and `draw` without manually wrapping them in `kke::Geometry`.
