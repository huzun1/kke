# D2D Effect Rendering

この文書は、このプロジェクト固有の実装メモである。D2D backend の effect 描画責務に関する判断をまとめる。

## Dedicated Effect Renderer

effect 描画は `D2dEngine` に直接ロジックを増やさず、`EffectRenderer` に集約する。

`D2dEngine` は他の renderer と同様に、API entry point から `EffectRenderer` へ委譲する薄い層として扱う。

`EffectRenderer` は effect 分岐と clip orchestration に留め、`Blur` / `Shadow` の position-independent cache は専用 cache class をそれぞれの renderer 側で使う。

position-independent cache 自体は brush や geometry 正規化の知識を抱え込まず、source/brush の正規化は別 helper に切り出して再利用可能に保つ。

## Source Rasterization Rule

`EffectSource` の `Geometry` / `GeometryCompose` / `Line` / `Text` は、そのまま D2D effect へ渡さず、一度 command list canvas へ rasterize して `ID2D1Image` 化する。

このときの描き方は `EffectSourceAppearance` で固定する。

- `drawMode` が `Fill` の場合は fill source として描く
- `drawMode` が `Stroke` の場合は stroke source として描く
- `strokeWidth` は stroke 描画時に使う
- `brush` は source image を作るときの見た目として使う

effect ごとに source の解釈を変えない。blur と shadow の違いは、source image を作った後の effect chain だけで吸収する。

## Effect Clip

effect の適用範囲を絞りたい場合は、`EffectSource` に clip の意味を持たせず、別引数の `EffectClipSource` で表現する。

`EffectClipSource` は effect の入力 image を変えるのではなく、最終的な `DrawImage` の出力範囲だけを制限する mask として扱う。

`Rect` を clip に使う場合は、出力 mask だけでなく入力 image にも `Crop` を入れる。

実装では既存の layer 制御を再利用し、`DrawImage` の前後で `PushLayer(mask)` / `PopLayer()` を行う。

## Blur Mode

`BlurEffect` は通常の blur に加えて `OuterOnly` を持つ。

`OuterOnly` の場合は、blur 出力から元 source の内側を `Composite(DestinationOut)` で削り、外側のにじみだけを描く。

## Command List Cycle

`renderEffect(effect)` のような source なし overload は、「現在の target command list の内容全体を入力 image にして、その結果を新しい target command list へ描き戻す」として扱う。

実装では以下のサイクルを使う。

1. 現在の target command list を close する
2. close 済み command list を effect 入力 image として保持する
3. 新しい command list を作成して device context の target に差し替える
4. `D2dContext` の target command list 参照も新しいものへ更新する
5. effect 出力を新しい command list へ描く

これにより、source なし effect を描画パス途中で表現しても、`D2dEngine` は orchestration だけで済む。
