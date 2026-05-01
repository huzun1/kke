# D2D Context Responsibility

この文書は、このプロジェクト固有の設計メモである。`D2dContext` の責務境界を広げすぎないための判断をまとめる。

## Thin D2D Boundary

`D2dContext` は、D2D の factory、device context、target command list への薄いアクセス境界として扱う。

`D2dContext` は「D2D にアクセスするための現在のハンドル群」を表す。resource cache の invalidation、前フレームとの差分、viewport サイズ変更の検出など、時間的な状態管理は基本的に持たせない。

## Provider-Owned State

状態変化によって何を無効化するかは provider ごとに違う。そのため、cache invalidation に必要な状態は、まず各 provider が自分で持つ。

例えば viewport サイズ変更を検出して geometry cache を消す必要がある場合、その前回 viewport サイズは geometry resource provider 側に置く。`D2dContext` に `isViewportResized()` のようなメソッドを追加して、全 provider がそれに依存する形にはしない。

## Shared State の切り出し

同じ viewport サイズ比較や frame state の参照が複数 provider に広がり、重複が意味を持ち始めた場合は、`D2dContext` を太らせるのではなく、別の state object を切り出すことを検討する。

候補は `D2dFrameState` や `ViewportState` のような、時間的状態だけを持つ専用オブジェクトである。`D2dContext` は D2D handle の境界に留める。
