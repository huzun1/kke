#include "D2dEngineContext.hh"

using namespace kke;

void D2dEngineContext::update(D2D1_SIZE_F viewportSize) {
	if (!d2dContext || !resourceProviders) {
		return;
	}

	this->viewportSize = viewportSize;
	resourceProviders->update(*d2dContext, viewportSize);
}

void D2dEngineContext::setD2dContext(D2dContext const& context) {
	d2dContext.emplace(context);
}

D2dContext* D2dEngineContext::getD2dContext() const {
	return d2dContext ? &*d2dContext : nullptr;
}

void D2dEngineContext::releaseD2dContext() {
	d2dContext.reset();
}

void D2dEngineContext::setResourceProviders(std::unique_ptr<D2dResourceProviders> resourceProviders
) {
	this->resourceProviders = std::move(resourceProviders);
}

D2dResourceProviders* D2dEngineContext::getResourceProviders() const {
	return resourceProviders.get();
}

D2D1_SIZE_F D2dEngineContext::getViewportSize() const {
	return viewportSize;
}
