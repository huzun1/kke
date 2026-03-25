#include <Unknwn.h>
#include <d2d1helper.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "kke/CacheStorage.hh"
#include "kke/common/Color.hh"
#include "kke/common/Point.hh"
#include "kke/common/Scale.hh"
#include "kke/common/geometry/Rect.hh"
#include "kke/common/geometry/Triangle.hh"
#include "kke/transform/Matrix.hh"

namespace {
class FakeUnknown : public IUnknown {
	ULONG refCount_ = 1;

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** object) override {
		if (object == nullptr) {
			return E_POINTER;
		}
		if (iid == IID_IUnknown) {
			*object = static_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}
		*object = nullptr;
		return E_NOINTERFACE;
	}

	ULONG STDMETHODCALLTYPE AddRef() override {
		return ++refCount_;
	}

	ULONG STDMETHODCALLTYPE Release() override {
		const ULONG count = --refCount_;
		if (count == 0) {
			delete this;
		}
		return count;
	}
};

Microsoft::WRL::ComPtr<IUnknown> makeFakeUnknown() {
	Microsoft::WRL::ComPtr<IUnknown> object;
	object.Attach(new FakeUnknown());
	return object;
}

void expect(bool condition, const std::string& message) {
	if (!condition) {
		throw std::runtime_error(message);
	}
}

void expectNear(float lhs, float rhs, const std::string& message, float epsilon = 0.0001f) {
	if (std::fabs(lhs - rhs) > epsilon) {
		throw std::runtime_error(message);
	}
}

void expectMatrixNear(
	const D2D1::Matrix3x2F& lhs,
	const D2D1::Matrix3x2F& rhs,
	const std::string& message) {
	expectNear(lhs._11, rhs._11, message + " (_11)");
	expectNear(lhs._12, rhs._12, message + " (_12)");
	expectNear(lhs._21, rhs._21, message + " (_21)");
	expectNear(lhs._22, rhs._22, message + " (_22)");
	expectNear(lhs._31, rhs._31, message + " (_31)");
	expectNear(lhs._32, rhs._32, message + " (_32)");
}

void testColorDefaults() {
	const kke::Color4f color;
	expectNear(color.r, 0.0f, "default color red should be 0");
	expectNear(color.g, 0.0f, "default color green should be 0");
	expectNear(color.b, 0.0f, "default color blue should be 0");
	expectNear(color.a, 1.0f, "default color alpha should be 1");
}

void testPointOperations() {
	const kke::Point2f point{3.0f, 4.0f};
	const kke::Point2f shifted = point.offset(2.0f);
	const kke::Point2f scaled = point * kke::Point2f{2.0f, -1.0f};

	expectNear(shifted.x, 5.0f, "scalar offset should update x");
	expectNear(shifted.y, 6.0f, "scalar offset should update y");
	expectNear(scaled.x, 6.0f, "component multiply should update x");
	expectNear(scaled.y, -4.0f, "component multiply should update y");
	expect(point.hash() == kke::Point2f{3.0f, 4.0f}.hash(), "point hash should be deterministic");
}

void testScaleValueSemantics() {
	const kke::Scale2f scale{2.0f, 3.0f};
	expectNear(scale.x, 2.0f, "scale x should preserve constructor value");
	expectNear(scale.y, 3.0f, "scale y should preserve constructor value");
}

void testGeometryHashes() {
	const kke::Rect first{10.0f, 20.0f, 110.0f, 120.0f};
	const kke::Rect second{40.0f, 60.0f, 140.0f, 160.0f};
	expect(first.hash(false) == second.hash(false), "position-independent rect hash should match equal sizes");
	expect(first.hash(true) != second.hash(true), "position-dependent rect hash should differ across origins");

	const kke::Triangle triangleA{{10.0f, 10.0f}, {20.0f, 10.0f}, {15.0f, 30.0f}};
	const kke::Triangle triangleB{{100.0f, 100.0f}, {110.0f, 100.0f}, {105.0f, 120.0f}};
	expect(triangleA.hash(false) == triangleB.hash(false), "position-independent triangle hash should match translated triangles");
	expect(triangleA.hash(true) != triangleB.hash(true), "position-dependent triangle hash should differ across origins");
}

void testMatrixBuildAndPop() {
	kke::Matrix matrix;
	matrix.pushTranslate({10.0f, 20.0f});
	matrix.pushScale({0.0f, 0.0f}, {2.0f, 3.0f});

	const D2D1::Matrix3x2F expected =
		D2D1::Matrix3x2F::Identity() *
		D2D1::Matrix3x2F::Translation(10.0f, 20.0f) *
		D2D1::Matrix3x2F::Scale({2.0f, 3.0f}, {0.0f, 0.0f});
	expectMatrixNear(matrix.build(), expected, "matrix build should compose transforms in stack order");

	matrix.pop();
	expectMatrixNear(
		matrix.build(),
		D2D1::Matrix3x2F::Identity() * D2D1::Matrix3x2F::Translation(10.0f, 20.0f),
		"matrix pop should remove the last transform");
}

void testCacheStorageEvictsLowUsageEntries() {
	kke::CacheStorage<IUnknown> cache(2);
	cache.put(1, makeFakeUnknown());
	cache.put(2, makeFakeUnknown());
	(void)cache.get(1);
	(void)cache.get(1);
	cache.put(3, makeFakeUnknown());

	expect(cache.get(1) != nullptr, "most-used cache entry should be retained");
	expect(cache.get(2) == nullptr, "least-used cache entry should be evicted");
	expect(cache.get(3) != nullptr, "new cache entry should be stored");
}
}  // namespace

int main() {
	const std::vector<std::pair<std::string, std::function<void()>>> tests = {
		{"Color defaults", testColorDefaults},
		{"Point operations", testPointOperations},
		{"Scale value semantics", testScaleValueSemantics},
		{"Geometry hashes", testGeometryHashes},
		{"Matrix build and pop", testMatrixBuildAndPop},
		{"CacheStorage eviction", testCacheStorageEvictsLowUsageEntries},
	};

	size_t failures = 0;
	for (const auto& [name, test] : tests) {
		try {
			test();
			std::cout << "[PASS] " << name << '\n';
		} catch (const std::exception& exception) {
			++failures;
			std::cerr << "[FAIL] " << name << ": " << exception.what() << '\n';
		}
	}

	if (failures != 0) {
		std::cerr << failures << " test(s) failed.\n";
		return 1;
	}

	return 0;
}
