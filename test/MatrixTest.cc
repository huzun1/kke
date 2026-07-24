#include <cassert>
#include <cmath>

#include "kke/appearance/transform/AffineTransform.hh"
#include "kke/engine/d2d/transform/Matrix.hh"

class MatrixTests {
	struct Point {
		float x;
		float y;
	};

	static bool isNear(float left, float right) {
		return std::abs(left - right) < 0.001f;
	}

	static Point apply(D2D1::Matrix3x2F const& matrix, Point const& point) {
		return {
			.x = point.x * matrix._11 + point.y * matrix._21 + matrix._31,
			.y = point.x * matrix._12 + point.y * matrix._22 + matrix._32,
		};
	}

	static void assertPoint(Point const& point, float x, float y) {
		assert(isNear(point.x, x));
		assert(isNear(point.y, y));
	}

	static void combinesChildBeforeParent() {
		kke::Matrix matrix;
		matrix.pushTransform(kke::AffineTransform{
			.xx = 2.0f,
			.xy = 0.0f,
			.yx = 0.0f,
			.yy = 2.0f,
			.tx = -880.0f,
			.ty = 0.0f,
		});
		matrix.pushTransform(kke::Rotation{
			.center = {1094.0f, 0.0f},
			.angle = 180.0f,
		});

		assertPoint(apply(matrix.build(), {1094.0f, 0.0f}), 1308.0f, 0.0f);
	}

	static void buildsFromCanvasLocalBaseDepth() {
		kke::Matrix matrix;
		matrix.pushTransform(kke::Scaling{
			.center = {0.0f, 0.0f},
			.scale = {2.0f, 2.0f},
		});
		size_t canvasBaseDepth = matrix.depth();
		assertPoint(apply(matrix.build(canvasBaseDepth), {10.0f, 10.0f}), 10.0f, 10.0f);

		matrix.pushTransform(kke::Translation{
			.offset = {3.0f, 5.0f},
		});
		assertPoint(apply(matrix.build(canvasBaseDepth), {10.0f, 10.0f}), 13.0f, 15.0f);

		matrix.popTransform();
		assertPoint(apply(matrix.build(canvasBaseDepth), {10.0f, 10.0f}), 10.0f, 10.0f);
		assertPoint(apply(matrix.build(), {10.0f, 10.0f}), 20.0f, 20.0f);
	}

  public:
	static void run() {
		combinesChildBeforeParent();
		buildsFromCanvasLocalBaseDepth();
	}
};

int main() {
	MatrixTests::run();
}
