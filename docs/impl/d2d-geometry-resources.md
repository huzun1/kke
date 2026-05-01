# D2D Geometry Resources Implementation

この文書は、このプロジェクト固有の実装メモである。D2D geometry resource 固有の実装判断だけをまとめる。

関連する横断パターンは次も参照する。

- `docs/impl/d2d-resource-caching.md`
- `docs/impl/d2d-resource-responsibility.md`
- `docs/impl/d2d-condition-labeling.md`

## GeometryCompose を D2D Geometry に変換する場合

`GeometryCompose` のように複数の `Geometry` から 1 つの D2D geometry を作る場合は、個々の `Geometry` を `GeometryProvider::get(D2dContext const&, Geometry const&)` で取得し、その結果から `ID2D1GeometryGroup` を作る。

cache key は `GeometryHasher::hash(compose, GeometryHashMode::PositionDependent)` を使う。

一部の geometry 作成に失敗した場合は `nullptr` を返す。失敗した geometry group はキャッシュしない。

## Inverted Geometry を作る場合

反転 mask のように「viewport から mask を差し引く」場合は、viewport rectangle geometry を基準にして、mask geometry を `D2D1_COMBINE_MODE_EXCLUDE` で差し引く。

結果は `ID2D1PathGeometry` を作り、`Open()` した `ID2D1GeometrySink` に D2D の combine 結果を書き込む。

`Open`、`CombineWithGeometry`、`Close` のどれかに失敗した場合は `nullptr` を返し、失敗結果は cache しない。

## Viewport Geometry を扱う場合

viewport rectangle geometry は、inverted geometry 生成の素材 resource として扱う。

viewport rectangle geometry の保持、viewport サイズ変更の検出、再作成は `ViewportGeometryCache` の責務にする。

inverted geometry provider は、viewport geometry の具体的な作成方法を持たず、`ViewportGeometryCache` から取得した geometry と mask geometry を factory に渡す。
