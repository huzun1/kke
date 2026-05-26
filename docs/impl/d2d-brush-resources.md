# D2D Brush Resources

この文書は、このプロジェクト固有の実装メモである。D2D brush resource の責務分割に関する判断をまとめる。

## BrushProvider

`BrushProvider` は project-side の `Brush` を Direct2D の `ID2D1Brush` に変換し、同じ入力に対する brush を再利用する責務を持つ。

cache key は `BrushHasher` で作る。solid color brush は RGBA 値を含め、linear gradient brush は color stops、start/end point、angle を含める。

## BrushFactory

D2D API を使って実際に brush を生成する処理は `BrushFactory` に閉じ込める。

renderer は brush の生成方法を持たず、`D2dResourceProviders::getBrushProvider()` から取得した `ID2D1Brush` を使って描画する。

## Renderer との境界

`D2dEngine` は draw/fill の入力を受け取り、現在の `D2dEngineContext` とともに renderer へ委譲する。

`StrokeRenderer` と `FaceRenderer` は brush を `BrushProvider` から取得し、geometry を `GeometryProvider` から取得して `DrawGeometry` / `FillGeometry` する。`Line` のように `Geometry` ではない primitive だけは renderer が直接 D2D primitive call に落とす。

`D2dEngine` に shape 分岐、brush 生成、path geometry 作成を持たせない。
