# Manual Harness

## Package Identity
- `test/**` is a Windows sample application used for manual renderer verification, not an automated unit-test suite.
- `test/CMakeLists.txt` builds a single `WIN32` executable target named `kke_test` that links against the library target.

## Setup & Run
- Configure repo: `cmake -S . -B build`
- Build harness: `cmake --build build --target kke_test`
- Locate the built exe: `Get-ChildItem -Recurse build -Filter kke_test.exe | Select-Object -First 1 -ExpandProperty FullName`
- Find render scenarios: `rg -n "engine->(draw|fill|blur|push|pop)" test/Renderer.cc`
- Find backend init flow: `rg -n "init\\(|Create|Present|PeekMessage|DispatchMessage" test`
- Build and manual rendering checks are for the user to run. Agents should give commands and validation steps, not execute them, unless explicitly asked.

## Patterns & Conventions
- Keep `test/Main.cc` as the application entry point. It owns `WinMain`, console attachment, the message pump, and the render loop.
- Keep backend setup split by responsibility: `test/backend/D3D11.cc` creates the device/swap-chain/framebuffer, and `test/backend/D2D1.cc` binds Direct2D on top of it.
- Put manual rendering scenarios in `test/Renderer.cc::renderFrame`; that file is the best place to add or tweak visual regression coverage.
- Load embedded resources through `application::Renderer::loadResource` and IDs from `test/resources/resources.h`, matching the existing font and texture flow in `test/Renderer.cc`.
- Keep embedded asset definitions in `test/resources/resources.rc` and payloads in `test/resources/contents/`.
- DO add new manual checks next to similar ones already in `test/Renderer.cc`, for example geometry drawing, transforms, shadows, textures, text, and blur passes.
- DO preserve the begin/render/end lifecycle in `test/Renderer.cc`:
  `engine->begin(...)`
  `renderFrame()`
  `engine->end(...)`
- DO keep backend assertions and Win32-specific setup local to this tree, as shown in `test/backend/D3D11.cc`, `test/backend/D2D1.cc`, and `test/Window.cc`.
- DON'T treat this folder like a Catch2/gtest suite. `test/CMakeLists.txt` only defines a GUI executable, and filenames such as `test/Main.cc` or `test/Renderer.cc` are harness code, not test cases.
- DON'T move `assert(...)`-heavy backend bootstrap code from `test/backend/D3D11.cc` into reusable library code. It is acceptable here as harness scaffolding, but not a library contract.
- DON'T add new sample assets without updating both `test/resources/resources.h` and `test/resources/resources.rc`.

## Key Files
- App entry point and loop: `test/Main.cc`
- Window setup: `test/Window.cc`, `test/Window.hh`
- Manual render scenarios: `test/Renderer.cc`, `test/Renderer.hh`
- D3D11 bootstrap: `test/backend/D3D11.cc`, `test/backend/D3D11.hh`
- D2D1 bootstrap: `test/backend/D2D1.cc`, `test/backend/D2D1.hh`
- Embedded resources: `test/resources/resources.h`, `test/resources/resources.rc`, `test/resources/contents/`

## JIT Index Hints
- Find resource loading sites: `rg -n "loadResource|FindResource|LockResource|SizeofResource" test`
- Find backend/device setup: `rg -n "D3D11CreateDevice|CreateSwapChain|CreateDeviceContext|CreateBitmapFromDxgiSurface" test/backend`
- Find message-loop and presentation code: `rg -n "PeekMessage|DispatchMessage|Present|Sleep" test`
- Find specific manual scenarios: `rg -n "Shadow Test|Texture Test|Text Test|Triangle Test|blur" test/Renderer.cc`

## Common Gotchas
- The resource loader looks up assets with resource type `"WAVE"` even for non-audio payloads; keep that consistent unless you rework the resource script together.
- `kke_test` is a manual visual check, so build success alone does not validate rendering correctness.
- Output path for the executable depends on the generator/configuration; use the search command above instead of hard-coding one path.

## Pre-PR Checks
- User-run only:
- `cmake --build build --target kke_test`
