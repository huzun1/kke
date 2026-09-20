# Cached shadow benchmark

Measures KKE's existing shadow cache hit path against no shadow, forced cache
misses, the uncached effect path, and DrawBitmap for cached images. The production
shadow renderer uses DrawBitmap for bitmap output with source-over primitive
blending and no explicit effect clip. Benchmark-only overrides retain the old
DrawImage path for comparison and can bypass the cache.

Build and run from the KKE repository (Windows, clang-cl, Ninja):

```powershell
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_CXX_COMPILER=clang-cl.exe -DKKE_BUILD_BENCHMARKS=ON
cmake --build build --target kke_cached_shadow_benchmark -j 6
./build/benchmarks/cached_shadow_benchmark/kke_cached_shadow_benchmark.exe 300 6
# Compare only cached DrawImage versus cached DrawBitmap, alternating order:
./build/benchmarks/cached_shadow_benchmark/kke_cached_shadow_benchmark.exe 600 8 bitmap
# Compare full, half, and quarter resolution shadow caches:
./build/benchmarks/cached_shadow_benchmark/kke_cached_shadow_benchmark.exe 600 9 resolution
```

Arguments are samples per scenario (1..10000, default 300) and rounds (1..30,
default 6). The optional third argument `bitmap` selects only the two bitmap
drawing scenarios; `resolution` selects three cache resolutions, all drawn with
the production DrawBitmap path and linear interpolation. Output has three
metadata lines followed by a CSV header and rows.
Hardware D3D11 is required; there is no WARP fallback. GPU query failure, cache
count mismatch, or a failed shadow pixel check makes the process exit nonzero.

## Workload

- Static 1, 6, or 24 rounded panels on a 1920x1080 offscreen target at 96 DPI.
  Panel dimensions are `(140 + index * 2) x 36`, with radius 8. Distinct widths
  give each panel its own cache entry. Panels do not overlap.
- HUD shadow settings: offset `(0, 4)`, sigma 8, black alpha 0.52,
  `OuterShadowOnly`. Identical background fills in every scenario.
- `no_shadow`: background fills only.
- `cached_shadow`: shadow plus background, with a viewport axis clip around
  each shadow, matching the inherited clip used by the haichi effect path.
  This scenario forces the old DrawImage path, even after production adoption.
- `cached_no_clip`: the same cached shadows without those axis clips.
- `cached_bitmap`: identical to `cached_shadow`, replacing its final DrawImage
  with DrawBitmap. Destination is the cached bitmap's DIP size at the cache's
  target offset, opacity 1, linear interpolation, default source-over blending.
  The production implementation's primitive-blend check, QueryInterface, GetSize,
  and the benchmark draw counter are included in CPU timing.
- `half_resolution` / `quarter_resolution` (resolution suite only): rasterize
  the shadow effect output at half/quarter width and height, preserving its
  logical size using bitmap DPI metadata (48/24 DPI) and drawing at the original
  destination with linear interpolation. The context and target remain 96 DPI.
  Bitmap dimensions round up, so padding may extend by up to a low-resolution
  texel. Geometry, sigma, offset, alpha, and background fill remain unchanged.
- `cache_miss`: the clipped shadow path, changing offset Y by 0.0001 per frame
  to force regeneration. This also measures cache insertion and eventual
  eviction; it is not an isolated blur-kernel benchmark.
- `cache_disabled`: identical shape, offset, appearance, and clip to
  `cached_shadow`, but bypasses KKE's shadow bitmap cache. The existing uncached
  renderer builds the effect chain from the geometry's command list and draws
  its output directly. D2D effect-object pooling remains enabled in both paths.
  Since there are no cache lookups, both hit and miss counts must be zero.
- Fresh KKE engine per scenario. Warm up 60 frames, then measure 300 by default.
  Rotate scenario order each round; six rounds exercise every order position
  in the full suite. The two-case bitmap suite alternates order each round.
  The three-case resolution suite rotates order every round (use a multiple
  of three rounds for balanced order positions).
  Check expected hit/miss counts on every frame, including warmup. Verify a
  shadow pixel outside the first panel once per scenario, outside timing.

The executable compiles a generated copy of `ShadowEffectRenderer.cc` in place
of the static library's translation unit. CMake changes only its cache-entry
condition to consult `ShadowCacheControl`, checks that exactly one condition
matches, and regenerates the copy when the source changes. This switch is local
to the single-threaded benchmark process; it does not add a production API or
disable D2D's internal optimizations. A source refactor that changes the matched
condition requires updating the benchmark override.

A second generated override of `EffectRenderer.cc` gates and counts the call to
the production `tryDrawBitmap` method. `ShadowBitmapDrawing` enables that call
only in the selected scenario; it contains no duplicate drawing implementation.
Non-bitmap shadow output, non-source-over primitive blending, and explicit
SOURCE_COPY effect clipping retain DrawImage. Every measured frame checks the number of bitmap
draws, so a silent fallback cannot pass as a successful DrawBitmap measurement.

Before timing, compare full BGRA output in 48 cases: 1/6/24 panels, identity /
fractional translation / nonuniform scale / rotation, transparent or opaque
background, and full or partial external clip. The two APIs draw the same cache
entries. Reject differences exceeding 1/255 in any channel. Readback is outside
all timed samples. This tests 96 DPI, linear sampling, source-over and these
transforms; it is not validation for every D2D compositing mode or DPI.

The resolution suite instead exports 18 cropped PPM images to the working
directory, named `shadow-quality-{scene}-{mode}.ppm`, and writes channel-error
metrics to stderr. It compares one panel at integer/fractional positions and
nonuniform scale on transparent/opaque backgrounds, after checking cold/hit
counts. Each crop is the same 320x160 region of the 1920x1080 target. Transparent
output is composited over gray for PPM export. Error metrics use the original
BGRA values including alpha, not the gray composite; ROI averages include
unchanged background pixels. There is no image-equality threshold for this
deliberately lossy experiment. Export and readback occur before timed samples.

A benchmark-only generated `PositionIndependentEffectCache.cc` switches bitmap
creation to `ShadowRasterization` for reduced resolutions and records actual
allocated pixel bytes. Scale is fixed for an engine's entire lifetime; the
production cache key and behavior are not changed. Full resolution uses the
original bitmap creation method. Scaling the effect graph's output does not
establish how D2D allocates or evaluates intermediate effect buffers, and this
benchmark does not measure regeneration cost after warmup.

## Metrics and limits

Each CSV row reports the upper median and p95 per-frame time in milliseconds:

- `record`: CPU wall time for beginDraw, clear, and recording all panel draws.
- `submit`: CPU wall time inside KKE endDraw (command-list playback/submission).
- `cpu`: record + submit for each frame, then summarized.
- `gpu_elapsed`: D3D11 timestamp interval surrounding the frame. This can include
  CPU submission gaps and GPU scheduling delays; it is not GPU busy time.
- `completed`: wall time from beginDraw through explicit flush and waiting for
  the GPU queries. Includes synchronization overhead.

Hits, misses, geometric layers, axis clips, and bitmap draws are totals over measured frames.
`generated_bitmap_bytes` and `generated_bitmaps` include warmup allocations.
For static cached scenarios there is exactly one bitmap per panel, so bytes
represent the sum of cached pixel payloads (excluding driver alignment and
resource overhead); for forced misses they are cumulative allocation totals.
CPU and GPU intervals overlap; do not add them. There is no pixel readback or
Present in the timed region. Waiting after each frame prevents overlap between
frames, so these numbers are latency measurements, not game throughput or FPS.
No text, layout, game rendering, backdrop blur, or HUD frame construction is
included. Power state and other machine load are uncontrolled.

## Reduced cache resolution: Intel Iris Xe, 2026-09-21

600 measured frames after 60 warmup frames, nine rotated rounds. All variants
use the production DrawBitmap path with linear interpolation. Values are
medians of per-round medians in ms, measured together in this run:
[results/iris-xe-resolution.csv](results/iris-xe-resolution.csv).

| Six panels | CPU | GPU elapsed | Through GPU completion | Cached pixel bytes |
| --- | ---: | ---: | ---: | ---: |
| Full resolution | 0.268 | 0.088 | 0.432 | 387,408 |
| Half width/height | 0.252 | 0.084 | 0.412 | 97,104 |
| Quarter width/height | 0.264 | 0.087 | 0.425 | 24,444 |

All 81 result rows passed per-frame counts: no measured misses, exactly one
generated bitmap per panel after warmup, and matching bitmap draw/axis clip
counts. Each six-panel mode had 32,400 measured hits. Cached pixel bytes fell
by about 75% and 94%, but a stable rendering speedup was not established.

For six panels, half resolution was faster on CPU in 6/9 paired rounds, with
delta ranging from -0.045 to +0.109 ms (median -0.023 ms). Quarter resolution was
faster in 4/9 rounds (median delta +0.004 ms). At 24 panels, CPU medians were
0.599 / 0.600 / 0.604 ms for full/half/quarter, and completion medians were
0.991 / 0.988 / 0.993 ms. The reduced source texture does not reduce output
area, panel count, or command-list playback. GPU elapsed improvements were small
and the measurements remain subject to uncontrolled clocks and background load.

Quality output: [comparison image](results/iris-xe-resolution-quality.png) and
[all six scene metrics](results/iris-xe-resolution-quality.txt). The comparison
shows integer placement on an opaque background, a 4x nearest-neighbor edge
crop, and an 8x amplified absolute RGB difference. These are visualization
magnifications, not the interpolation used by the renderer.

Maximum BGRA channel error across the six scenes was 25/255 at half resolution
and 40/255 at quarter resolution. On the opaque integer-position scene, RGB
errors peaked at 15/255 and 24/255. Differences concentrate near the panel edge:
OuterShadowOnly includes a sharp inner cutout as well as the soft blur, so
downsampling the entire cached result softens that boundary. Full-frame/ROI
average errors dilute this localized artifact; use the edge crop and maxima.

For these small cached HUD shadows, the reliable benefit is reduced image
storage, not rendering speed. The quality tradeoff is not justified by the
observed timing difference. The production cache remains at full resolution.

## Production implementation verification: Intel Iris Xe, 2026-09-21

After adopting the optimization in `EffectRenderer`, repeated `600 8 bitmap`
with the benchmark calling the production method. Values are medians of the
eight per-round medians, in ms. Raw output:
[results/iris-xe-production-bitmap.csv](results/iris-xe-production-bitmap.csv).

| Panels | API | CPU | GPU elapsed | Through GPU completion |
| --- | --- | ---: | ---: | ---: |
| 1 | DrawImage | 0.163 | 0.063 | 0.376 |
| 1 | DrawBitmap | 0.158 | 0.058 | 0.364 |
| 6 | DrawImage | 0.378 | 0.231 | 0.773 |
| 6 | DrawBitmap | 0.286 | 0.184 | 0.617 |
| 24 | DrawImage | 0.901 | 0.847 | 1.924 |
| 24 | DrawBitmap | 0.604 | 0.646 | 1.428 |

All 48 timing rows passed cache and draw-count checks. Pixel comparison again
passed 48 cases with maximum channel difference 1/255. The production-linked
`kke_shadow_bitmap_drawing_test` additionally checks colored translucent shadow
composition over an opaque background, translation of reused cache entries,
explicit SOURCE_COPY clips, and the uncached Canvas fallback.

## Initial DrawImage versus DrawBitmap experiment: Intel Iris Xe, 2026-09-21

600 measured frames after 60 warmup frames, eight alternating rounds, hardware
D3D11, RelWithDebInfo. Values are medians of the eight per-round medians, in ms.
Raw output: [results/iris-xe-draw-bitmap.csv](results/iris-xe-draw-bitmap.csv).

| Panels | API | CPU | GPU elapsed | Through GPU completion |
| --- | --- | ---: | ---: | ---: |
| 1 | DrawImage | 0.167 | 0.062 | 0.378 |
| 1 | DrawBitmap | 0.161 | 0.057 | 0.363 |
| 6 | DrawImage | 0.395 | 0.232 | 0.788 |
| 6 | DrawBitmap | 0.303 | 0.185 | 0.643 |
| 24 | DrawImage | 0.934 | 0.894 | 1.988 |
| 24 | DrawBitmap | 0.617 | 0.649 | 1.448 |

For six panels, each API had 28,800 cache hits and zero misses. DrawBitmap made
28,800 verified bitmap draws; DrawImage made zero. Both had zero geometric
layers and six axis clips per measured frame. All 48 result rows passed their
per-frame cache/draw-count checks.

DrawBitmap was faster in all eight paired rounds for six and 24 panels. The
six-panel CPU improvement ranged from 0.049 to 0.221 ms (median paired delta
0.091 ms); through completion it ranged from 0.084 to 0.309 ms (median 0.158 ms).
Most CPU savings were in endDraw: medians 0.298 versus 0.222 ms for six panels,
and 0.750 versus 0.442 ms for 24. One-panel CPU and completion differences were
small and noisy, with a reversed result in some rounds.

Pixel verification passed all 48 cases: maximum channel difference 1/255,
2,304 differing channels total. Thus the paths are not bit-identical, but meet
the stated tolerance on this adapter.

This supports trying DrawBitmap for cached shadow bitmaps on this path. The
experiment does not establish the same speedup on other GPUs or in-game FPS,
and was initially performed before changing the production renderer.

## Cache enabled versus disabled: Intel Iris Xe, 2026-09-21

300 measured frames after 60 warmup frames, five rotated rounds, hardware D3D11,
RelWithDebInfo. Values are medians of the five per-round medians, in ms.
Raw output: [results/iris-xe-cache-toggle.csv](results/iris-xe-cache-toggle.csv).

| Six panels | CPU | GPU elapsed | Through GPU completion |
| --- | ---: | ---: | ---: |
| No shadow | 0.203 | 0.083 | 0.413 |
| Cache enabled, axis clip | 0.372 | 0.141 | 0.676 |
| Cache disabled, axis clip | 0.917 | 0.298 | 1.404 |
| Forced cache miss, axis clip | 2.088 | 0.415 | 2.818 |

The disabled case has the same static geometry and shadow settings as the enabled
case. Enabled: 9,000 hits, zero misses. Disabled: zero hits and misses, since it
bypasses cache lookup entirely. Both use six axis clips per frame and zero
geometric layers. All 75 scenario/round rows passed cache-count checks and the
shadow pixel sanity check passed for every scenario.

| Panels | Cache enabled CPU | Cache disabled CPU |
| --- | ---: | ---: |
| 1 | 0.189 | 0.279 |
| 6 | 0.372 | 0.917 |
| 24 | 0.849 | 2.744 |

For six panels, the disabled CPU median was about 2.46 times the enabled median;
wall time through GPU completion was about 2.08 times. CPU round medians ranged
from 0.308 to 0.442 ms with caching and 0.776 to 1.236 ms without caching.

Forced misses are more expensive than bypassing the cache here. On a miss, KKE
renders the effect into a new bitmap, flushes, stores it, and later draws that
bitmap. The bypass path draws the effect output without that explicit cache
population step. Thus the old forced-miss result should not be interpreted as
the cost of disabling caching.

Absolute GPU elapsed values changed from the earlier run; power state and other
load were not controlled. Compare enabled/disabled within this run, not against
the earlier table. This does not directly predict in-game FPS.

## Earlier hit-versus-miss measurement: Intel Iris Xe, 2026-09-21

300 measured frames after 60 warmup frames, four rotated rounds, hardware D3D11,
RelWithDebInfo. Values below are medians of the four per-round medians, in ms.
Raw output: [results/iris-xe.csv](results/iris-xe.csv).

| Six panels | CPU | GPU elapsed | Through GPU completion |
| --- | ---: | ---: | ---: |
| No shadow | 0.191 | 0.120 | 0.444 |
| Cached shadow, axis clip | 0.374 | 0.232 | 0.754 |
| Cached shadow, no clip | 0.341 | 0.233 | 0.733 |
| Forced cache miss | 2.044 | 0.849 | 3.084 |

For six panels, both cached scenarios had 7,200 hits and zero misses over all
measured frames. The forced-miss scenario had zero hits and 7,200 misses. All
scenarios had zero geometric layers; the clipped scenarios used six axis clips
per frame.

Comparing corresponding rounds, cached shadows with clips added 0.110..0.253 ms
CPU time (median delta 0.169 ms) and 0.222..0.318 ms through GPU completion
(median delta 0.286 ms). Removing clips did not remove the shadow cost. The
difference between clipped/unclipped CPU medians was -0.019..+0.072 ms per
round and should not be treated as a precise clip cost under uncontrolled load.

| Panels | No shadow CPU | Cached CPU | Forced miss CPU |
| --- | ---: | ---: | ---: |
| 1 | 0.150 | 0.157 | 0.514 |
| 6 | 0.191 | 0.374 | 2.044 |
| 24 | 0.378 | 0.944 | 7.736 |

One-panel CPU measurements were noisy (the no-shadow round medians ranged from
0.123 to 0.303 ms); no reliable small CPU delta is established at that size.

The cache avoids rerunning shadow generation on hits: the current implementation
returns a stored bitmap and composites it with DrawImage. Cache lookup, command
recording/playback, bitmap sampling, and blending still occur. The observed
six-panel CPU difference is mostly in endDraw rather than command recording.
These measurements do not identify undocumented D2D internal buffer use.

This confirms a measurable cost even with all shadow lookups hitting, but does
not establish that it explains a particular in-game FPS drop. Determining that
requires the game's actual GPU/load, HUD dimensions, cache hit rate, and CPU/GPU
frame timings.
