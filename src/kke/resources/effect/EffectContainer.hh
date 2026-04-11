#pragma once

#include <d2d1.h>
#include <d2d1_1.h>

#include <memory>
#include <vector>

#include "Effect.hh"
#include "kke/effect/Effect.hh"

namespace kke {
class EffectContainer {
	ID2D1DeviceContext* deviceContext;
	std::vector<std::shared_ptr<Effect>> effectContainer;

public:
	EffectContainer(ID2D1DeviceContext* context);

	template <typename T>
	std::shared_ptr<T> acquireOrCreateEffect() {
		std::shared_ptr<T> instance = findEffect<T>();
		if (instance) {
			return instance;
		}
		return createEffect<T>();
	}

	template <typename T>
	std::shared_ptr<T> createEffect() {
		std::shared_ptr<T> instance = std::make_shared<T>();
		effectContainer.push_back(instance);
		return instance;
	}

	template <typename T>
	std::shared_ptr<T> findEffect() {
		for (auto& effect : effectContainer) {
			if (auto casted = std::dynamic_pointer_cast<T>(effect)) {
				return casted;
			}
		}
		return nullptr;
	}
};
};	// namespace kke
