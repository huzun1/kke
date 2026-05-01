# C++コードスタイルガイドライン

## プロジェクト固有の設計・実装ガイド

作業対象に関連する設計や実装の注意点が`docs/arch`や`docs/impl`に存在する場合は、実装前に参照する。

これらのドキュメントは、このプロジェクト固有の判断をまとめたものであり、一般的な C++ や Direct2D のルールとして扱わない。

作業中に新しいプロジェクト固有の判断、責務分割、実装パターン、注意点が明確になった場合は、必要に応じて`docs/arch`や`docs/impl`に新規ファイルを作成、または既存ファイルを更新する。

基本環境:
- C++ std 23
- Cmake
- Windowsプラットフォーム

## プリプロセッサの記法

必ず、ヘッダーには`#pragma once`を最初につける。

基本的に、includeをする際には、

1. stdヘッダー (cstdintやstringなど)
2. ライブラリ関連ヘッダー (Plog, Minhookなど)
2. Windows関連ヘッダー (Windows.hやdwrite.hなど)
3. プロジェクト固有のヘッダー (src/kke/Engine.hhなど)

の順番で行う。

- 正しい例
```
#include <cstdint>
#include <string>
#include <vector>

#include "kke/resources/font/FontWeight.hh"

#include <MinHook.h>
#include <plog/Log.h>

#include <dwrite.h>
#include <dwrite_3.h>
#include <wrl/client.h>
```
## ネームスペース・構造体・クラスの記法

### インデント・行間

ネームスペース宣言或いは、クラス宣言において、次の宣言の間にスペースを入れないようにする。

- 正しくない例 1
```
namespace kke {

class DWriteFontWrapper {

...

}

}
```

- 正しくない例 2
```
namespace kke {

namespace oreik {

...

}

}
```

- 正しい例 1
```
namespace kke {
class DWriteFontWrapper {
...
}
}
```

- 正しい例 2
```
namespace kke {
namespace oreik {
namespace sushi {
} // ここも改行してはならない
}
}
...
```

ネームスペース宣言の場合、次の行のインデントはそのままだが、クラスの場合は、一つずらす。

- 正しくない例
```
class kke {
class oreik {
class sushi {
...
}
}
}
```

- 正しい例
```
class kke {
    class oreik {
        class sushi {
        }
    }
}
...
```

また、メソッド宣言は無条件で次の行を空行にする(最後のメソッドを除く。)

- 正しくない例
```
	DWriteFontWrapper() = default;
	~DWriteFontWrapper();

	void initialize();
	void addFont(const void* data, size_t size);
	void finalizeCollection();

	Microsoft::WRL::ComPtr<IDWriteTextFormat> createTextFormat(
		const std::wstring& fontFamily,
		int32_t fontSize,
		FontWeight weight);

	Microsoft::WRL::ComPtr<IDWriteTextLayout> createTextLayout(
		const std::wstring& text,
		IDWriteTextFormat* textFormat);
```

- 正しい例
```
	DWriteFontWrapper() = default;
    
	~DWriteFontWrapper();

	void initialize();

	void addFont(const void* data, size_t size);

	void finalizeCollection();

	Microsoft::WRL::ComPtr<IDWriteTextFormat> createTextFormat(
		const std::wstring& fontFamily,
		int32_t fontSize,
		FontWeight weight);

	Microsoft::WRL::ComPtr<IDWriteTextLayout> createTextLayout(
		const std::wstring& text,
		IDWriteTextFormat* textFormat);
```

### メンバー宣言について

クラスと構造体の場合に限るが、メンバー宣言に関しては一番最初に宣言を完了させておく。

そして、メソッドの間などに宣言をしない。

- 正しい例

```
class DWriteFontWrapper {
private:
	Microsoft::WRL::ComPtr<IDWriteFactory5> writeFactory;
	Microsoft::WRL::ComPtr<IDWriteInMemoryFontFileLoader> fontFileLoader;
	Microsoft::WRL::ComPtr<IDWriteFontSetBuilder1> fontSetBuilder;
	Microsoft::WRL::ComPtr<IDWriteFontCollection1> fontCollection;
	bool isRegistered = false;
...
```

- 正しくない例

```
...
	Microsoft::WRL::ComPtr<IDWriteTextLayout> createTextLayout(
		const std::wstring& text,
		IDWriteTextFormat* textFormat);

private:
	Microsoft::WRL::ComPtr<IDWriteFactory5> writeFactory;
	Microsoft::WRL::ComPtr<IDWriteInMemoryFontFileLoader> fontFileLoader;
	Microsoft::WRL::ComPtr<IDWriteFontSetBuilder1> fontSetBuilder;
	Microsoft::WRL::ComPtr<IDWriteFontCollection1> fontCollection;
	std::vector<std::vector<uint8_t>> fontDataStorage;
	bool isRegistered = false;

	DWRITE_FONT_WEIGHT toDWriteWeight(FontWeight weight) const;
};
}  // namespace kke
```

また、この際の注意点として、メソッドを宣言として含めてはならない。

プライベートメソッドが必要な場合は、別で新しく非公開スコープを作成する。

- 正しくない例
```
class DWriteFontWrapper {
private:
	bool isRegistered = false;

	DWRITE_FONT_WEIGHT toDWriteWeight(FontWeight weight) const;
...
```

- 正しい例
```
class DWriteFontWrapper {
private:
	bool isRegistered = false;

public:
...

private:
	DWRITE_FONT_WEIGHT toDWriteWeight(FontWeight weight) const;
...
}
```

### メソッドの配置順

メソッドは公開/非公開に関わらず、依存順に配置する。

外部から呼ばれるメソッド、或いは呼び出し元になるメソッドを上に置き、そのメソッドが依存する補助メソッドを下に置く。

- 正しい例
```
class GeometryFactory {
public:
	static Microsoft::WRL::ComPtr<ID2D1Geometry> create(D2dContext const& context, Geometry const& geometry);

private:
	static Microsoft::WRL::ComPtr<ID2D1Geometry> create(D2dContext const& context, Triangle const& triangle);

	static Microsoft::WRL::ComPtr<ID2D1Geometry> create(D2dContext const& context, Rect const& rect);

	static Microsoft::WRL::ComPtr<ID2D1Geometry> create(D2dContext const& context, Polygon const& polygon);

	static D2D1_POINT_2F pointToD2d(Point const& point);

	static Microsoft::WRL::ComPtr<ID2D1Geometry> createPathGeometry(D2dContext const& context, std::span<Point const> points);
};
```

- 正しくない例
```
class GeometryFactory {
private:
	static D2D1_POINT_2F pointToD2d(Point const& point);

	static Microsoft::WRL::ComPtr<ID2D1Geometry> createPathGeometry(D2dContext const& context, std::span<Point const> points);

	static Microsoft::WRL::ComPtr<ID2D1Geometry> create(D2dContext const& context, Triangle const& triangle);
};
```

`.cc`側の実装順も、ヘッダーの宣言順と同じにする。

### 非公開ヘルパー関数

`.cc`だけに存在する単体関数は避ける。

匿名ネームスペースで関数を隠すのではなく、対象クラスの`private`メソッドとしてヘッダーに宣言する。

- 正しくない例
```
namespace {
D2D1_POINT_2F pointToD2d(Point const& point) {
	return {point.x, point.y};
}
}
```

- 正しい例
```
class GeometryFactory {
private:
	static D2D1_POINT_2F pointToD2d(Point const& point);
};

D2D1_POINT_2F GeometryFactory::pointToD2d(Point const& point) {
	return {point.x, point.y};
}
```

ただし、クラスに所属しない独立した責務がある場合は、専用のクラスや名前付きnamespaceを検討する。

### 初期化の記法

単純な構造体への変換では、冗長なファクトリ関数よりも波括弧初期化を優先する。

- 正しくない例
```
D2D1_POINT_2F GeometryFactory::pointToD2d(Point const& point) {
	return D2D1::Point2F(point.x, point.y);
}
```

- 正しい例
```
D2D1_POINT_2F GeometryFactory::pointToD2d(Point const& point) {
	return {point.x, point.y};
}
```

