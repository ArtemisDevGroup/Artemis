#include "Vector.h"

template<typename T = float>
class Matrix2x2 {
public:
	Vector2<T> szMatrix[2];

	inline Matrix2x2() : szMatrix() {}
	inline Matrix2x2(Vector2<T> n) {
		szMatrix[0] = n;
		szMatrix[1] = n;
	}
	inline Matrix2x2(Vector2<T> x, Vector2<T> y) {
		szMatrix[0] = x;
		szMatrix[1] = y;
	}

	inline Vector2<T>& X() { return szMatrix[0]; }
	inline Vector2<T>& Y() { return szMatrix[1]; }

	inline Vector2<T>& operator[](int i) { return szMatrix[i]; }
};