# D2D Resource Responsibility

この文書は、このプロジェクト固有の実装パターン集である。D2D resource provider や factory の責務分割に関する判断をまとめる。

## Resource 管理と Resource 生成を分ける場合

1 つの provider が cache 管理、素材 resource 管理、D2D resource 生成を同時に持ち始めたら、責務を分ける。

素材 resource の寿命や更新条件を扱う処理は、外部から resource を取得する窓口なら provider に置く。特定 provider の内部部品として閉じるなら、`Cache` や `State` など provider 以外の名前を使う。D2D API を使って新しい resource を生成するだけの処理は factory に置く。

呼び出し元の provider は、cache key の作成、cache の読み書き、必要な provider/factory の接続に集中させる。

## 派生 Resource と素材 Resource を分ける場合

素材 resource から派生 resource を作る場合は、素材 resource と派生 resource の cache を分ける。

素材 resource の無効化条件が派生 resource にも影響する場合は、同じ状態変化のタイミングで両方を無効化する。ただし、無効化の判断は各責務の境界内に置く。

例えば viewport rectangle geometry は素材 resource であり、mask を差し引いた inverted geometry は派生 resource である。この場合、viewport rectangle geometry は `ViewportGeometryCache` のような内部 cache に保持させ、inverted geometry は hash key 付きの cache に置く。

## Helper Method に責務を閉じる場合

helper method には、呼び出し元で確定済みの判断を重複して持ち込まない。

状態変化の判定は状態を同期する method に寄せ、取得 helper は「あれば返す。なければ作る」という取得責務だけを持つ。
