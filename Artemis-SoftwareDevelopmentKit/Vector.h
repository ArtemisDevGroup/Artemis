//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "Definitions.h"

namespace Artemis {
	/// <summary>
	/// A class for storing for example two-dimensional coordinates.
	/// </summary>
	/// <typeparam name="T">The type of elements to store.</typeparam>
	template<typename T = float>
	class Vector2 {
	public:
		T szVector[2]; // The core array holding the values.

		inline Vector2() : szVector() {}

		/// <summary>
		/// Constructs a vector object setting all elements in the array to the provided element.
		/// </summary>
		/// <param name="n">- The element to set.</param>
		inline Vector2(_In_ T n) {
			szVector[0] = n;
			szVector[1] = n;
		}

		/// <summary>
		/// Constructs a vector and initializes the array.
		/// </summary>
		/// <param name="x">- The first value of the vector.</param>
		/// <param name="y">- The second value of the vector.</param>
		inline Vector2(_In_ T x, _In_ T y) {
			szVector[0] = x;
			szVector[1] = y;
		}

		/// <summary>
		/// Gets a reference to the X slot in the vector.
		/// </summary>
		/// <returns>A reference to X.</returns>
		inline _Check_return_ T& X() { return szVector[0]; }

		/// <summary>
		/// Gets the value of the X slot in the vector.
		/// </summary>
		/// <returns>The value of X.</returns>
		inline _Check_return_ T X() const { return szVector[0]; }

		/// <summary>
		/// Gets a reference to the Y slot in the vector.
		/// </summary>
		/// <returns>A reference to Y.</returns>
		inline _Check_return_ T& Y() { return szVector[1]; }

		/// <summary>
		/// Gets the value of the Y slot in the vector.
		/// </summary>
		/// <returns>The value of Y.</returns>
		inline _Check_return_ T Y() const { return szVector[1]; }

		/// <summary>
		/// Performs a dot calculation on the current vector and the referenced vector.
		/// </summary>
		/// <param name="v">- The second vector to perform the dot operation with.</param>
		/// <returns>The dot value.</returns>
		inline _Check_return_ T Dot(_In_ Vector2<T> v) { return X() * v.X() + Y() * v.Y(); }

		inline _Check_return_ T& operator[](int i) { return szVector[i]; }
		inline _Check_return_ T operator[](int i) const { return szVector[i]; }

		inline _Check_return_ Vector2<T> operator+(Vector2<T> Right) {
			return Vector2<T>(
				szVector[0] + Right.X(),
				szVector[1] + Right.Y()
				);
		}
		inline _Check_return_ Vector2<T> operator-(Vector2<T> Right) {
			return Vector2<T>(
				szVector[0] - Right.X(),
				szVector[1] - Right.Y()
				);
		}
		inline _Check_return_ Vector2<T> operator*(Vector2<T> Right) {
			return Vector2<T>(
				szVector[0] * Right.X(),
				szVector[1] * Right.Y()
				);
		}
		inline _Check_return_ Vector2<T> operator/(Vector2<T> Right) {
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

	/// <summary>
	/// A class for storing for example three-dimensional coordinates or RGB color.
	/// </summary>
	/// <typeparam name="T">The type of elements to store.</typeparam>
	template<typename T = float>
	class Vector3 {
	public:
		T szVector[3]; // The core array holding the values.

		inline Vector3() : szVector() {}

		/// <summary>
		/// Constructs a vector object setting all elements in the array to the provided element.
		/// </summary>
		/// <param name="n">- The element to set.</param>
		inline Vector3(_In_ T n) {
			szVector[0] = n;
			szVector[1] = n;
			szVector[2] = n;
		}

		/// <summary>
		/// Constructs a vector and initializes the array.
		/// </summary>
		/// <param name="x">- The first value of the vector.</param>
		/// <param name="y">- The second value of the vector.</param>
		/// <param name="z">- The third value of the vector.</param>
		inline Vector3(_In_ T x, _In_ T y, _In_ T z) {
			szVector[0] = x;
			szVector[1] = y;
			szVector[2] = z;
		}

		/// <summary>
		/// Gets a reference to the X slot in the vector.
		/// </summary>
		/// <returns>A reference to X.</returns>
		inline _Check_return_ T& X() { return szVector[0]; }

		/// <summary>
		/// Gets the value of the X slot in the vector.
		/// </summary>
		/// <returns>The value of X.</returns>
		inline _Check_return_ T X() const { return szVector[0]; }

		/// <summary>
		/// Gets a reference to the Y slot in the vector.
		/// </summary>
		/// <returns>A reference to Y.</returns>
		inline _Check_return_ T& Y() { return szVector[1]; }

		/// <summary>
		/// Gets the value of the Y slot in the vector.
		/// </summary>
		/// <returns>The value of Y.</returns>
		inline _Check_return_ T Y() const { return szVector[1]; }

		/// <summary>
		/// Gets a reference to the Z slot in the vector.
		/// </summary>
		/// <returns>A reference to Z.</returns>
		inline _Check_return_ T& Z() { return szVector[2]; }

		/// <summary>
		/// Gets the value of the Z slot in the vector.
		/// </summary>
		/// <returns>The value of Z.</returns>
		inline _Check_return_ T Z() const { return szVector[2]; }

		/// <summary>
		/// Performs a dot calculation on the current vector and the referenced vector.
		/// </summary>
		/// <param name="v">- The second vector to perform the dot operation with.</param>
		/// <returns>The dot value.</returns>
		inline _Check_return_ T Dot(_In_ Vector3<T> v) { return X() * v.X() + Y() * v.Y() + Z() * v.Z(); }

		inline _Check_return_ T& operator[](int i) { return szVector[i]; }
		inline _Check_return_ T operator[](int i) const { return szVector[i]; }

		inline _Check_return_ Vector3<T> operator+(Vector3<T> Right) {
			return Vector3<T>(
				szVector[0] + Right.X(),
				szVector[1] + Right.Y(),
				szVector[2] + Right.Z()
				);
		}
		inline _Check_return_ Vector3<T> operator-(Vector3<T> Right) {
			return Vector3<T>(
				szVector[0] - Right.X(),
				szVector[1] - Right.Y(),
				szVector[2] - Right.Z()
				);
		}
		inline _Check_return_ Vector3<T> operator*(Vector3<T> Right) {
			return Vector3<T>(
				szVector[0] * Right.X(),
				szVector[1] * Right.Y(),
				szVector[2] * Right.Z()
				);
		}
		inline _Check_return_ Vector3<T> operator/(Vector3<T> Right) {
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

	/// <summary>
	/// A cvlass for storing for example quaternions or RGBA color.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T = float>
	class Vector4 {
	public:
		T szVector[4]; // The core array holding the values.

		inline Vector4() : szVector() {}

		/// <summary>
		/// Constructs a vector object setting all elements in the array to the provided element.
		/// </summary>
		/// <param name="n">- The element to set.</param>
		inline Vector4(_In_ T n) {
			szVector[0] = n;
			szVector[1] = n;
			szVector[2] = n;
			szVector[3] = n;
		}

		/// <summary>
		/// Constructs a vector and initializes the array.
		/// </summary>
		/// <param name="x">- The first value of the vector.</param>
		/// <param name="y">- The second value of the vector.</param>
		/// <param name="z">- The third value of the vector.</param>
		/// <param name="w">- The fourth value of the vector.</param>
		inline Vector4(_In_ T x, _In_ T y, _In_ T z, _In_ T w) {
			szVector[0] = x;
			szVector[1] = y;
			szVector[2] = z;
			szVector[3] = w;
		}

		/// <summary>
		/// Gets a reference to the X slot in the vector.
		/// </summary>
		/// <returns>A reference to X.</returns>
		inline _Check_return_ T& X() { return szVector[0]; }

		/// <summary>
		/// Gets the value of the X slot in the vector.
		/// </summary>
		/// <returns>The value of X.</returns>
		inline _Check_return_ T X() const { return szVector[0]; }

		/// <summary>
		/// Gets a reference to the Y slot in the vector.
		/// </summary>
		/// <returns>A reference to Y.</returns>
		inline _Check_return_ T& Y() { return szVector[1]; }

		/// <summary>
		/// Gets the value of the Y slot in the vector.
		/// </summary>
		/// <returns>The value of Y.</returns>
		inline _Check_return_ T Y() const { return szVector[1]; }

		/// <summary>
		/// Gets a reference to the Z slot in the vector.
		/// </summary>
		/// <returns>A reference to Z.</returns>
		inline _Check_return_ T& Z() { return szVector[2]; }

		/// <summary>
		/// Gets the value of the Z slot in the vector.
		/// </summary>
		/// <returns>The value of Z.</returns>
		inline _Check_return_ T Z() const { return szVector[2]; }

		/// <summary>
		/// Gets a reference to the W slot in the vector.
		/// </summary>
		/// <returns>A reference to W.</returns>
		inline _Check_return_ T& W() { return szVector[3]; }

		/// <summary>
		/// Gets the value of the W slot in the vector.
		/// </summary>
		/// <returns>The value of W.</returns>
		inline _Check_return_ T W() const { return szVector[3]; }

		/// <summary>
		/// Performs a dot calculation on the current vector and the referenced vector.
		/// </summary>
		/// <param name="v">- The second vector to perform the dot operation with.</param>
		/// <returns>The dot value.</returns>
		inline _Check_return_ T Dot(_In_ Vector4<T> v) { return X() * v.X() + Y() * v.Y() + Z() * v.Z() + W() * v.W(); }

		inline _Check_return_ T& operator[](int i) { return szVector[i]; }
		inline _Check_return_ T operator[](int i) const { return szVector[i]; }

		inline _Check_return_ Vector4<T> operator+(Vector4<T> Right) {
			return Vector4<T>(
				szVector[0] + Right.X(),
				szVector[1] + Right.Y(),
				szVector[2] + Right.Z(),
				szVector[3] + Right.W()
				);
		}
		inline _Check_return_ Vector4<T> operator-(Vector4<T> Right) {
			return Vector4<T>(
				szVector[0] - Right.X(),
				szVector[1] - Right.Y(),
				szVector[2] - Right.Z(),
				szVector[3] - Right.W()
				);
		}
		inline _Check_return_ Vector4<T> operator*(Vector4<T> Right) {
			return Vector4<T>(
				szVector[0] * Right.X(),
				szVector[1] * Right.Y(),
				szVector[2] * Right.Z(),
				szVector[3] * Right.W()
				);
		}
		inline _Check_return_ Vector4<T> operator/(Vector4<T> Right) {
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