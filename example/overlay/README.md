![Thumbnail](./img/image.png)

# D3D11 Overlay Example

This example builds an injectable Windows DLL that uses Kiero to discover the Direct3D 11
swap-chain methods, MinHook to detour them, and kke to draw through Direct2D on the current
back buffer.

## Build

```powershell
cmake -S . -B build -G Ninja `
    -DCMAKE_C_COMPILER=clang-cl.exe `
    -DCMAKE_CXX_COMPILER=clang-cl.exe `
    -DCMAKE_BUILD_TYPE=RelWithDebInfo `
    -DKKE_BUILD_EXAMPLES=ON `
    -DKKE_BUILD_TESTS=OFF `
    -DKKE_BUILD_BENCHMARKS=OFF
cmake --build build --target kke_overlay_example
```

Run these commands from an x64 developer environment so clang-cl can find the Windows SDK
and MSVC-compatible libraries. The overlay configuration rejects 32-bit toolchains.

The resulting `kke_overlay_example.dll` must be loaded into a Direct3D 11 process by a DLL
loader or injector. Loading tools are intentionally outside this example's scope. Hooks are
removed from `DLL_PROCESS_DETACH` when the DLL is unloaded.

This example builds a 64-bit DLL and must be loaded into a 64-bit target process.

Only load the example into software you own or are authorized to test.

## Structure

- `Hooks`: installs and removes the Present and ResizeBuffers detours.
- `D2dEntrance`: connects the hooked DXGI swap chain to a kke Direct2D frame.
- `D3d11ContextIsolation`: saves and restores the host application's immediate-context
  state around overlay rendering. Rendering is skipped when this D3D11.1 facility is unavailable.
- `PageController`: handles the function keys, selects a page, and draws the key guide.
- `pages`: contains one renderer for each F6-F9 page.

## Controls

- `F6`: fill, border, gradient, and text
- `F7`: animated rotation, scaling, and translation
- `F8`: shadow and clipping
- `F9`: full-frame blur

The control guide remains visible in the upper-left corner. The example does not clear the
host back buffer and restores the Direct3D 11 device-context state after every overlay frame.

The text sample loads `Segoe UI` from the Windows Fonts directory at runtime, so no font asset
is copied into the DLL.
