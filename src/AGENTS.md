# Library Implementation

## Package Identity
- `src/**` contains the concrete implementation of the `kke` static library declared in `includes/kke/**`.
- This layer owns Direct2D/DirectWrite/WIC interactions, resource caching, effect dispatch, geometry conversion, and transform stack behavior.

## Setup & Run
- Configure repo: `cmake -S . -B build`
- Build library target: `cmake --build build --target kke`
- Format changed library files: `python format-code.py`
- Find implementation for a public type: `rg -n "TypeName" src includes/kke`
- Find allocator/cache usage: `rg -n "acquireOrCreate|acquireOrDispatch|nextFrame" src`
- Build commands are for the user to run. Agents should not invoke them automatically.

## Patterns & Conventions
- Keep one implementation unit per public feature where practical, matching the structure already used by `src/Engine.cc`, `src/TextureRepository.cc`, `src/effect/impl/BlurEffect.cc`, and `src/transform/impl/ScaleTransform.cc`.
- Route draw-time resource creation through `ResourceAllocator`, `TextureRepository`, `ShadowDispatcher`, and `EffectContainer` instead of open-coding duplicate caches.
- Use `Microsoft::WRL::ComPtr` for owned COM objects inside implementations, as shown in `src/Engine.cc`, `src/ResourceAllocator.cc`, and `src/common/geometry/Triangle.cc`.
- Keep frame lifecycle logic centralized in `Engine::begin`, `Engine::flush`, and `Engine::end`; new code should not introduce side-channel state resets outside that flow.
- Reset transforms after mutation and effect application, following the existing push/pop and `effect(...)` patterns in `src/Engine.cc`.
- When adding new geometry, follow the existing declaration/implementation split used by `src/common/geometry/Rect.cc`, `src/common/geometry/RoundedRect.cc`, and `src/common/geometry/Triangle.cc`.
- When adding new effects, mirror `src/effect/impl/BlurEffect.cc` plus `includes/kke/effect/impl/BlurEffect.hh` and wire usage through `EffectContainer` and `Engine`, not directly from the sample app.
- Keep cache keys deterministic and derived from value inputs, following `src/brush/LinearGradientBrush.cc::hash` and the custom hashing path in `src/Engine.cc::drawLineShadow`.
- DO reuse allocator helpers from `src/Engine.cc` such as `resourceAllocator.acquireOrCreateBrush(...)`, `acquireOrCreateGeometry(...)`, and `acquireOrDispatchShadow(...)`.
- DO keep math-only transform behavior in `src/transform/**`; `src/transform/Matrix.cc` and `src/transform/impl/RotateTransform.cc` are the reference shape.
- DO keep Windows UI and swap-chain ownership out of this tree; those concerns live in `test/Main.cc` and `test/backend/*.cc`.
- DON'T copy raw sample-only drawing patterns from `test/Renderer.cc::preRender` into library code when an engine/resource-allocator path already exists.
- DON'T add new entry-point, message-loop, or window procedures under `src/`; that would blur the library/harness boundary that `test/` already establishes.
- DON'T "fix" exported typo spellings only in source files; `src/ShadowDispatcher.cc` and `src/transform/impl/TranslateTransform.cc` must stay aligned with their public headers until a coordinated rename happens.

## Key Files
- Core orchestration: `src/Engine.cc`
- Resource cache/allocation: `src/ResourceAllocator.cc`
- Texture loading/cache: `src/TextureRepository.cc`
- Shadow/effect dispatch: `src/ShadowDispatcher.cc`, `src/effect/EffectInstance.cc`, `src/effect/impl/BlurEffect.cc`
- Fonts/text layout: `src/font/FontLoader.cc`
- Geometry conversion: `src/common/geometry/Rect.cc`, `src/common/geometry/Ellipse.cc`, `src/common/geometry/Triangle.cc`
- Transform stack: `src/transform/Matrix.cc`, `src/transform/impl/TranslateTransform.cc`

## JIT Index Hints
- Find all draw/fill entry points: `rg -n "^void Engine::(draw|fill|blur|push|pop)" src/Engine.cc`
- Find cache/resource code paths: `rg -n "acquireOrCreate|acquireOrDispatch|Cache|hash\\(" src`
- Find effect implementations: `rg --files src/effect src/effect/impl`
- Find geometry implementations: `rg --files src/common/geometry`
- Find transform implementations: `rg --files src/transform src/transform/impl`

## Common Gotchas
- `Engine` is stateful across `begin`/`flush`/`end`; changes there can break every manual render path.
- Many public APIs still pass raw Direct2D pointers; be explicit about ownership and release behavior when extending them.
- `ShadowDisaptcher` and `TranslateTransfrom.hh` are existing typos wired through the build and include graph.

## Pre-PR Checks
- User-run only:
- `python format-code.py; if ($?) { cmake --build build --target kke }`
