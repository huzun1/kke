#pragma once

#include <memory>
#include <optional>

#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/engine/d2d/context/D2dResourceProviders.hh"

namespace kke {
class D2dEngineContext {
	mutable std::optional<D2dContext> d2dContext;
	std::unique_ptr<D2dResourceProviders> resourceProviders;
	D2D1_SIZE_F viewportSize = {};

  public:
	void update(D2D1_SIZE_F viewportSize);

	void setD2dContext(D2dContext const& context);

	D2dContext* getD2dContext() const;

	void releaseD2dContext();

	void setResourceProviders(std::unique_ptr<D2dResourceProviders> resourceProviders);

	D2dResourceProviders* getResourceProviders() const;

	D2D1_SIZE_F getViewportSize() const;
};
}; // namespace kke
