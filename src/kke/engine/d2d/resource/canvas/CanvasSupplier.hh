#pragma once

#include <memory>

#include "D2dCanvas.hh"
#include "kke/engine/d2d/D2dContext.hh"
#include "kke/engine/d2d/resource/PoolCache.hh"

namespace kke {
class CanvasSupplier {
	PoolCache<std::shared_ptr<D2dCanvas>> commandListCache;

public:
	std::shared_ptr<D2dCanvas> getOrCreateCanvas(const D2dContext& context);

	void releaseCanvas(std::shared_ptr<D2dCanvas> canvas);

private:
    std::shared_ptr<D2dCanvas> getCanvasFromCache(const D2dContext& context);

	std::shared_ptr<D2dCanvas> createCanvas(const D2dContext& context);
};
};	// namespace kke
