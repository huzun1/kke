# D2D Texture Resources

この文書は、このプロジェクト固有の実装メモである。texture upload と draw の責務分割に関する判断をまとめる。

## Upload API

texture upload は encoded bytes と raw RGBA8 の 2 系統を project-side API として持つ。

encoded bytes は `Engine::uploadTexture(void const*, size_t)` を使い、PNG/JPEG などの decode を内部で吸収する。

raw pixels は引数爆発を避けるため、幅・高さ・stride を `RawTextureData` にまとめて `Engine::uploadTexture(RawTextureData const&)` で受ける。

## Draw API

texture draw は opacity, interpolation, srcRect を `TextureDrawAppearance` にまとめて受ける。

将来的に tile mode や effect 連携などの option が増えても、`draw(texture, rect, appearance)` の形を保つ。

## D2D Responsibility

`TextureProvider` は upload の窓口と WIC imaging factory の保持だけを担当する。

`TextureFactory` は encoded decode, raw RGBA8 -> PBGRA 変換, `ID2D1Bitmap1` 生成だけを担当する。

`TextureRenderer` は `D2dTexture` を `DrawBitmap` する描画責務だけを持つ。
