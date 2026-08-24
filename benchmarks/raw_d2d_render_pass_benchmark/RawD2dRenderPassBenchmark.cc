#include "RawD2dRenderPassBenchmark.hh"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>

#include <d2d1effects.h>

using Microsoft::WRL::ComPtr;
using namespace raw_d2d_benchmark;

RawD2dRenderPassBenchmark::RawD2dRenderPassBenchmark(size_t measuredFrameCount)
	: measuredFrameCount(measuredFrameCount), instance(GetModuleHandleW(nullptr)) {
}

RawD2dRenderPassBenchmark::~RawD2dRenderPassBenchmark() {
	if (window != nullptr) {
		DestroyWindow(window);
	}
	if (windowClass != 0) {
		UnregisterClassW(L"KkeRawD2dRenderPassBenchmark", instance);
	}
}

int RawD2dRenderPassBenchmark::run() {
	initialize();
	std::cout << "Raw Direct2D render-pass benchmark (no KKE)\n"
			  << "viewport=" << viewportWidth << 'x' << viewportHeight
			  << " warmup=" << warmupFrameCount << " measured=" << measuredFrameCount << "\n\n"
			  << std::left << std::setw(38) << "scenario" << std::right << std::setw(13) << "fps"
			  << std::setw(16) << "complete avg" << std::setw(14) << "p50" << std::setw(14) << "p95"
			  << std::setw(14) << "submit avg" << std::setw(14) << "enddraw avg" << std::setw(14)
			  << "present avg" << std::setw(14) << "gpu-wait avg" << '\n';

	for (Scenario scenario : {
			 Scenario::Direct,
			 Scenario::CommandList,
			 Scenario::CommandListOneSnapshot,
			 Scenario::CommandListOneCapture,
			 Scenario::CommandListOneCaptureRecreateBase,
			 Scenario::CommandListOneCaptureReuseBase,
			 Scenario::FullBaseSnapshotReuse,
			 Scenario::CroppedFullSnapshotReuse,
			 Scenario::CroppedQuarterSnapshotReuse,
			 Scenario::BackdropReplayCommandList,
			 Scenario::BackdropFlattenSnapshot,
			 Scenario::CommandListTwoCaptures,
			 Scenario::OffscreenBitmapOneCapture,
			 Scenario::ClipControl,
			 Scenario::AxisAlignedClip,
			 Scenario::RectangleLayerInfiniteBounds,
			 Scenario::RoundedLayerInfiniteBounds,
			 Scenario::AxisClipRoundedLayerInfiniteBounds,
			 Scenario::NestedLayersInfiniteBounds,
			 Scenario::RoundedLayerTightBounds,
			 Scenario::RoundedLayerTightBoundsReusableLayer,
		 }) {
		runScenario(scenario);
	}
	return 0;
}

void RawD2dRenderPassBenchmark::initialize() {
	createWindow();
	createDevices();
	createSwapChain();
	createD2dResources();
	createSceneResources();
}

void RawD2dRenderPassBenchmark::createWindow() {
	WNDCLASSEXW windowClassDescription{
		.cbSize = sizeof(WNDCLASSEXW),
		.lpfnWndProc = windowProcedure,
		.hInstance = instance,
		.lpszClassName = L"KkeRawD2dRenderPassBenchmark",
	};
	windowClass = RegisterClassExW(&windowClassDescription);
	if (windowClass == 0) {
		throw std::runtime_error("RegisterClassExW failed");
	}

	RECT windowRect{0, 0, static_cast<LONG>(viewportWidth), static_cast<LONG>(viewportHeight)};
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	window = CreateWindowExW(
		0,
		windowClassDescription.lpszClassName,
		L"Raw Direct2D Render Pass Benchmark",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		instance,
		nullptr
	);
	if (window == nullptr) {
		throw std::runtime_error("CreateWindowExW failed");
	}
	ShowWindow(window, SW_SHOWNA);
}

void RawD2dRenderPassBenchmark::createDevices() {
	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	D3D_FEATURE_LEVEL featureLevel;
	requireSuccess(
		D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			deviceFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&d3dDevice,
			&featureLevel,
			&d3dContext
		),
		"D3D11CreateDevice"
	);

	D3D11_QUERY_DESC queryDescription{.Query = D3D11_QUERY_EVENT};
	requireSuccess(d3dDevice->CreateQuery(&queryDescription, &completionQuery), "CreateQuery");

	D2D1_FACTORY_OPTIONS factoryOptions{};
	requireSuccess(
		D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory1),
			&factoryOptions,
			reinterpret_cast<void**>(d2dFactory.GetAddressOf())
		),
		"D2D1CreateFactory"
	);

	ComPtr<IDXGIDevice> dxgiDevice;
	requireSuccess(d3dDevice.As(&dxgiDevice), "Query IDXGIDevice");
	requireSuccess(d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice), "Create D2D device");
	requireSuccess(
		d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dContext),
		"Create D2D context"
	);
	requireSuccess(
		d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &snapshotContext),
		"Create snapshot D2D context"
	);

	requireSuccess(
		DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(writeFactory.GetAddressOf())
		),
		"DWriteCreateFactory"
	);
}

void RawD2dRenderPassBenchmark::createSwapChain() {
	ComPtr<IDXGIDevice> dxgiDevice;
	ComPtr<IDXGIAdapter> adapter;
	ComPtr<IDXGIFactory2> factory;
	requireSuccess(d3dDevice.As(&dxgiDevice), "Query IDXGIDevice for swap chain");
	requireSuccess(dxgiDevice->GetAdapter(&adapter), "GetAdapter");
	requireSuccess(adapter->GetParent(IID_PPV_ARGS(&factory)), "Get DXGI factory");

	DXGI_SWAP_CHAIN_DESC1 description{
		.Width = viewportWidth,
		.Height = viewportHeight,
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
		.Stereo = FALSE,
		.SampleDesc = {.Count = 1, .Quality = 0},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 2,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_IGNORE,
	};
	requireSuccess(
		factory->CreateSwapChainForHwnd(
			d3dDevice.Get(),
			window,
			&description,
			nullptr,
			nullptr,
			&swapChain
		),
		"CreateSwapChainForHwnd"
	);
	factory->MakeWindowAssociation(window, DXGI_MWA_NO_ALT_ENTER);
}

void RawD2dRenderPassBenchmark::createD2dResources() {
	ComPtr<IDXGISurface> surface;
	requireSuccess(swapChain->GetBuffer(0, IID_PPV_ARGS(&surface)), "Get swap-chain surface");
	D2D1_BITMAP_PROPERTIES1 targetProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)
	);
	requireSuccess(
		d2dContext->CreateBitmapFromDxgiSurface(surface.Get(), &targetProperties, &targetBitmap),
		"Create target bitmap"
	);

	D2D1_BITMAP_PROPERTIES1 offscreenProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	for (ComPtr<ID2D1Bitmap1>& bitmap : snapshotBitmaps) {
		requireSuccess(
			snapshotContext->CreateBitmap(
				{viewportWidth, viewportHeight},
				nullptr,
				0,
				&offscreenProperties,
				&bitmap
			),
			"Create snapshot bitmap"
		);
	}
	requireSuccess(
		d2dContext->CreateBitmap(
			{viewportWidth, viewportHeight},
			nullptr,
			0,
			&offscreenProperties,
			&offscreenBitmap
		),
		"Create offscreen bitmap"
	);
	requireSuccess(
		snapshotContext->CreateBitmap(
			{viewportWidth, viewportHeight},
			nullptr,
			0,
			&offscreenProperties,
			&externalBaseBitmap
		),
		"Create external base bitmap"
	);
	requireSuccess(
		snapshotContext->CreateBitmap(
			{viewportWidth, viewportHeight},
			nullptr,
			0,
			&offscreenProperties,
			&flattenedBaseBitmap
		),
		"Create flattened base bitmap"
	);
	snapshotContext->BeginDraw();
	snapshotContext->SetTarget(externalBaseBitmap.Get());
	snapshotContext->Clear(D2D1::ColorF(0x0B1118));
	requireSuccess(snapshotContext->EndDraw(), "Initialize external base bitmap");

	D2D1_SIZE_U croppedPixelSize{
		capturePixelBounds.right - capturePixelBounds.left,
		capturePixelBounds.bottom - capturePixelBounds.top,
	};
	D2D1_BITMAP_PROPERTIES1 croppedBaseProperties =
		D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_NONE, targetBitmap->GetPixelFormat());
	requireSuccess(
		d2dContext->CreateBitmap(
			croppedPixelSize,
			nullptr,
			0,
			&croppedBaseProperties,
			&croppedBaseBitmap
		),
		"Create cropped base bitmap"
	);
	requireSuccess(
		snapshotContext->CreateBitmap(
			croppedPixelSize,
			nullptr,
			0,
			&offscreenProperties,
			&croppedSnapshotBitmap
		),
		"Create cropped snapshot bitmap"
	);
	D2D1_SIZE_U downscaledPixelSize{
		static_cast<UINT32>(croppedPixelSize.width * captureRasterScale),
		static_cast<UINT32>(croppedPixelSize.height * captureRasterScale),
	};
	requireSuccess(
		snapshotContext->CreateBitmap(
			downscaledPixelSize,
			nullptr,
			0,
			&offscreenProperties,
			&downscaledSnapshotBitmap
		),
		"Create downscaled snapshot bitmap"
	);
}

void RawD2dRenderPassBenchmark::createSceneResources() {
	requireSuccess(
		d2dContext->CreateSolidColorBrush(D2D1::ColorF(0x16202A, 0.88f), &panelBrush),
		"Create panel brush"
	);
	requireSuccess(
		d2dContext->CreateSolidColorBrush(D2D1::ColorF(0x56C7F2, 1.0f), &accentBrush),
		"Create accent brush"
	);
	requireSuccess(
		d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &textBrush),
		"Create text brush"
	);
	requireSuccess(
		writeFactory->CreateTextFormat(
			L"Segoe UI",
			nullptr,
			DWRITE_FONT_WEIGHT_SEMI_BOLD,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			15.0f,
			L"en-US",
			&textFormat
		),
		"Create text format"
	);

	for (size_t index = 0; index < textLayouts.size(); ++index) {
		std::wstring label = L"Module " + std::to_wstring(index + 1);
		requireSuccess(
			writeFactory->CreateTextLayout(
				label.c_str(),
				static_cast<UINT32>(label.size()),
				textFormat.Get(),
				210.0f,
				24.0f,
				&textLayouts[index]
			),
			"Create text layout"
		);
	}

	for (ComPtr<ID2D1Effect>& effect : blurEffects) {
		requireSuccess(
			d2dContext->CreateEffect(CLSID_D2D1GaussianBlur, &effect),
			"Create blur effect"
		);
		requireSuccess(
			effect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, 12.0f),
			"Configure blur effect"
		);
		requireSuccess(
			effect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD),
			"Configure blur border mode"
		);
	}

	for (size_t index = 0; index < clipCount; ++index) {
		D2D1_RECT_F bounds = clipBounds(index);
		requireSuccess(
			d2dFactory->CreateRectangleGeometry(bounds, &clipRectangleGeometries[index]),
			"Create rectangle clip geometry"
		);
		requireSuccess(
			d2dFactory->CreateRoundedRectangleGeometry(
				D2D1::RoundedRect(bounds, 10.0f, 10.0f),
				&clipRoundedGeometries[index]
			),
			"Create rounded clip geometry"
		);
	}
	requireSuccess(d2dContext->CreateLayer(nullptr, &reusableLayer), "Create reusable layer");
}

void RawD2dRenderPassBenchmark::runScenario(Scenario scenario) {
	for (size_t frame = 0; frame < warmupFrameCount; ++frame) {
		renderFrame(scenario);
	}

	std::vector<FrameTiming> samples;
	samples.reserve(measuredFrameCount);
	for (size_t frame = 0; frame < measuredFrameCount; ++frame) {
		samples.push_back(renderFrame(scenario));
	}
	printResult(scenarioName(scenario), samples);
}

RawD2dRenderPassBenchmark::FrameTiming RawD2dRenderPassBenchmark::renderFrame(Scenario scenario) {
	pumpWindowMessages();
	auto frameBegin = Clock::now();
	switch (scenario) {
	case Scenario::Direct:
		renderDirect();
		break;
	case Scenario::CommandList:
		renderCommandList(0, false);
		break;
	case Scenario::CommandListOneSnapshot:
		renderCommandList(1, false);
		break;
	case Scenario::CommandListOneCapture:
		renderCommandList(1, true);
		break;
	case Scenario::CommandListOneCaptureRecreateBase:
		renderCommandList(1, true, BasePreservation::Recreate);
		break;
	case Scenario::CommandListOneCaptureReuseBase:
		renderCommandList(1, true, BasePreservation::Reuse);
		break;
	case Scenario::FullBaseSnapshotReuse:
		renderCommandList(1, false, BasePreservation::Reuse);
		break;
	case Scenario::CroppedFullSnapshotReuse:
		renderCroppedSnapshot(false);
		break;
	case Scenario::CroppedQuarterSnapshotReuse:
		renderCroppedSnapshot(true);
		break;
	case Scenario::BackdropReplayCommandList:
		renderBackdropBoundary(false);
		break;
	case Scenario::BackdropFlattenSnapshot:
		renderBackdropBoundary(true);
		break;
	case Scenario::CommandListTwoCaptures:
		renderCommandList(2, true);
		break;
	case Scenario::OffscreenBitmapOneCapture:
		renderOffscreenBitmap();
		break;
	case Scenario::ClipControl:
	case Scenario::AxisAlignedClip:
	case Scenario::RectangleLayerInfiniteBounds:
	case Scenario::RoundedLayerInfiniteBounds:
	case Scenario::AxisClipRoundedLayerInfiniteBounds:
	case Scenario::NestedLayersInfiniteBounds:
	case Scenario::RoundedLayerTightBounds:
	case Scenario::RoundedLayerTightBoundsReusableLayer:
		renderClipScenario(scenario);
		break;
	}
	auto submitEnd = Clock::now();
	HRESULT endDrawResult = d2dContext->EndDraw();
	auto endDrawEnd = Clock::now();
	requireSuccess(endDrawResult, "EndDraw");
	requireSuccess(swapChain->Present(0, 0), "Present");
	auto presentEnd = Clock::now();
	waitForGpu();
	auto completeEnd = Clock::now();
	return {
		.submitMilliseconds = elapsedMilliseconds(frameBegin, submitEnd),
		.endDrawMilliseconds = elapsedMilliseconds(submitEnd, endDrawEnd),
		.presentMilliseconds = elapsedMilliseconds(endDrawEnd, presentEnd),
		.gpuWaitMilliseconds = elapsedMilliseconds(presentEnd, completeEnd),
		.completeMilliseconds = elapsedMilliseconds(frameBegin, completeEnd),
	};
}

void RawD2dRenderPassBenchmark::renderDirect() {
	d2dContext->BeginDraw();
	d2dContext->SetTarget(targetBitmap.Get());
	d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dContext->Clear(D2D1::ColorF(0x0B1118));
	drawSceneRange(0, textCount);
}

void RawD2dRenderPassBenchmark::renderCommandList(
	size_t captureCount, bool drawBackdropEffect, BasePreservation basePreservation
) {
	d2dContext->BeginDraw();
	ComPtr<ID2D1CommandList> commandList = createCommandList();
	d2dContext->SetTarget(commandList.Get());
	d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	preparePreservedBaseBitmap(basePreservation);

	size_t segmentSize = textCount / (captureCount + 1);
	size_t segmentBegin = 0;
	for (size_t captureIndex = 0; captureIndex < captureCount; ++captureIndex) {
		size_t segmentEnd = segmentBegin + segmentSize;
		drawSceneRange(segmentBegin, segmentEnd);
		requireSuccess(commandList->Close(), "Close command list for snapshot");
		snapshotCommandList(commandList.Get(), captureIndex, preservedBaseBitmap.Get());

		ComPtr<ID2D1CommandList> nextCommandList = createCommandList();
		d2dContext->SetTarget(nextCommandList.Get());
		d2dContext->DrawImage(commandList.Get());
		if (drawBackdropEffect) {
			drawBlurredSnapshot(snapshotBitmaps[captureIndex].Get(), captureIndex);
		}
		commandList = nextCommandList;
		segmentBegin = segmentEnd;
	}
	drawSceneRange(segmentBegin, textCount);
	requireSuccess(commandList->Close(), "Close final command list");

	d2dContext->SetTarget(targetBitmap.Get());
	d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dContext->Clear(D2D1::ColorF(0x0B1118));
	d2dContext->DrawImage(commandList.Get());
}

void RawD2dRenderPassBenchmark::renderOffscreenBitmap() {
	d2dContext->BeginDraw();
	d2dContext->SetTarget(offscreenBitmap.Get());
	d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dContext->Clear(D2D1::ColorF(0x0B1118));
	drawSceneRange(0, textCount / 2);

	d2dContext->SetTarget(targetBitmap.Get());
	d2dContext->Clear(D2D1::ColorF(0x0B1118));
	d2dContext->DrawImage(offscreenBitmap.Get());
	drawBlurredSnapshot(offscreenBitmap.Get(), 0);
	drawSceneRange(textCount / 2, textCount);
}

void RawD2dRenderPassBenchmark::renderCroppedSnapshot(bool downscale) {
	d2dContext->BeginDraw();
	ComPtr<ID2D1CommandList> commandList = createCommandList();
	d2dContext->SetTarget(commandList.Get());
	d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	drawSceneRange(0, textCount / 2);
	requireSuccess(commandList->Close(), "Close command list for cropped snapshot");

	D2D1_POINT_2U destinationPoint{0, 0};
	requireSuccess(
		croppedBaseBitmap
			->CopyFromBitmap(&destinationPoint, targetBitmap.Get(), &capturePixelBounds),
		"Copy cropped base bitmap"
	);

	ID2D1Bitmap1* snapshotBitmap =
		downscale ? downscaledSnapshotBitmap.Get() : croppedSnapshotBitmap.Get();
	float rasterScale = downscale ? captureRasterScale : 1.0f;
	D2D1_SIZE_F snapshotSize = snapshotBitmap->GetSize();
	D2D1_SIZE_U croppedPixelSize = croppedBaseBitmap->GetPixelSize();
	snapshotContext->BeginDraw();
	snapshotContext->SetTarget(snapshotBitmap);
	snapshotContext->SetTransform(D2D1::Matrix3x2F::Identity());
	snapshotContext->Clear(D2D1::ColorF(0x0B1118));
	if (downscale) {
		snapshotContext->DrawBitmap(
			croppedBaseBitmap.Get(),
			{0.0f, 0.0f, snapshotSize.width, snapshotSize.height},
			1.0f,
			D2D1_INTERPOLATION_MODE_LINEAR,
			{0.0f,
			 0.0f,
			 static_cast<float>(croppedPixelSize.width),
			 static_cast<float>(croppedPixelSize.height)}
		);
	} else {
		snapshotContext->DrawImage(croppedBaseBitmap.Get());
	}
	snapshotContext->SetTransform(D2D1::Matrix3x2F(
		rasterScale,
		0.0f,
		0.0f,
		rasterScale,
		-static_cast<float>(capturePixelBounds.left) * rasterScale,
		-static_cast<float>(capturePixelBounds.top) * rasterScale
	));
	snapshotContext->DrawImage(commandList.Get());
	requireSuccess(snapshotContext->EndDraw(), "Cropped snapshot EndDraw");

	ComPtr<ID2D1CommandList> nextCommandList = createCommandList();
	d2dContext->SetTarget(nextCommandList.Get());
	d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dContext->DrawImage(commandList.Get());
	drawSceneRange(textCount / 2, textCount);
	requireSuccess(nextCommandList->Close(), "Close final cropped command list");

	d2dContext->SetTarget(targetBitmap.Get());
	d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dContext->Clear(D2D1::ColorF(0x0B1118));
	d2dContext->DrawImage(nextCommandList.Get());
}

void RawD2dRenderPassBenchmark::renderBackdropBoundary(bool flattenSnapshot) {
	d2dContext->BeginDraw();
	ComPtr<ID2D1CommandList> commandList = createCommandList();
	d2dContext->SetTarget(commandList.Get());
	d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	drawSceneRange(0, textCount / 2);
	requireSuccess(commandList->Close(), "Close backdrop input command list");

	D2D1_SIZE_U pixelSize = externalBaseBitmap->GetPixelSize();
	D2D1_POINT_2U destinationPoint{0, 0};
	D2D1_RECT_U sourceRect{0, 0, pixelSize.width, pixelSize.height};
	requireSuccess(
		flattenedBaseBitmap
			->CopyFromBitmap(&destinationPoint, externalBaseBitmap.Get(), &sourceRect),
		"Copy backdrop base bitmap"
	);

	snapshotContext->BeginDraw();
	snapshotContext->SetTarget(snapshotBitmaps[0].Get());
	snapshotContext->SetTransform(D2D1::Matrix3x2F::Identity());
	snapshotContext->Clear();
	snapshotContext->DrawImage(flattenedBaseBitmap.Get());
	snapshotContext->DrawImage(commandList.Get());
	requireSuccess(snapshotContext->EndDraw(), "Backdrop snapshot EndDraw");

	ComPtr<ID2D1CommandList> nextCommandList = createCommandList();
	d2dContext->SetTarget(nextCommandList.Get());
	d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	if (flattenSnapshot) {
		d2dContext->DrawImage(snapshotBitmaps[0].Get());
	} else {
		d2dContext->DrawImage(commandList.Get());
	}
	drawBlurredSnapshot(snapshotBitmaps[0].Get(), 0);
	drawSceneRange(textCount / 2, textCount);
	requireSuccess(nextCommandList->Close(), "Close backdrop output command list");

	d2dContext->SetTarget(targetBitmap.Get());
	d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dContext->Clear();
	if (!flattenSnapshot) {
		d2dContext->DrawImage(externalBaseBitmap.Get());
	}
	d2dContext->DrawImage(nextCommandList.Get());
}

void RawD2dRenderPassBenchmark::renderClipScenario(Scenario scenario) {
	d2dContext->BeginDraw();
	d2dContext->SetTarget(targetBitmap.Get());
	d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dContext->Clear(D2D1::ColorF(0x0B1118));

	for (size_t index = 0; index < clipCount; ++index) {
		D2D1_RECT_F bounds = clipBounds(index);
		D2D1_RECT_F fillBounds{
			bounds.left - 12.0f,
			bounds.top - 12.0f,
			bounds.right + 12.0f,
			bounds.bottom + 12.0f,
		};
		auto infiniteRectangleLayer =
			D2D1::LayerParameters1(D2D1::InfiniteRect(), clipRectangleGeometries[index].Get());
		auto infiniteRoundedLayer =
			D2D1::LayerParameters1(D2D1::InfiniteRect(), clipRoundedGeometries[index].Get());
		auto tightRoundedLayer = D2D1::LayerParameters1(bounds, clipRoundedGeometries[index].Get());

		switch (scenario) {
		case Scenario::ClipControl:
			break;
		case Scenario::AxisAlignedClip:
			d2dContext->PushAxisAlignedClip(bounds, D2D1_ANTIALIAS_MODE_ALIASED);
			break;
		case Scenario::RectangleLayerInfiniteBounds:
			d2dContext->PushLayer(infiniteRectangleLayer, nullptr);
			break;
		case Scenario::RoundedLayerInfiniteBounds:
			d2dContext->PushLayer(infiniteRoundedLayer, nullptr);
			break;
		case Scenario::AxisClipRoundedLayerInfiniteBounds:
			d2dContext->PushAxisAlignedClip(bounds, D2D1_ANTIALIAS_MODE_ALIASED);
			d2dContext->PushLayer(infiniteRoundedLayer, nullptr);
			break;
		case Scenario::NestedLayersInfiniteBounds:
			d2dContext->PushLayer(infiniteRectangleLayer, nullptr);
			d2dContext->PushLayer(infiniteRoundedLayer, nullptr);
			break;
		case Scenario::RoundedLayerTightBounds:
			d2dContext->PushLayer(tightRoundedLayer, nullptr);
			break;
		case Scenario::RoundedLayerTightBoundsReusableLayer:
			d2dContext->PushLayer(tightRoundedLayer, reusableLayer.Get());
			break;
		default:
			break;
		}

		d2dContext->FillRectangle(fillBounds, accentBrush.Get());

		switch (scenario) {
		case Scenario::AxisAlignedClip:
			d2dContext->PopAxisAlignedClip();
			break;
		case Scenario::RectangleLayerInfiniteBounds:
		case Scenario::RoundedLayerInfiniteBounds:
		case Scenario::RoundedLayerTightBounds:
		case Scenario::RoundedLayerTightBoundsReusableLayer:
			d2dContext->PopLayer();
			break;
		case Scenario::AxisClipRoundedLayerInfiniteBounds:
			d2dContext->PopLayer();
			d2dContext->PopAxisAlignedClip();
			break;
		case Scenario::NestedLayersInfiniteBounds:
			d2dContext->PopLayer();
			d2dContext->PopLayer();
			break;
		default:
			break;
		}
	}
}

D2D1_RECT_F RawD2dRenderPassBenchmark::clipBounds(size_t index) const {
	float column = static_cast<float>(index % 4);
	float row = static_cast<float>(index / 4);
	float left = 90.0f + column * 440.0f;
	float top = 92.0f + row * 176.0f;
	return {left, top, left + 320.0f, top + 96.0f};
}

void RawD2dRenderPassBenchmark::drawSceneRange(size_t beginIndex, size_t endIndex) {
	for (size_t index = beginIndex; index < endIndex; ++index) {
		float column = static_cast<float>(index % 5);
		float row = static_cast<float>(index / 5);
		float left = 72.0f + column * 360.0f;
		float top = 84.0f + row * 132.0f;
		D2D1_ROUNDED_RECT panel{
			.rect = {left, top, left + 276.0f, top + 86.0f},
			.radiusX = 12.0f,
			.radiusY = 12.0f,
		};
		d2dContext->FillRoundedRectangle(panel, panelBrush.Get());
		d2dContext->DrawRoundedRectangle(panel, accentBrush.Get(), 1.5f);
		d2dContext->PushAxisAlignedClip(
			{left + 12.0f, top + 10.0f, left + 264.0f, top + 76.0f},
			D2D1_ANTIALIAS_MODE_ALIASED
		);
		d2dContext->DrawTextLayout(
			{left + 18.0f, top + 18.0f},
			textLayouts[index].Get(),
			textBrush.Get(),
			D2D1_DRAW_TEXT_OPTIONS_CLIP
		);
		d2dContext->PopAxisAlignedClip();
	}
}

void RawD2dRenderPassBenchmark::drawBlurredSnapshot(ID2D1Image* snapshot, size_t effectIndex) {
	ComPtr<ID2D1Effect>& effect = blurEffects[effectIndex];
	effect->SetInput(0, snapshot);
	for (size_t panelIndex = 0; panelIndex < 6; ++panelIndex) {
		float left = 118.0f + static_cast<float>(panelIndex) * 288.0f;
		D2D1_RECT_F clip{left, 790.0f, left + 244.0f, 962.0f};
		d2dContext->PushAxisAlignedClip(clip, D2D1_ANTIALIAS_MODE_ALIASED);
		d2dContext->DrawImage(effect.Get());
		d2dContext->PopAxisAlignedClip();
	}
}

ComPtr<ID2D1CommandList> RawD2dRenderPassBenchmark::createCommandList() {
	ComPtr<ID2D1CommandList> commandList;
	requireSuccess(d2dContext->CreateCommandList(&commandList), "CreateCommandList");
	return commandList;
}

void RawD2dRenderPassBenchmark::preparePreservedBaseBitmap(BasePreservation basePreservation) {
	if (basePreservation == BasePreservation::None) {
		preservedBaseBitmap.Reset();
		return;
	}
	if (basePreservation == BasePreservation::Recreate) {
		preservedBaseBitmap.Reset();
	}
	if (!preservedBaseBitmap) {
		float dpiX, dpiY;
		targetBitmap->GetDpi(&dpiX, &dpiY);
		D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_NONE,
			targetBitmap->GetPixelFormat(),
			dpiX,
			dpiY
		);
		requireSuccess(
			d2dContext->CreateBitmap(
				targetBitmap->GetPixelSize(),
				nullptr,
				0,
				&properties,
				&preservedBaseBitmap
			),
			"Create preserved base bitmap"
		);
	}

	D2D1_SIZE_U pixelSize = targetBitmap->GetPixelSize();
	D2D1_POINT_2U destinationPoint{0, 0};
	D2D1_RECT_U sourceRect{0, 0, pixelSize.width, pixelSize.height};
	requireSuccess(
		preservedBaseBitmap->CopyFromBitmap(&destinationPoint, targetBitmap.Get(), &sourceRect),
		"Copy preserved base bitmap"
	);
}

void RawD2dRenderPassBenchmark::snapshotCommandList(
	ID2D1CommandList* commandList, size_t snapshotIndex, ID2D1Image* preservedBase
) {
	snapshotContext->BeginDraw();
	snapshotContext->SetTarget(snapshotBitmaps[snapshotIndex].Get());
	snapshotContext->SetTransform(D2D1::Matrix3x2F::Identity());
	snapshotContext->Clear(D2D1::ColorF(0x0B1118));
	if (preservedBase != nullptr) {
		snapshotContext->DrawImage(preservedBase);
	}
	snapshotContext->DrawImage(commandList);
	requireSuccess(snapshotContext->EndDraw(), "Snapshot EndDraw");
}

void RawD2dRenderPassBenchmark::waitForGpu() {
	d3dContext->End(completionQuery.Get());
	d3dContext->Flush();
	while (d3dContext->GetData(completionQuery.Get(), nullptr, 0, 0) == S_FALSE) {
		std::this_thread::yield();
	}
}

void RawD2dRenderPassBenchmark::pumpWindowMessages() {
	MSG message;
	while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}
}

void RawD2dRenderPassBenchmark::requireSuccess(HRESULT result, std::string_view operation) {
	if (SUCCEEDED(result)) {
		return;
	}
	std::ostringstream message;
	message << operation << " failed with HRESULT 0x" << std::hex
			<< static_cast<unsigned long>(result);
	throw std::runtime_error(message.str());
}

double
RawD2dRenderPassBenchmark::elapsedMilliseconds(Clock::time_point begin, Clock::time_point end) {
	return std::chrono::duration<double, std::milli>(end - begin).count();
}

RawD2dRenderPassBenchmark::Distribution
RawD2dRenderPassBenchmark::distribution(std::span<double const> samples) {
	std::vector<double> sorted(samples.begin(), samples.end());
	std::sort(sorted.begin(), sorted.end());
	double sum = std::accumulate(sorted.begin(), sorted.end(), 0.0);
	size_t p95Index = static_cast<size_t>(std::ceil(sorted.size() * 0.95)) - 1;
	return {
		.average = sum / static_cast<double>(sorted.size()),
		.p50 = sorted[sorted.size() / 2],
		.p95 = sorted[std::min(p95Index, sorted.size() - 1)],
	};
}

void RawD2dRenderPassBenchmark::printResult(
	std::string_view name, std::span<FrameTiming const> samples
) {
	std::vector<double> submit;
	std::vector<double> endDraw;
	std::vector<double> present;
	std::vector<double> gpuWait;
	std::vector<double> complete;
	submit.reserve(samples.size());
	endDraw.reserve(samples.size());
	present.reserve(samples.size());
	gpuWait.reserve(samples.size());
	complete.reserve(samples.size());
	for (FrameTiming const& sample : samples) {
		submit.push_back(sample.submitMilliseconds);
		endDraw.push_back(sample.endDrawMilliseconds);
		present.push_back(sample.presentMilliseconds);
		gpuWait.push_back(sample.gpuWaitMilliseconds);
		complete.push_back(sample.completeMilliseconds);
	}
	Distribution submitTiming = distribution(submit);
	Distribution endDrawTiming = distribution(endDraw);
	Distribution presentTiming = distribution(present);
	Distribution gpuWaitTiming = distribution(gpuWait);
	Distribution completeTiming = distribution(complete);
	std::cout << std::left << std::setw(38) << name << std::right << std::fixed
			  << std::setprecision(2) << std::setw(13) << 1000.0 / completeTiming.average
			  << std::setprecision(3) << std::setw(16) << completeTiming.average << std::setw(14)
			  << completeTiming.p50 << std::setw(14) << completeTiming.p95 << std::setw(14)
			  << submitTiming.average << std::setw(14) << endDrawTiming.average << std::setw(14)
			  << presentTiming.average << std::setw(14) << gpuWaitTiming.average << '\n';
}

std::string_view RawD2dRenderPassBenchmark::scenarioName(Scenario scenario) {
	switch (scenario) {
	case Scenario::Direct:
		return "direct";
	case Scenario::CommandList:
		return "command-list";
	case Scenario::CommandListOneSnapshot:
		return "command-list + 1 snapshot";
	case Scenario::CommandListOneCapture:
		return "command-list + 1 capture";
	case Scenario::CommandListOneCaptureRecreateBase:
		return "1 capture + recreate base bitmap";
	case Scenario::CommandListOneCaptureReuseBase:
		return "1 capture + reuse base bitmap";
	case Scenario::FullBaseSnapshotReuse:
		return "full base snapshot (reuse)";
	case Scenario::CroppedFullSnapshotReuse:
		return "cropped full snapshot (reuse)";
	case Scenario::CroppedQuarterSnapshotReuse:
		return "cropped 1/4 snapshot (reuse)";
	case Scenario::BackdropReplayCommandList:
		return "backdrop: replay command-list";
	case Scenario::BackdropFlattenSnapshot:
		return "backdrop: flatten snapshot";
	case Scenario::CommandListTwoCaptures:
		return "command-list + 2 captures";
	case Scenario::OffscreenBitmapOneCapture:
		return "offscreen-bitmap + 1 effect";
	case Scenario::ClipControl:
		return "clip control (20 fills)";
	case Scenario::AxisAlignedClip:
		return "axis-aligned clip x20";
	case Scenario::RectangleLayerInfiniteBounds:
		return "rectangle layer infinite x20";
	case Scenario::RoundedLayerInfiniteBounds:
		return "rounded layer infinite x20";
	case Scenario::AxisClipRoundedLayerInfiniteBounds:
		return "axis + rounded layer infinite x20";
	case Scenario::NestedLayersInfiniteBounds:
		return "nested layers infinite x20";
	case Scenario::RoundedLayerTightBounds:
		return "rounded layer tight x20";
	case Scenario::RoundedLayerTightBoundsReusableLayer:
		return "rounded tight reused layer x20";
	}
	return "unknown";
}

LRESULT CALLBACK RawD2dRenderPassBenchmark::windowProcedure(
	HWND window, UINT message, WPARAM wParam, LPARAM lParam
) {
	if (message == WM_CLOSE) {
		return 0;
	}
	return DefWindowProcW(window, message, wParam, lParam);
}
