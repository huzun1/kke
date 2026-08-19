# Textures

This guide covers texture upload and texture drawing.

## Uploading Encoded Texture Data

Use `uploadTexture(void const*, size_t)` when you have encoded image bytes such as PNG data.

```cpp
std::shared_ptr<kke::Texture> texture = engine.uploadTexture(textureData, textureSize);
```

## Uploading Raw Pixel Data

Use `uploadTexture(RawTextureData const&)` when you already have pixel data in memory.

```cpp
std::shared_ptr<kke::Texture> texture = engine.uploadTexture({
	pixels.data(),
	width,
	height,
	width * 4
});
```

## Drawing A Texture

Use `draw(texture, destRect, appearance)`.

```cpp
engine.draw(
	texture,
	kke::Rect{{860.0f, 164.0f}, {1180.0f, 500.0f}},
	{0.98f, kke::TextureInterpolation::Linear});
```

## Drawing A Sub-Rectangle

`TextureDrawAppearance` supports an optional source rectangle:

```cpp
engine.draw(
	texture,
	kke::Rect{{860.0f, 164.0f}, {1180.0f, 500.0f}},
	{0.98f, kke::TextureInterpolation::Linear, kke::Rect{{70.0f, 24.0f}, {360.0f, 250.0f}}});
```

The fields are:

- `opacity`
- `interpolation`
- `srcRect`

## Useful Patterns

- Use encoded upload when your app already loads PNG or similar assets.
- Use raw upload for procedural textures, generated previews, or test patterns.
- Use `TextureInterpolation::Nearest` for pixel-art style output.
- Use `TextureInterpolation::HighQualityCubic` when photographs or artwork are
  reduced significantly and smooth downscaling matters more than sampling cost.
