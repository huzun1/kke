# Public Headers

## Package Identity
- `includes/kke/**` defines the installed API surface for the `kke` static library.
- Most headers map directly to one implementation file under `src/**`; the notable exception is template-heavy code such as `includes/kke/effect/EffectContainer.hh`.

## Setup & Run
- Configure repo: `cmake -S . -B build`
- Build library target: `cmake --build build --target kke`
- Format headers and sources: `python format-code.py`
- Find exported header for a type: `rg -n "class TypeName|struct TypeName|enum class TypeName" includes/kke`
- Find matching implementation: `rg --files src | rg "Engine|ResourceAllocator|ShadowDispatcher|LinearGradientBrush|BlurEffect"`
- Build commands are for the user to run. Agents should cite them, not execute them, unless explicitly requested.

## Patterns & Conventions
- Keep public include paths stable under `kke/...`; new headers should follow the same install-friendly layout as `includes/kke/brush/LinearGradientBrush.hh` and `includes/kke/common/geometry/Rect.hh`.
- Pair nontrivial headers with one implementation file in `src/`, for example `includes/kke/Engine.hh` <-> `src/Engine.cc` and `includes/kke/brush/LinearGradientBrush.hh` <-> `src/brush/LinearGradientBrush.cc`.
- Keep template-only logic in headers only when needed for compilation, as in `includes/kke/effect/EffectContainer.hh`.
- Preserve the existing `namespace kke` boundary and `.hh` extension everywhere in this tree.
- Favor lightweight value types and explicit ownership in the public API, following `includes/kke/common/Point.hh`, `includes/kke/common/Color.hh`, and `includes/kke/font/FontLoader.hh`.
- Public geometry, brush, and effect types should stay consistent with the hashable/cacheable model already used by `includes/kke/brush/Brush.hh` and `includes/kke/brush/LinearGradientBrush.hh`.
- DO mirror any public API addition in both declaration and implementation, using `includes/kke/effect/impl/BlurEffect.hh` and `src/effect/impl/BlurEffect.cc` as the reference pattern.
- DO keep nested feature groups under clear subfolders such as `brush/`, `common/geometry/`, `effect/impl/`, and `transform/impl/`.
- DO use concrete file examples when adding new families:
  `includes/kke/common/geometry/Triangle.hh`
  `includes/kke/transform/impl/ScaleTransform.hh`
- DON'T add new `using namespace` directives to public headers. Existing directives in `includes/kke/Engine.hh`, `includes/kke/ResourceAllocator.hh`, `includes/kke/font/FontLoader.hh`, and `includes/kke/TextureRepository.hh` should be treated as legacy, not copied forward.
- DON'T silently rename exported spellings such as `ShadowDisaptcher` in `includes/kke/ShadowDispatcher.hh` or `TranslateTransfrom.hh` in `includes/kke/transform/impl/TranslateTransfrom.hh` unless the change is coordinated across the whole tree.
- DON'T expose temporary or workaround APIs without documenting why; `includes/kke/Engine.hh` already marks `drawBitmap` with a `FIXME`, so treat new exceptions the same way.

## Key Files
- Public entry point: `includes/kke/Engine.hh`
- Resource/caching API: `includes/kke/ResourceAllocator.hh`
- Surface/texture APIs: `includes/kke/RenderSurface.hh`, `includes/kke/TextureRepository.hh`
- Geometry/value types: `includes/kke/common/Point.hh`, `includes/kke/common/geometry/Rect.hh`, `includes/kke/common/geometry/RoundedRect.hh`
- Effects: `includes/kke/effect/Effect.hh`, `includes/kke/effect/EffectContainer.hh`, `includes/kke/effect/impl/BlurEffect.hh`
- Transform stack: `includes/kke/transform/Matrix.hh`, `includes/kke/transform/Transform.hh`, `includes/kke/transform/impl/TranslateTransfrom.hh`

## JIT Index Hints
- Find headers with WRL/COM exposure: `rg -n "ComPtr|ID2D1|IDWrite|IWIC" includes/kke`
- Find public APIs related to effects: `rg -n "Effect|blur|draw.*Shadow" includes/kke`
- Find geometry/value types: `rg -n "class (Rect|RoundedRect|Ellipse|Triangle)|struct .*Point|Color" includes/kke/common includes/kke/brush`
- Find header/source pairs: `rg -n "TypeName" includes/kke src`

## Common Gotchas
- `src/CMakeLists.txt` installs this tree directly, so path/layout changes affect consumers.
- `TranslateTransfrom.hh` and `ShadowDisaptcher` are current exported spellings; fix them only as a deliberate breaking change.
- Because Direct2D/WRL types appear in public headers, "small" signature changes can become ABI and platform-support changes.

## Pre-PR Checks
- User-run only:
- `python format-code.py; if ($?) { cmake --build build --target kke }`
