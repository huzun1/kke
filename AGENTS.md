# C++ Style Guidelines

## Project-Specific Design and Implementation Notes

If the task depends on project-specific architectural or implementation decisions, inspect the relevant code first and derive the local conventions from the existing implementation.

This repository currently does not keep project notes under `docs/arch` or `docs/impl`, so do not assume those directories exist.

If a new project-specific rule, responsibility split, implementation pattern, or caution becomes clear during the work, document it in an appropriate location only if the project later introduces a dedicated place for such notes.

Base environment:
- C++23
- CMake
- Windows platform

## Preprocessor Conventions

Always put `#pragma once` at the top of every header.

When ordering includes, use this sequence:

1. Standard library headers (`cstdint`, `string`, etc.)
2. Third-party library headers (`Plog`, `MinHook`, etc.)
3. Windows-related headers (`Windows.h`, `dwrite.h`, etc.)
4. Project headers (`src/kke/Engine.hh`, etc.)

- Correct example
```cpp
#include <cstdint>
#include <string>
#include <vector>

#include <MinHook.h>
#include <plog/Log.h>

#include <dwrite.h>
#include <dwrite_3.h>
#include <wrl/client.h>

#include "kke/resources/font/FontWeight.hh"
```

## Namespace, Struct, and Class Conventions

### Indentation and Spacing

Do not insert blank lines between consecutive namespace declarations or between a namespace declaration and the next class declaration.

- Incorrect example 1
```cpp
namespace kke {

class DWriteFontWrapper {

...

}

}
```

- Incorrect example 2
```cpp
namespace kke {

namespace oreik {

...

}

}
```

- Correct example 1
```cpp
namespace kke {
class DWriteFontWrapper {
...
}
}
```

- Correct example 2
```cpp
namespace kke {
namespace oreik {
namespace sushi {
} // do not add a blank line here either
}
}
...
```

For namespaces, the next line keeps the same indentation level. For classes, indent the contents by one level.

- Incorrect example
```cpp
class kke {
class oreik {
class sushi {
...
}
}
}
```

- Correct example
```cpp
class kke {
    class oreik {
        class sushi {
        }
    }
}
...
```

Also, always leave one blank line after a method declaration, except after the last method in the group.

- Incorrect example
```cpp
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

- Correct example
```cpp
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

### Member Declarations

In classes and structs, declare data members first.

Do not place member variable declarations between methods.

- Correct example
```cpp
class DWriteFontWrapper {
private:
	Microsoft::WRL::ComPtr<IDWriteFactory5> writeFactory;
	Microsoft::WRL::ComPtr<IDWriteInMemoryFontFileLoader> fontFileLoader;
	Microsoft::WRL::ComPtr<IDWriteFontSetBuilder1> fontSetBuilder;
	Microsoft::WRL::ComPtr<IDWriteFontCollection1> fontCollection;
	bool isRegistered = false;
...
```

- Incorrect example
```cpp
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
} // namespace kke
```

Also, do not mix method declarations into the member declaration block.

If private methods are needed, open a separate private section for them.

- Incorrect example
```cpp
class DWriteFontWrapper {
private:
	bool isRegistered = false;

	DWRITE_FONT_WEIGHT toDWriteWeight(FontWeight weight) const;
...
```

- Correct example
```cpp
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

### Method Ordering

Order methods by dependency, regardless of visibility.

Place externally called methods or higher-level entry points first, and place the helper methods they depend on below them.

- Correct example
```cpp
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

- Incorrect example
```cpp
class GeometryFactory {
private:
	static D2D1_POINT_2F pointToD2d(Point const& point);

	static Microsoft::WRL::ComPtr<ID2D1Geometry> createPathGeometry(D2dContext const& context, std::span<Point const> points);

	static Microsoft::WRL::ComPtr<ID2D1Geometry> create(D2dContext const& context, Triangle const& triangle);
};
```

Keep the implementation order in `.cc` files consistent with the declaration order in headers.

### Private Helper Functions

Avoid standalone functions that exist only in a `.cc` file.

Instead of hiding them in an anonymous namespace, declare them as `private` methods on the relevant class.

- Incorrect example
```cpp
namespace {
D2D1_POINT_2F pointToD2d(Point const& point) {
	return {point.x, point.y};
}
}
```

- Correct example
```cpp
class GeometryFactory {
private:
	static D2D1_POINT_2F pointToD2d(Point const& point);
};

D2D1_POINT_2F GeometryFactory::pointToD2d(Point const& point) {
	return {point.x, point.y};
}
```

If the responsibility is truly independent from the class, consider introducing a dedicated class or a named namespace instead.

### Initialization Style

For simple struct conversions, prefer brace initialization over verbose factory helpers.

- Incorrect example
```cpp
D2D1_POINT_2F GeometryFactory::pointToD2d(Point const& point) {
	return D2D1::Point2F(point.x, point.y);
}
```

- Correct example
```cpp
D2D1_POINT_2F GeometryFactory::pointToD2d(Point const& point) {
	return {point.x, point.y};
}
```
