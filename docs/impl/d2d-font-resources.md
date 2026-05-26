# D2D Font Resources

この文書は、このプロジェクト固有の実装メモである。D2D/DWrite font resource の入口に関する判断をまとめる。

## uploadFont

font data は `Engine::uploadFont(void const* data, size_t size)` から登録する。

texture の `uploadTexture` と同じく、project-side resource として `std::shared_ptr<Font>` を返す。D2D 実装では `D2dFont` が font data のコピーを保持する。

## beginDraw に依存しない

font upload は描画フレームの外で行える必要がある。そのため、`D2dEngine::uploadFont` は現在の `D2dContext` を要求しない。

DWrite font collection や text layout 生成など、D2D/DWrite handle が必要な処理は後続の provider/factory 側で行う。

## FontProvider

`FontProvider` は登録済み font resource を保持する。Text rendering と text measurement は、後続でこの provider から font data を参照する。

## TextFormatProvider と TextLayoutProvider

`TextFormatProvider` は `FontAppearance` から `IDWriteTextFormat` を取得し、同じ appearance に対する format を cache する。

`TextLayoutProvider` は `Text` から `IDWriteTextLayout` を取得し、同じ text content と appearance に対する layout を cache する。text position は layout の内容には影響しないため、cache key には含めない。

font collection が再構築された場合、text format と text layout は collection に依存するため、両方の cache を clear する。

## FontFactory

DWrite API を直接呼び出して `IDWriteTextFormat` や `IDWriteTextLayout` を作る処理は `FontFactory` に閉じ込める。provider は cache key の作成、cache の読み書き、factory の接続に集中する。
