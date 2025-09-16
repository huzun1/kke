#include <d2d1.h>

#include <cstdint>
#include <optional>
#include <oreik/ResourceAllocator.hpp>

oreik::ResourceAllocator::ResourceAllocator(ID2D1DeviceContext* context)
	: deviceContext(context), brushStorage() {
}

ID2D1Brush* oreik::ResourceAllocator::aquireOrCreate(oreik::Brush const& brush) {
	uint64_t key = brush.hash();
	std::optional<ID2D1Brush*> cachedBrush = brushStorage.get(key);
	if (cachedBrush) {
		return cachedBrush.value();
	}

	ID2D1Brush* brushInstance = brush.create(deviceContext);
	brushStorage.put(brush.hash(), brushInstance);
	return brushInstance;
}
