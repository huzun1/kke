#pragma once

#include <array>
#include <chrono>
#include <cstddef>
#include <span>
#include <string_view>
#include <vector>

#include <Windows.h>
#include <d2d1_1.h>
#include <d3d11.h>
#include <dwrite.h>
#include <dxgi1_2.h>
#include <wrl/client.h>

namespace raw_d2d_benchmark {
class RawD2dRenderPassBenchmark {
	using Clock = std::chrono::steady_clock;

	enum class Scenario {
		Direct,
		CommandList,
		CommandListOneSnapshot,
		CommandListOneCapture,
		CommandListOneCaptureRecreateBase,
		CommandListOneCaptureReuseBase,
		FullBaseSnapshotReuse,
		CroppedFullSnapshotReuse,
		CroppedQuarterSnapshotReuse,
		BackdropReplayCommandList,
		BackdropFlattenSnapshot,
		CommandListTwoCaptures,
		OffscreenBitmapOneCapture,
		ClipControl,
		AxisAlignedClip,
		RectangleLayerInfiniteBounds,
		RoundedLayerInfiniteBounds,
		AxisClipRoundedLayerInfiniteBounds,
		NestedLayersInfiniteBounds,
		RoundedLayerTightBounds,
		RoundedLayerTightBoundsReusableLayer,
	};

	enum class BasePreservation {
		None,
		Recreate,
		Reuse,
	};

	struct FrameTiming {
		double submitMilliseconds = 0.0;
		double endDrawMilliseconds = 0.0;
		double presentMilliseconds = 0.0;
		double gpuWaitMilliseconds = 0.0;
		double completeMilliseconds = 0.0;
	};

	struct Distribution {
		double average = 0.0;
		double p50 = 0.0;
		double p95 = 0.0;
	};

	static constexpr UINT viewportWidth = 1920;
	static constexpr UINT viewportHeight = 1009;
	static constexpr size_t textCount = 30;
	static constexpr size_t clipCount = 20;
	static constexpr size_t warmupFrameCount = 120;
	static constexpr D2D1_RECT_U capturePixelBounds{320, 120, 1600, 880};
	static constexpr float captureRasterScale = 0.25f;

	size_t measuredFrameCount;
	HINSTANCE instance = nullptr;
	HWND window = nullptr;
	ATOM windowClass = 0;
	Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
	Microsoft::WRL::ComPtr<ID3D11Query> completionQuery;
	Microsoft::WRL::ComPtr<ID2D1Factory1> d2dFactory;
	Microsoft::WRL::ComPtr<ID2D1Device> d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> snapshotContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> targetBitmap;
	std::array<Microsoft::WRL::ComPtr<ID2D1Bitmap1>, 2> snapshotBitmaps;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> preservedBaseBitmap;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> croppedBaseBitmap;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> croppedSnapshotBitmap;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> downscaledSnapshotBitmap;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> offscreenBitmap;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> externalBaseBitmap;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> flattenedBaseBitmap;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> panelBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> accentBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> textBrush;
	Microsoft::WRL::ComPtr<IDWriteFactory> writeFactory;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;
	std::array<Microsoft::WRL::ComPtr<IDWriteTextLayout>, textCount> textLayouts;
	std::array<Microsoft::WRL::ComPtr<ID2D1Effect>, 2> blurEffects;
	std::array<Microsoft::WRL::ComPtr<ID2D1RectangleGeometry>, clipCount> clipRectangleGeometries;
	std::array<Microsoft::WRL::ComPtr<ID2D1RoundedRectangleGeometry>, clipCount>
		clipRoundedGeometries;
	Microsoft::WRL::ComPtr<ID2D1Layer> reusableLayer;

  public:
	explicit RawD2dRenderPassBenchmark(size_t measuredFrameCount);

	~RawD2dRenderPassBenchmark();

	int run();

  private:
	void initialize();

	void createWindow();

	void createDevices();

	void createSwapChain();

	void createD2dResources();

	void createSceneResources();

	void runScenario(Scenario scenario);

	FrameTiming renderFrame(Scenario scenario);

	void renderDirect();

	void renderCommandList(
		size_t captureCount,
		bool drawBackdropEffect,
		BasePreservation basePreservation = BasePreservation::None
	);

	void renderOffscreenBitmap();

	void renderCroppedSnapshot(bool downscale);

	void renderBackdropBoundary(bool flattenSnapshot);

	void renderClipScenario(Scenario scenario);

	D2D1_RECT_F clipBounds(size_t index) const;

	void drawSceneRange(size_t beginIndex, size_t endIndex);

	void drawBlurredSnapshot(ID2D1Image* snapshot, size_t effectIndex);

	Microsoft::WRL::ComPtr<ID2D1CommandList> createCommandList();

	void preparePreservedBaseBitmap(BasePreservation basePreservation);

	void snapshotCommandList(
		ID2D1CommandList* commandList, size_t snapshotIndex, ID2D1Image* preservedBase
	);

	void waitForGpu();

	void pumpWindowMessages();

	static void requireSuccess(HRESULT result, std::string_view operation);

	static double elapsedMilliseconds(Clock::time_point begin, Clock::time_point end);

	static Distribution distribution(std::span<double const> samples);

	static void printResult(std::string_view name, std::span<FrameTiming const> samples);

	static std::string_view scenarioName(Scenario scenario);

	static LRESULT CALLBACK
	windowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
};
} // namespace raw_d2d_benchmark
