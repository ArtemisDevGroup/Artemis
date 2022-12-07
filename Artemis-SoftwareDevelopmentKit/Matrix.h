#include "Vector.h"

#ifndef __MATRIX_H__
#define __MATRIX_H__

namespace Artemis {

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
		inline Vector2<T> X() const { return szMatrix[0]; }
		inline Vector2<T>& Y() { return szMatrix[1]; }
		inline Vector2<T> Y() const { return szMatrix[1]; }

		inline Vector2<T>& operator[](int i) { return szMatrix[i]; }
		inline Vector2<T> operator[](int i) const { return szMatrix[i]; }
	};

	template<typename T = float>
	class Matrix3x3 {
	public:
		Vector3<T> szMatrix[3];

		inline Matrix3x3() : szMatrix() {}
		inline Matrix3x3(Vector3<T> n) {
			szMatrix[0] = n;
			szMatrix[1] = n;
			szMatrix[2] = n;
		}
		inline Matrix3x3(Vector3<T> x, Vector3<T> y, Vector3<T> z) {
			szMatrix[0] = x;
			szMatrix[1] = y;
			szMatrix[2] = z;
		}

		inline Vector3<T>& X() { return szMatrix[0]; }
		inline Vector3<T> X() const { return szMatrix[0]; }
		inline Vector3<T>& Y() { return szMatrix[1]; }
		inline Vector3<T> Y() const { return szMatrix[1]; }
		inline Vector3<T>& Z() { return szMatrix[2]; }
		inline Vector3<T> Z() const { return szMatrix[2]; }

		inline Vector3<T>& operator[](int i) { return szMatrix[i]; }
		inline Vector3<T> operator[](int i) const { return szMatrix[i]; }
	};

	template<typename T = float>
	class Matrix4x4 {
	public:
		Vector4<T> szMatrix[4];

		inline Matrix4x4() : szMatrix() {}
		inline Matrix4x4(Vector4<T> n) {
			szMatrix[0] = n;
			szMatrix[1] = n;
			szMatrix[2] = n;
			szMatrix[3] = n;
		}
		inline Matrix4x4(Vector4<T> x, Vector4<T> y, Vector4<T> z, Vector4<T> w) {
			szMatrix[0] = x;
			szMatrix[1] = y;
			szMatrix[2] = z;
			szMatrix[3] = w:
		}

		inline Vector4<T>& X() { return szMatrix[0]; }
		inline Vector4<T> X() const { return szMatrix[0]; }
		inline Vector4<T>& Y() { return szMatrix[1]; }
		inline Vector4<T> Y() const { return szMatrix[1]; }
		inline Vector4<T>& Z() { return szMatrix[2]; }
		inline Vector4<T> Z() const { return szMatrix[2]; }
		inline Vector4<T>& W() { return szMatrix[3]; }
		inline Vector4<T> W() const { return szMatrix[3]; }

		inline Vector4<T>& operator[](int i) { return szMatrix[i]; }
		inline Vector4<T> operator[](int i) const { return szMatrix[i]; }
	};

}

#endif // !__MATRIX_H__