# D2D Resource Caching

この文書は、このプロジェクト固有の実装パターン集である。D2D resource を cache するときの判断基準をまとめる。

## 入力条件に依存する Resource をキャッシュする場合

viewport サイズ、transform、描画設定など、外部状態に依存して D2D resource の結果が変わる場合は、その依存条件を cache key に含める。

依存条件が key に含まれている cache は、毎フレーム無条件にクリアしない。条件が同じなら再利用し、条件が変わった時だけ無効化する。

`KeyCacheStorage` を使う場合は、resource の寿命管理を storage に任せる。独自の `std::unordered_map` を増やす前に、key-value cache として表現できないか検討する。

## Composite Resource をキャッシュする場合

複数の project-side resource から 1 つの D2D resource を作る場合は、composite 全体の構造を表す hash を cache key にする。

要素の順序や値が結果に影響する場合は、それらを hash に含める。個々の要素を既存 provider 経由で取得できるなら、composite 側で要素生成ロジックを重複実装しない。

## フレーム境界でキャッシュを更新する場合

フレーム境界メソッドでは、毎フレーム必ず破棄する処理ではなく、状態変化の検出を優先する。

例えば viewport サイズに依存する cache では、`syncViewportSize(D2dContext const& context)` で現在の viewport サイズを読み、前回サイズと比較する。サイズが変わった場合だけ、関連する cache と派生 resource をクリアする。

## Context に Cache 状態を押し込まない

外部状態の変化を知るためだけに `D2dContext` に前回値や差分判定を追加しない。`D2dContext` は D2D handle へのアクセス境界に留め、cache invalidation に必要な状態は provider 側で持つ。

同じ viewport サイズ比較が複数 provider に増えてきた場合は、`D2dContext` へ追加する前に、専用の frame state や viewport state として分離できないか検討する。
