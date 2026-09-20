#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include <d3d11.h>

#include "kke/engine/d2d/context/D2dContext.hh"

namespace kke::benchmark {
class ShadowBenchmarkDevice {
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
	Microsoft::WRL::ComPtr<ID2D1Factory1> factory;
	Microsoft::WRL::ComPtr<ID2D1Device> d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> context;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> target;
	Microsoft::WRL::ComPtr<ID3D11Query> disjoint;
	Microsoft::WRL::ComPtr<ID3D11Query> startTimestamp;
	Microsoft::WRL::ComPtr<ID3D11Query> endTimestamp;

  public:
	bool initialize();

	D2dContext drawingContext() const;

	ID2D1Bitmap1* bitmap() const;

	void beginTiming();

	void endTiming();

	std::optional<double> resolveMilliseconds();

	bool hasShadowPixels(bool expected);

	std::optional<std::vector<uint8_t>> readPixels();

  private:
	bool waitForQuery(ID3D11Query* query, void* result, UINT size);
};
} // namespace kke::benchmark
