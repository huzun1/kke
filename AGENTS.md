# kke

## Project Snapshot
- Single-package C++ renderer library with one static library target (`kke`) and one Windows sample target (`kke_test`).
- Build system is plain CMake; no monorepo/workspace tooling is present.
- Public API lives in `includes/kke/**`, implementation lives in `src/**`, and manual rendering verification lives in `test/**`.
- The codebase is Windows-specific today because public and private code depend on Direct2D, Direct3D 11, DXGI, Win32, and WRL types.
- Read the nearest `AGENTS.md` before editing: [`includes/AGENTS.md`](includes/AGENTS.md), [`src/AGENTS.md`](src/AGENTS.md), [`test/AGENTS.md`](test/AGENTS.md).

## Root Setup Commands
- Configure: `cmake -S . -B build`
- Build everything: `cmake --build build`
- Build library only: `cmake --build build --target kke`
- Build manual harness only: `cmake --build build --target kke_test`
- Format tracked library code: `python format-code.py`
- Find built sample exe: `Get-ChildItem -Recurse build -Filter kke_test.exe | Select-Object -First 1 -ExpandProperty FullName`

## Universal Conventions
- Use C++20 and keep header/source extensions consistent with the repo: public headers use `.hh`, implementations use `.cc`.
- Follow `.clang-format`: Google base style, tabs enabled, width 4, no column limit.
- Prefer matching public header and implementation pairs, for example `includes/kke/Engine.hh` <-> `src/Engine.cc`.
- Keep Windows harness code in `test/`; do not move WinMain, swap-chain setup, or resource-script concerns into the reusable library unless the API is being redesigned.
- Treat existing exported spellings as stable unless you are doing a coordinated rename across headers, sources, and callers.
- Builds and manual verification are user-run in this repository. Agents should provide the exact commands, but should not run `cmake --build ...` or launch `kke_test` unless the user explicitly asks.

## Security & Assets
- Do not commit credentials, API keys, or machine-local paths. There are no secret-management helpers in this repo.
- Keep binary assets scoped to `test/resources/contents/` unless a new install/runtime asset path is intentionally introduced.
- Be careful with third-party fonts and images added to `test/resources`; only add files that are expected to ship with the sample harness.

## JIT Index

### Package Structure
- Public API: `includes/` -> [`includes/AGENTS.md`](includes/AGENTS.md)
- Library implementation: `src/` -> [`src/AGENTS.md`](src/AGENTS.md)
- Manual rendering harness: `test/` -> [`test/AGENTS.md`](test/AGENTS.md)

### Quick Find Commands
- Find exported types: `rg -n "^(class|struct|enum class) " includes/kke`
- Find header/source pair for a symbol: `rg -n "SymbolName" includes src`
- Find Direct2D/Direct3D usage: `rg -n "ID2D1|ID3D11|IDXGI|ComPtr" includes src test`
- Find manual rendering scenarios: `rg -n "engine->(draw|fill|blur|push|pop)" test/Renderer.cc`
- Find install/build target definitions: `rg -n "add_library|add_executable|install\\(" CMakeLists.txt src test`

## Definition Of Done
- Changed C++ files are formatted with the repository style.
- Relevant CMake build and manual validation commands are identified for the user to run.
- Public API edits are mirrored between `includes/` and `src/` and reflected in the sample harness when behavior changes.
- Rendering changes include a note about how the user should validate them in `kke_test`.
