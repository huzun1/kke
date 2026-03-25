# kke

`kke` is a Windows-focused C++20 renderer library built on top of Direct2D, DirectWrite, WIC, DXGI, and D3D11-compatible surfaces.

The repository currently contains:

- `kke`: a static library with reusable drawing, text, texture, shadow, blur, and transform APIs
- `kke_test`: a Win32 sample harness for manual rendering checks
- `kke_unit_tests`: a deterministic non-GUI test executable registered with CTest

## Requirements

- Windows
- CMake 3.10 or newer
- A C++20-capable compiler
- Direct2D / DirectWrite / WIC development libraries available through the Windows SDK
- `clang-format` if you want to run the repository formatter

## Configure

```powershell
cmake -S . -B build
```

## Build

Build the library:

```powershell
cmake --build build --target kke
```

Build the deterministic unit tests:

```powershell
cmake --build build --target kke_unit_tests
ctest --test-dir build --output-on-failure
```

Build the manual renderer harness:

```powershell
cmake --build build --target kke_test
Get-ChildItem -Recurse build -Filter kke_test.exe | Select-Object -First 1 -ExpandProperty FullName
```

## Manual Validation

After building `kke_test`, verify at least:

- basic primitives: rect, rounded rect, ellipse, triangle
- text rendering and shadowed text
- texture rendering
- transform stack behavior during rotation and scaling
- blur and shadow output without crashes or obvious corruption

The main visual coverage lives in `test/Renderer.cc`.

## Formatting

```powershell
python format-code.py
```

## CMake Package Usage

The install step exports a config package so consumers can use `find_package`.

```cmake
find_package(kke CONFIG REQUIRED)

add_executable(app main.cc)
target_link_libraries(app PRIVATE kke::kke)
```

## Minimal Usage Sketch

```cpp
#include <kke/Engine.hh>
#include <kke/brush/SolidColorBrush.hh>
#include <kke/common/geometry/Rect.hh>

void render(ID2D1Factory* factory, ID2D1DeviceContext* context, ID2D1Bitmap1* backBufferTarget) {
	kke::Engine engine(factory, context);
	engine.init({});

	engine.begin(backBufferTarget);
	engine.fillRect(
		kke::Rect{20.0f, 20.0f, 180.0f, 120.0f},
		kke::SolidColorBrush({0.1f, 0.5f, 0.9f, 1.0f}));

	ID2D1Image* output = nullptr;
	engine.end(&output);

	context->SetTarget(backBufferTarget);
	context->BeginDraw();
	context->DrawImage(output);
	context->EndDraw();
	output->Release();
}
```

## Notes

- The public API is Windows-specific today.
- The sample harness is for manual verification, not visual goldens.
- Some low-level Direct2D types are still part of the public surface where the API depends on external render targets and images.
