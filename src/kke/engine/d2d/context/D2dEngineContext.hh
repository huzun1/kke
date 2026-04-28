#pragma once

#include <memory>

#include "kke/engine/d2d/context/D2dResourceProviders.hh"
#include "kke/engine/d2d/context/D2dContext.hh"

namespace kke {
class D2dEngineContext {
	std::unique_ptr<D2dContext> d2dContext;
    std::unique_ptr<D2dResourceProviders> resourceProviders;

public:
    void setD2dContext(std::unique_ptr<D2dContext> context);

    D2dContext* getD2dContext() const;

    void releaseD2dContext();

    void setResourceProviders(std::unique_ptr<D2dResourceProviders> resourceProviders);

    D2dResourceProviders* getResourceProviders() const;
};
};	// namespace kke
