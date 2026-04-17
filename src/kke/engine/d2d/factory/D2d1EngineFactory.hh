#pragma once

#include <memory>

#include "kke/engine/d2d1/D2d1Engine.hh"
#include "kke/engine/d2d1/D2d1Context.hh"
#include "kke/engine/d2d1/d2d1_headers.hh"

namespace kke {
class D2d1EngineFactory {
public:
	static std::shared_ptr<D2d1Engine> create(D2d1Context context);
};
};	// namespace kke
