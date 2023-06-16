//------------------------------------------------------------------------>
// MIT License
// 
// Copyright (c) 2023 Artemis Group
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------>
// Aurora: https://github.com/ArtemisDevGroup/Aurora
// This file was authored by:
// @Astrea0014: https://github.com/Astrea0014
//------------------------------------------------------------------------>

#ifndef __AURORA_VECTOR_H__
#define __AURORA_VECTOR_H__

#include "Definitions.h"

namespace Aurora {
	/// <summary>
	/// A class for storing for example two-dimensional coordinates.
	/// </summary>
	/// <typeparam name="T">The type of elements to store.</typeparam>
	template<ArithmeticType T = A_FL32>
	struct Vector2 {
		T szVector[2]; // The core array holding the values.

		constexpr Vector2() noexcept : szVector() {}

		/// <summary>
		/// Constructs a vector object setting all elements in the array to the provided element.
		/// </summary>
		/// <param name="n">- The element to set.</param>
		constexpr Vector2(_In_ const T& n) noexcept {
			szVector[0] = n;
			szVector[1] = n;
		}

		/// <summary>
		/// Constructs a vector and initializes the array.
		/// </summary>
		/// <param name="x">- The first value of the vector.</param>
		/// <param name="y">- The second value of the vector.</param>
		constexpr Vector2(_In_ const T& x, _In_ const T& y) noexcept {
			szVector[0] = x;
			szVector[1] = y;
		}

		/// <summary>
		/// Gets a reference to the X slot in the vector.
		/// </summary>
		/// <returns>A reference to X.</returns>
		constexpr _Check_return_ T& X() noexcept { return szVector[0]; }

		/// <summary>
		/// Gets the value of the X slot in the vector.
		/// </summary>
		/// <returns>The value of X.</returns>
		constexpr _Check_return_ const T& X() const noexcept { return szVector[0]; }

		/// <summary>
		/// Gets a reference to the Y slot in the vector.
		/// </summary>
		/// <returns>A reference to Y.</returns>
		constexpr _Check_return_ T& Y() noexcept { return szVector[1]; }

		/// <summary>
		/// Gets the value of the Y slot in the vector.
		/// </summary>
		/// <returns>The value of Y.</returns>
		constexpr _Check_return_ const T& Y() const noexcept { return szVector[1]; }

		/// <summary>
		/// Performs a dot calculation on the current vector and the referenced vector.
		/// </summary>
		/// <param name="v">- The second vector to perform the dot operation with.</param>
		/// <returns>The dot value.</returns>
		constexpr _Check_return_ T Dot(_In_ const Vector2<T>& v) const noexcept { return X() * v.X() + Y() * v.Y(); }

		constexpr _Check_return_ T& operator[](int i) noexcept { return szVector[i]; }
		constexpr _Check_return_ const T& operator[](int i) const noexcept { return szVector[i]; }

		constexpr _Check_return_ Vector2<T> operator+(const Vector2<T>& Right) const noexcept {
			return Vector2<T>(
				szVector[0] + Right.X(),
				szVector[1] + Right.Y()
				);
		}
		constexpr _Check_return_ Vector2<T> operator-(const Vector2<T>& Right) const noexcept {
			return Vector2<T>(
				szVector[0] - Right.X(),
				szVector[1] - Right.Y()
				);
		}
		constexpr _Check_return_ Vector2<T> operator*(const Vector2<T>& Right) const noexcept {
			return Vector2<T>(
				szVector[0] * Right.X(),
				szVector[1] * Right.Y()
				);
		}
		constexpr _Check_return_ Vector2<T> operator/(const Vector2<T>& Right) const noexcept {
			return Vector2<T>(
				szVector[0] / Right.X(),
				szVector[1] / Right.Y()
				);
		}
		constexpr void operator+=(const Vector2<T>& Right) noexcept {
			szVector[0] += Right.X();
			szVector[1] += Right.Y();
		}
		constexpr void operator-=(const Vector2<T>& Right) noexcept {
			szVector[0] -= Right.X();
			szVector[1] -= Right.Y();
		}
		constexpr void operator*=(const Vector2<T>& Right) noexcept {
			szVector[0] *= Right.X();
			szVector[1] *= Right.Y();
		}
		constexpr void operator/=(const Vector2<T>& Right) noexcept {
			szVector[0] /= Right.X();
			szVector[1] /= Right.Y();
		}

		constexpr operator T* () noexcept { return szVector; }
		constexpr operator const T* () const noexcept { return szVector; }
	};

	/// <summary>
	/// A class for storing for example three-dimensional coordinates or RGB color.
	/// </summary>
	/// <typeparam name="T">The type of elements to store.</typeparam>
	template<ArithmeticType T = A_FL32>
	struct Vector3 {
		T szVector[3]; // The core array holding the values.

		constexpr Vector3() noexcept : szVector() {}

		/// <summary>
		/// Constructs a vector object setting all elements in the array to the provided element.
		/// </summary>
		/// <param name="n">- The element to set.</param>
		constexpr Vector3(_In_ const T& n) noexcept {
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
		constexpr Vector3(_In_ const T& x, _In_ const T& y, _In_ const T& z) noexcept {
			szVector[0] = x;
			szVector[1] = y;
			szVector[2] = z;
		}

		/// <summary>
		/// Gets a reference to the X slot in the vector.
		/// </summary>
		/// <returns>A reference to X.</returns>
		constexpr _Check_return_ T& X() noexcept { return szVector[0]; }

		/// <summary>
		/// Gets the value of the X slot in the vector.
		/// </summary>
		/// <returns>The value of X.</returns>
		constexpr _Check_return_ T X() const noexcept { return szVector[0]; }

		/// <summary>
		/// Gets a reference to the Y slot in the vector.
		/// </summary>
		/// <returns>A reference to Y.</returns>
		constexpr _Check_return_ T& Y() noexcept { return szVector[1]; }

		/// <summary>
		/// Gets the value of the Y slot in the vector.
		/// </summary>
		/// <returns>The value of Y.</returns>
		constexpr _Check_return_ T Y() const noexcept { return szVector[1]; }

		/// <summary>
		/// Gets a reference to the Z slot in the vector.
		/// </summary>
		/// <returns>A reference to Z.</returns>
		constexpr _Check_return_ T& Z() noexcept { return szVector[2]; }

		/// <summary>
		/// Gets the value of the Z slot in the vector.
		/// </summary>
		/// <returns>The value of Z.</returns>
		constexpr _Check_return_ T Z() const noexcept { return szVector[2]; }

		/// <summary>
		/// Performs a dot calculation on the current vector and the referenced vector.
		/// </summary>
		/// <param name="v">- The second vector to perform the dot operation with.</param>
		/// <returns>The dot value.</returns>
		constexpr _Check_return_ T Dot(_In_ const Vector3<T>& v) const noexcept { return X() * v.X() + Y() * v.Y() + Z() * v.Z(); }

		constexpr _Check_return_ T& operator[](int i) noexcept { return szVector[i]; }
		constexpr _Check_return_ const T& operator[](int i) const noexcept { return szVector[i]; }

		constexpr _Check_return_ Vector3<T> operator+(const Vector3<T>& Right) const noexcept {
			return Vector3<T>(
				szVector[0] + Right.X(),
				szVector[1] + Right.Y(),
				szVector[2] + Right.Z()
				);
		}
		constexpr _Check_return_ Vector3<T> operator-(const Vector3<T>& Right) const noexcept {
			return Vector3<T>(
				szVector[0] - Right.X(),
				szVector[1] - Right.Y(),
				szVector[2] - Right.Z()
				);
		}
		constexpr _Check_return_ Vector3<T> operator*(const Vector3<T>& Right) const noexcept {
			return Vector3<T>(
				szVector[0] * Right.X(),
				szVector[1] * Right.Y(),
				szVector[2] * Right.Z()
				);
		}
		constexpr _Check_return_ Vector3<T> operator/(const Vector3<T>& Right) const noexcept {
			return Vector3<T>(
				szVector[0] / Right.X(),
				szVector[1] / Right.Y(),
				szVector[2] / Right.Z()
				);
		}
		constexpr void operator+=(const Vector3<T>& Right) noexcept {
			szVector[0] += Right.X();
			szVector[1] += Right.Y();
			szVector[2] += Right.Z();
		}
		constexpr void operator-=(const Vector3<T>& Right) noexcept {
			szVector[0] -= Right.X();
			szVector[1] -= Right.Y();
			szVector[2] -= Right.Z();
		}
		constexpr void operator*=(const Vector3<T>& Right) noexcept {
			szVector[0] *= Right.X();
			szVector[1] *= Right.Y();
			szVector[2] *= Right.Z();
		}
		constexpr void operator/=(const Vector3<T>& Right) noexcept {
			szVector[0] /= Right.X();
			szVector[1] /= Right.Y();
			szVector[2] /= Right.Z();
		}
		constexpr operator T* () noexcept { return szVector; }
		constexpr operator const T* () const noexcept { return szVector; }
	};

	/// <summary>
	/// A class for storing for example quaternions or RGBA color.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<ArithmeticType T = A_FL32>
	struct Vector4 {
		T szVector[4]; // The core array holding the values.

		constexpr Vector4() noexcept : szVector() {}

		/// <summary>
		/// Constructs a vector object setting all elements in the array to the provided element.
		/// </summary>
		/// <param name="n">- The element to set.</param>
		constexpr Vector4(_In_ const T& n) noexcept {
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
		constexpr Vector4(_In_ const T& x, _In_ const T& y, _In_ const T& z, _In_ const T& w) noexcept {
			szVector[0] = x;
			szVector[1] = y;
			szVector[2] = z;
			szVector[3] = w;
		}

		/// <summary>
		/// Gets a reference to the X slot in the vector.
		/// </summary>
		/// <returns>A reference to X.</returns>
		constexpr _Check_return_ T& X() noexcept { return szVector[0]; }

		/// <summary>
		/// Gets the value of the X slot in the vector.
		/// </summary>
		/// <returns>The value of X.</returns>
		constexpr _Check_return_ const T& X() const noexcept { return szVector[0]; }

		/// <summary>
		/// Gets a reference to the Y slot in the vector.
		/// </summary>
		/// <returns>A reference to Y.</returns>
		constexpr _Check_return_ T& Y() noexcept { return szVector[1]; }

		/// <summary>
		/// Gets the value of the Y slot in the vector.
		/// </summary>
		/// <returns>The value of Y.</returns>
		constexpr _Check_return_ const T& Y() const noexcept { return szVector[1]; }

		/// <summary>
		/// Gets a reference to the Z slot in the vector.
		/// </summary>
		/// <returns>A reference to Z.</returns>
		constexpr _Check_return_ T& Z() noexcept { return szVector[2]; }

		/// <summary>
		/// Gets the value of the Z slot in the vector.
		/// </summary>
		/// <returns>The value of Z.</returns>
		constexpr _Check_return_ const T& Z() const noexcept { return szVector[2]; }

		/// <summary>
		/// Gets a reference to the W slot in the vector.
		/// </summary>
		/// <returns>A reference to W.</returns>
		constexpr _Check_return_ T& W() noexcept { return szVector[3]; }

		/// <summary>
		/// Gets the value of the W slot in the vector.
		/// </summary>
		/// <returns>The value of W.</returns>
		constexpr _Check_return_ const T& W() const noexcept { return szVector[3]; }

		/// <summary>
		/// Performs a dot calculation on the current vector and the referenced vector.
		/// </summary>
		/// <param name="v">- The second vector to perform the dot operation with.</param>
		/// <returns>The dot value.</returns>
		constexpr _Check_return_ T Dot(_In_ const Vector4<T>& v) const noexcept { return X() * v.X() + Y() * v.Y() + Z() * v.Z() + W() * v.W(); }

		constexpr _Check_return_ T& operator[](int i) noexcept { return szVector[i]; }
		constexpr _Check_return_ const T& operator[](int i) const noexcept { return szVector[i]; }

		constexpr _Check_return_ Vector4<T> operator+(const Vector4<T>& Right) const noexcept {
			return Vector4<T>(
				szVector[0] + Right.X(),
				szVector[1] + Right.Y(),
				szVector[2] + Right.Z(),
				szVector[3] + Right.W()
				);
		}
		constexpr _Check_return_ Vector4<T> operator-(const Vector4<T>& Right) const noexcept {
			return Vector4<T>(
				szVector[0] - Right.X(),
				szVector[1] - Right.Y(),
				szVector[2] - Right.Z(),
				szVector[3] - Right.W()
				);
		}
		constexpr _Check_return_ Vector4<T> operator*(const Vector4<T>& Right) const noexcept {
			return Vector4<T>(
				szVector[0] * Right.X(),
				szVector[1] * Right.Y(),
				szVector[2] * Right.Z(),
				szVector[3] * Right.W()
				);
		}
		constexpr _Check_return_ Vector4<T> operator/(const Vector4<T>& Right) const noexcept {
			return Vector4<T>(
				szVector[0] / Right.X(),
				szVector[1] / Right.Y(),
				szVector[2] / Right.Z(),
				szVector[3] / Right.W()
				);
		}
		constexpr void operator+=(const Vector4<T>& Right) noexcept {
			szVector[0] += Right.X();
			szVector[1] += Right.Y();
			szVector[2] += Right.Z();
			szVector[3] += Right.W();
		}
		constexpr void operator-=(const Vector4<T>& Right) noexcept {
			szVector[0] -= Right.X();
			szVector[1] -= Right.Y();
			szVector[2] -= Right.Z();
			szVector[3] -= Right.W();
		}
		constexpr void operator*=(const Vector4<T>& Right) noexcept {
			szVector[0] *= Right.X();
			szVector[1] *= Right.Y();
			szVector[2] *= Right.Z();
			szVector[3] *= Right.W();
		}
		constexpr void operator/=(const Vector4<T>& Right) noexcept {
			szVector[0] /= Right.X();
			szVector[1] /= Right.Y();
			szVector[2] /= Right.Z();
			szVector[3] /= Right.W();
		}
		constexpr operator T* () noexcept { return szVector; }
		constexpr operator const T* () const noexcept { return szVector; }
	};
}

#endif // !__AURORA_VECTOR_H__