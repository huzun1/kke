# C++コードスタイルガイドライン

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

