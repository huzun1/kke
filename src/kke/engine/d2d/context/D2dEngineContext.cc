#include "D2dEngineContext.hh"

using namespace kke;

void D2dEngineContext::setD2dContext(std::unique_ptr<D2dContext> context) {
    d2dContext = std::move(context);
}

D2dContext* D2dEngineContext::getD2dContext() const {
    return d2dContext.get();
}

void D2dEngineContext::releaseD2dContext() {
    d2dContext.reset();
}

void D2dEngineContext::setResourceProviders(std::unique_ptr<D2dResourceProviders> resourceProviders) {
    this->resourceProviders = std::move(resourceProviders);
}

D2dResourceProviders* D2dEngineContext::getResourceProviders() const {
    return resourceProviders.get();
}
