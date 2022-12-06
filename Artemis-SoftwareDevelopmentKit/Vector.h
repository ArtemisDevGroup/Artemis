#ifndef __VECTOR_H__
#define __VECTOR_H__

namespace Artemis {
	template<typename T = float>
	class Vector2 {
	public:
		T szVector[2];

		inline Vector2() : szVector() {}
		inline Vector2(T n) {
			szVector[0] = n;
			szVector[1] = n;
		}
		inline Vector2(T x, T y) {
			szVector[0] = x;
			szVector[1] = y;
		}

		inline T& X() { return szVector[0]; }
		inline T& Y() { return szVector[1]; }

		inline T Dot(Vector2<T> v) { return X() * v.X() + Y() * v.Y(); }

		inline T& operator[](int i) { return szVector[i]; }

		inline Vector2<T> operator+(Vector2<T> Right) {
			return Vector2<T>(
				szVector[0] + Right.X(),
				szVector[1] + Right.Y()
				);
		}
		inline Vector2<T> operator-(Vector2<T> Right) {
			return Vector2<T>(
				szVector[0] - Right.X(),
				szVector[1] - Right.Y()
				);
		}
		inline Vector2<T> operator*(Vector2<T> Right) {
			return Vector2<T>(
				szVector[0] * Right.X(),
				szVector[1] * Right.Y()
				);
		}
		inline Vector2<T> operator/(Vector2<T> Right) {
			return Vector2<T>(
				szVector[0] / Right.X(),
				szVector[1] / Right.Y()
				);
		}
		inline void operator+=(Vector2<T> Right) {
			szVector[0] += Right.X();
			szVector[1] += Right.Y();
		}
		inline void operator-=(Vector2<T> Right) {
			szVector[0] -= Right.X();
			szVector[1] -= Right.Y();
		}
		inline void operator*=(Vector2<T> Right) {
			szVector[0] *= Right.X();
			szVector[1] *= Right.Y();
		}
		inline void operator/=(Vector2<T> Right) {
			szVector[0] /= Right.X();
			szVector[1] /= Right.Y();
		}
		inline operator T* () { return szVector; }
	};

	template<typename T = float>
	class Vector3 {
	public:
		T szVector[3];

		inline Vector3() : szVector() {}
		inline Vector3(T n) {
			szVector[0] = n;
			szVector[1] = n;
			szVector[2] = n;
		}
		inline Vector3(T x, T y, T z) {
			szVector[0] = x;
			szVector[1] = y;
			szVector[2] = z;
		}

		inline T& X() { return szVector[0]; }
		inline T& Y() { return szVector[1]; }
		inline T& Z() { return szVector[2]; }

		inline T Dot(Vector3<T> v) { return X() * v.X() + Y() * v.Y() + Z() * v.Z(); }

		inline T& operator[](int i) { return szVector[i]; }

		inline Vector3<T> operator+(Vector3<T> Right) {
			return Vector3<T>(
				szVector[0] + Right.X(),
				szVector[1] + Right.Y(),
				szVector[2] + Right.Z()
				);
		}
		inline Vector3<T> operator-(Vector3<T> Right) {
			return Vector3<T>(
				szVector[0] - Right.X(),
				szVector[1] - Right.Y(),
				szVector[2] - Right.Z()
				);
		}
		inline Vector3<T> operator*(Vector3<T> Right) {
			return Vector3<T>(
				szVector[0] * Right.X(),
				szVector[1] * Right.Y(),
				szVector[2] * Right.Z()
				);
		}
		inline Vector3<T> operator/(Vector3<T> Right) {
			return Vector3<T>(
				szVector[0] / Right.X(),
				szVector[1] / Right.Y(),
				szVector[2] / Right.Z()
				);
		}
		inline void operator+=(Vector3<T> Right) {
			szVector[0] += Right.X();
			szVector[1] += Right.Y();
			szVector[2] += Right.Z();
		}
		inline void operator-=(Vector3<T> Right) {
			szVector[0] -= Right.X();
			szVector[1] -= Right.Y();
			szVector[2] -= Right.Z();
		}
		inline void operator*=(Vector3<T> Right) {
			szVector[0] *= Right.X();
			szVector[1] *= Right.Y();
			szVector[2] *= Right.Z();
		}
		inline void operator/=(Vector3<T> Right) {
			szVector[0] /= Right.X();
			szVector[1] /= Right.Y();
			szVector[2] /= Right.Z();
		}
		inline operator T* () { return szVector; }
	};

	template<typename T = float>
	class Vector4 {
	public:
		T szVector[4];

		inline Vector4() : szVector() {}
		inline Vector4(T n) {
			szVector[0] = n;
			szVector[1] = n;
			szVector[2] = n;
			szVector[3] = n;
		}
		inline Vector4(T x, T y, T z, T w) {
			szVector[0] = x;
			szVector[1] = y;
			szVector[2] = z;
			szVector[3] = w;
		}

		inline T& X() { return szVector[0]; }
		inline T& Y() { return szVector[1]; }
		inline T& Z() { return szVector[2]; }
		inline T& W() { return szVector[3]; }

		inline T Dot(Vector4<T> v) { return X() * v.X() + Y() * v.Y() + Z() * v.Z() + W() * v.W(); }

		inline T& operator[](int i) { return szVector[i]; }

		inline Vector4<T> operator+(Vector4<T> Right) {
			return Vector4<T>(
				szVector[0] + Right.X(),
				szVector[1] + Right.Y(),
				szVector[2] + Right.Z(),
				szVector[3] + Right.W()
				);
		}
		inline Vector4<T> operator-(Vector4<T> Right) {
			return Vector4<T>(
				szVector[0] - Right.X(),
				szVector[1] - Right.Y(),
				szVector[2] - Right.Z(),
				szVector[3] - Right.W()
				);
		}
		inline Vector4<T> operator*(Vector4<T> Right) {
			return Vector4<T>(
				szVector[0] * Right.X(),
				szVector[1] * Right.Y(),
				szVector[2] * Right.Z(),
				szVector[3] * Right.W()
				);
		}
		inline Vector4<T> operator/(Vector4<T> Right) {
			return Vector4<T>(
				szVector[0] / Right.X(),
				szVector[1] / Right.Y(),
				szVector[2] / Right.Z(),
				szVector[3] / Right.W()
				);
		}
		inline void operator+=(Vector4<T> Right) {
			szVector[0] += Right.X();
			szVector[1] += Right.Y();
			szVector[2] += Right.Z();
			szVector[3] += Right.W();
		}
		inline void operator-=(Vector4<T> Right) {
			szVector[0] -= Right.X();
			szVector[1] -= Right.Y();
			szVector[2] -= Right.Z();
			szVector[3] -= Right.W();
		}
		inline void operator*=(Vector4<T> Right) {
			szVector[0] *= Right.X();
			szVector[1] *= Right.Y();
			szVector[2] *= Right.Z();
			szVector[3] *= Right.W();
		}
		inline void operator/=(Vector4<T> Right) {
			szVector[0] /= Right.X();
			szVector[1] /= Right.Y();
			szVector[2] /= Right.Z();
			szVector[3] /= Right.W();
		}
		inline operator T* () { return szVector; }
	};
}

#endif // !__VECTOR_H__