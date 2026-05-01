# D2D Geometry Resources

この文書は、このプロジェクト固有の設計メモである。D2D geometry resource 周りで採用する判断だけをまとめる。

関連する横断ルールは次も参照する。

- `docs/arch/d2d-context-responsibility.md`
- `docs/impl/d2d-resource-caching.md`
- `docs/impl/d2d-resource-responsibility.md`
- `docs/impl/d2d-condition-labeling.md`

## GeometryProvider

`GeometryProvider` は、`Geometry` や `GeometryCompose` を Direct2D の `ID2D1Geometry` に変換し、同じ入力に対する geometry を再利用する責務を持つ。

`GeometryCompose` は複数の `Geometry` をまとめた mask/source として扱う。D2D 側では `ID2D1GeometryGroup` として表現し、個々の geometry は `GeometryProvider::get(D2dContext const&, Geometry const&)` 経由で取得する。

## Inverted Geometry

inverted geometry は、viewport 全体から mask geometry を差し引いた geometry として扱う。

反転 geometry は viewport サイズに依存する。そのため、キャッシュキーには viewport サイズと mask geometry の position-dependent hash を含める。

viewport rectangle geometry は inverted geometry を作るための素材 resource である。mask を差し引いた inverted geometry は派生 resource として扱う。

反転 geometry の生成処理は、cache 管理や viewport geometry の cache 管理から分ける。`InvertedGeometryProvider` は cache と依存部品の接続を担当し、Direct2D の combine 処理は factory に閉じ込める。
