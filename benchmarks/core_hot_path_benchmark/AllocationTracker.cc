#include "AllocationTracker.hh"

#include <cstdlib>
#include <malloc.h>
#include <new>

using namespace kke::benchmark;

thread_local bool AllocationTracker::isTracking = false;
thread_local AllocationSample AllocationTracker::currentSample;

void AllocationTracker::begin() {
	currentSample = {};
	isTracking = true;
}

AllocationSample AllocationTracker::finish() {
	isTracking = false;
	return currentSample;
}

void AllocationTracker::record(size_t bytes) {
	if (isTracking) {
		++currentSample.count;
		currentSample.bytes += bytes;
	}
}

void* operator new(size_t size) {
	AllocationTracker::record(size);
	if (void* memory = std::malloc(size)) {
		return memory;
	}
	throw std::bad_alloc();
}

void* operator new[](size_t size) {
	return ::operator new(size);
}

void operator delete(void* memory) noexcept {
	std::free(memory);
}

void operator delete[](void* memory) noexcept {
	std::free(memory);
}

void operator delete(void* memory, size_t) noexcept {
	std::free(memory);
}

void operator delete[](void* memory, size_t) noexcept {
	std::free(memory);
}

void* operator new(size_t size, std::align_val_t alignment) {
	AllocationTracker::record(size);
	if (void* memory = _aligned_malloc(size, static_cast<size_t>(alignment))) {
		return memory;
	}
	throw std::bad_alloc();
}

void* operator new[](size_t size, std::align_val_t alignment) {
	return ::operator new(size, alignment);
}

void operator delete(void* memory, std::align_val_t) noexcept {
	_aligned_free(memory);
}

void operator delete[](void* memory, std::align_val_t) noexcept {
	_aligned_free(memory);
}

void operator delete(void* memory, size_t, std::align_val_t) noexcept {
	_aligned_free(memory);
}

void operator delete[](void* memory, size_t, std::align_val_t) noexcept {
	_aligned_free(memory);
}
