#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "Def.h"

#include <stdlib.h> // calloc
#include <Windows.h> // RaiseException, EXCEPTION_ARRAY_BOUNDS_EXCEEDED

#ifdef __cplusplus

/// <summary>
/// A vector of size 2. Is intended to store coordinates in 2d space (XY).
/// </summary>
/// <typeparam name="T">- The type of the vector.</typeparam>
template<typename T = float>
class Vec2 {
public:
	T Vec[2];

	/// <summary>
	/// Constructs a null object.
	/// </summary>
	inline Vec2() : Vec() {}

	/// <summary>
	/// Constructs a Vec2 object and sets all values to n.
	/// </summary>
	/// <param name="n">- The value to set.</param>
	inline Vec2(T n) {
		Vec[0] = n;
		Vec[1] = n;
	}

	/// <summary>
	/// Constructs a Vec2 object.
	/// </summary>
	/// <param name="x">- The x value.</param>
	/// <param name="y">- The y value.</param>
	inline Vec2(T x, T y) {
		Vec[0] = x;
		Vec[1] = y;
	}

#ifdef __VECTOR_OPERATOR_H__
	inline void operator+=(Vec2<T> vRight) { Vec[0] += vRight.X(); Vec[1] += vRight.Y(); }
	inline void operator-=(Vec2<T> vRight) { Vec[0] -= vRight.X(); Vec[1] -= vRight.Y(); }
	inline void operator*=(Vec2<T> vRight) { Vec[0] *= vRight.X(); Vec[1] *= vRight.Y(); }
	inline void operator/=(Vec2<T> vRight) { Vec[0] /= vRight.X(); Vec[1] /= vRight.Y(); }
	inline void operator&=(Vec2<T> vRight) { Vec[0] &= vRight.X(); Vec[1] &= vRight.Y(); }
	inline void operator|=(Vec2<T> vRight) { Vec[0] |= vRight.X(); Vec[1] |= vRight.Y(); }
	inline void operator^=(Vec2<T> vRight) { Vec[0] ^= vRight.X(); Vec[1] ^= vRight.Y(); }
#endif

	/// <summary>
	/// Gets a reference to X, to read and write to.
	/// </summary>
	/// <returns>A reference to X to modify.</returns>
	inline T& X() { return Vec[0]; }
	
	/// <summary>
	/// Gets a reference to Y, to read and write to.
	/// </summary>
	/// <returns>A reference to Y to modify.</returns>
	inline T& Y() { return Vec[1]; }
};

/// <summary>
/// A vector of size 3. Is intended to store either coordinates in 3d space (XYZ), or a colour (RGB).
/// </summary>
/// <typeparam name="T">- The type of the vector.</typeparam>
template<typename T = float>
class Vec3 {
public:
	T Vec[3];

	/// <summary>
	/// Constructs a null object.
	/// </summary>
	inline Vec3() : Vec() {}

	/// <summary>
	/// Constructs a Vec3 object and sets all values to n.
	/// </summary>
	/// <param name="n">- The value to set.</param>
	inline Vec3(T n) {
		Vec[0] = n;
		Vec[1] = n;
		Vec[2] = n;
	}

	/// <summary>
	/// Extends a Vec2 into a Vec3 and fills the additonal z value with provided value.
	/// </summary>
	/// <param name="vVec">- The Vec2 to extend.</param>
	/// <param name="z">- The z value.</param>
	inline Vec3(Vec2<T> vVec, T z) {
		Vec[0] = vVec.X();
		Vec[1] = vVec.Y();
		Vec[2] = z;
	}

	/// <summary>
	/// Constructs a Vec3 object.
	/// </summary>
	/// <param name="x">- The x/r value.</param>
	/// <param name="y">- The y/g value.</param>
	/// <param name="z">- The z/b value.</param>
	inline Vec3(T x, T y, T z) {
		Vec[0] = x;
		Vec[1] = y;
		Vec[2] = z;
	}

#ifdef __VECTOR_OPERATOR_H__
	inline void operator+=(Vec2<T> vRight) { Vec[0] += vRight.X(); Vec[1] += vRight.Y(); }
	inline void operator-=(Vec2<T> vRight) { Vec[0] -= vRight.X(); Vec[1] -= vRight.Y(); }
	inline void operator*=(Vec2<T> vRight) { Vec[0] *= vRight.X(); Vec[1] *= vRight.Y(); }
	inline void operator/=(Vec2<T> vRight) { Vec[0] /= vRight.X(); Vec[1] /= vRight.Y(); }
	inline void operator&=(Vec2<T> vRight) { Vec[0] &= vRight.X(); Vec[1] &= vRight.Y(); }
	inline void operator|=(Vec2<T> vRight) { Vec[0] |= vRight.X(); Vec[1] |= vRight.Y(); }
	inline void operator^=(Vec2<T> vRight) { Vec[0] ^= vRight.X(); Vec[1] ^= vRight.Y(); }

	inline void operator+=(Vec3<T> vRight) { Vec[0] += vRight.X(); Vec[1] += vRight.Y(); Vec[2] += vRight.Z(); }
	inline void operator-=(Vec3<T> vRight) { Vec[0] -= vRight.X(); Vec[1] -= vRight.Y(); Vec[2] -= vRight.Z(); }
	inline void operator*=(Vec3<T> vRight) { Vec[0] *= vRight.X(); Vec[1] *= vRight.Y(); Vec[2] *= vRight.Z(); }
	inline void operator/=(Vec3<T> vRight) { Vec[0] /= vRight.X(); Vec[1] /= vRight.Y(); Vec[2] /= vRight.Z(); }
	inline void operator&=(Vec3<T> vRight) { Vec[0] &= vRight.X(); Vec[1] &= vRight.Y(); Vec[2] &= vRight.Z(); }
	inline void operator|=(Vec3<T> vRight) { Vec[0] |= vRight.X(); Vec[1] |= vRight.Y(); Vec[2] |= vRight.Z(); }
	inline void operator^=(Vec3<T> vRight) { Vec[0] ^= vRight.X(); Vec[1] ^= vRight.Y(); Vec[2] ^= vRight.Z(); }
#endif

	/// <summary>
	/// Gets a reference to X, to read and write to.
	/// </summary>
	/// <returns>A reference to X to modify.</returns>
	inline T& X() { return Vec[0]; }

	/// <summary>
	/// Gets a reference to Y, to read and write to.
	/// </summary>
	/// <returns>A reference to Y to modify.</returns>
	inline T& Y() { return Vec[1]; }

	/// <summary>
	/// Gets a reference to Z, to read and write to.
	/// </summary>
	/// <returns>A reference to Z to modify.</returns>
	inline T& Z() { return Vec[2]; }

	/// <summary>
	/// Gets a reference to R, to read and write to. R is the same as X.
	/// </summary>
	/// <returns>A reference to R to modify.</returns>
	inline T& R() { return Vec[0]; }

	/// <summary>
	/// Gets a reference to G, to read and write to. G is the same as Y.
	/// </summary>
	/// <returns>A reference to G to modify.</returns>
	inline T& G() { return Vec[1]; }

	/// <summary>
	/// Gets a reference to B, to read and write to. B is the same as Z.
	/// </summary>
	/// <returns>A reference to B to modify.</returns>
	inline T& B() { return Vec[2]; }
};

/// <summary>
/// A vector of size 4. Is intended to store either mesh tangents, parameters for shaders, etc... (XYZW), or a colour with alpha (RGBA).
/// </summary>
/// <typeparam name="T">- The type of the vector.</typeparam>
template<typename T = float>
class Vec4 {
public:
	/// <summary>
	/// The vector as a C style array.
	/// </summary>
	T Vec[4];

	/// <summary>
	/// Constructs a null object.
	/// </summary>
	inline Vec4() : Vec() {}

	/// <summary>
	/// Constructs a Vec4 object and sets all values to n.
	/// </summary>
	/// <param name="n">- The value to set.</param>
	inline Vec4(T n) {
		Vec[0] = n;
		Vec[1] = n;
		Vec[2] = n;
		Vec[3] = n;
	}

	/// <summary>
	/// Extends a Vec3 into a Vec4 and fills the additonal w value with provided value.
	/// </summary>
	/// <param name="vVec">- The Vec3 to extend.</param>
	/// <param name="w">- The z value.</param>
	inline Vec4(Vec3<T> vVec, T w) {
		Vec[0] = vVec.X();
		Vec[1] = vVec.Y();
		Vec[2] = vVec.Z();
		Vec[3] = w;
	}

	/// <summary>
	/// Constructs a Vec4 object.
	/// </summary>
	/// <param name="x">- The x/r value.</param>
	/// <param name="y">- The y/g value.</param>
	/// <param name="z">- The z/b value.</param>
	/// <param name="w">- THe w/a value.</param>
	inline Vec4(T x, T y, T z, T w) {
		Vec[0] = x;
		Vec[1] = y;
		Vec[2] = z;
		Vec[3] = w;
	}

#ifdef __VECTOR_OPERATOR_H__
	inline void operator+=(Vec2<T> vRight) { Vec[0] += vRight.X(); Vec[1] += vRight.Y(); }
	inline void operator-=(Vec2<T> vRight) { Vec[0] -= vRight.X(); Vec[1] -= vRight.Y(); }
	inline void operator*=(Vec2<T> vRight) { Vec[0] *= vRight.X(); Vec[1] *= vRight.Y(); }
	inline void operator/=(Vec2<T> vRight) { Vec[0] /= vRight.X(); Vec[1] /= vRight.Y(); }
	inline void operator&=(Vec2<T> vRight) { Vec[0] &= vRight.X(); Vec[1] &= vRight.Y(); }
	inline void operator|=(Vec2<T> vRight) { Vec[0] |= vRight.X(); Vec[1] |= vRight.Y(); }
	inline void operator^=(Vec2<T> vRight) { Vec[0] ^= vRight.X(); Vec[1] ^= vRight.Y(); }

	inline void operator+=(Vec3<T> vRight) { Vec[0] += vRight.X(); Vec[1] += vRight.Y(); Vec[2] += vRight.Z(); }
	inline void operator-=(Vec3<T> vRight) { Vec[0] -= vRight.X(); Vec[1] -= vRight.Y(); Vec[2] -= vRight.Z(); }
	inline void operator*=(Vec3<T> vRight) { Vec[0] *= vRight.X(); Vec[1] *= vRight.Y(); Vec[2] *= vRight.Z(); }
	inline void operator/=(Vec3<T> vRight) { Vec[0] /= vRight.X(); Vec[1] /= vRight.Y(); Vec[2] /= vRight.Z(); }
	inline void operator&=(Vec3<T> vRight) { Vec[0] &= vRight.X(); Vec[1] &= vRight.Y(); Vec[2] &= vRight.Z(); }
	inline void operator|=(Vec3<T> vRight) { Vec[0] |= vRight.X(); Vec[1] |= vRight.Y(); Vec[2] |= vRight.Z(); }
	inline void operator^=(Vec3<T> vRight) { Vec[0] ^= vRight.X(); Vec[1] ^= vRight.Y(); Vec[2] ^= vRight.Z(); }

	inline void operator+=(Vec4<T> vRight) { Vec[0] += vRight.X(); Vec[1] += vRight.Y(); Vec[2] += vRight.Z(); Vec[3] += vRight.W(); }
	inline void operator-=(Vec4<T> vRight) { Vec[0] -= vRight.X(); Vec[1] -= vRight.Y(); Vec[2] -= vRight.Z(); Vec[3] -= vRight.W(); }
	inline void operator*=(Vec4<T> vRight) { Vec[0] *= vRight.X(); Vec[1] *= vRight.Y(); Vec[2] *= vRight.Z(); Vec[3] *= vRight.W(); }
	inline void operator/=(Vec4<T> vRight) { Vec[0] /= vRight.X(); Vec[1] /= vRight.Y(); Vec[2] /= vRight.Z(); Vec[3] /= vRight.W(); }
	inline void operator&=(Vec4<T> vRight) { Vec[0] &= vRight.X(); Vec[1] &= vRight.Y(); Vec[2] &= vRight.Z(); Vec[3] &= vRight.W(); }
	inline void operator|=(Vec4<T> vRight) { Vec[0] |= vRight.X(); Vec[1] |= vRight.Y(); Vec[2] |= vRight.Z(); Vec[3] |= vRight.W(); }
	inline void operator^=(Vec4<T> vRight) { Vec[0] ^= vRight.X(); Vec[1] ^= vRight.Y(); Vec[2] ^= vRight.Z(); Vec[3] ^= vRight.W(); }
#endif

	/// <summary>
	/// Gets a reference to X, to read and write to.
	/// </summary>
	/// <returns>A reference to X to modify.</returns>
	inline T& X() { return Vec[0]; }

	/// <summary>
	/// Gets a reference to Y, to read and write to.
	/// </summary>
	/// <returns>A reference to Y to modify.</returns>
	inline T& Y() { return Vec[1]; }

	/// <summary>
	/// Gets a reference to Z, to read and write to.
	/// </summary>
	/// <returns>A reference to Z to modify.</returns>
	inline T& Z() { return Vec[2]; }

	/// <summary>
	/// Gets a reference to W, to read and write to.
	/// </summary>
	/// <returns>A reference to W to modify.</returns>
	inline T& W() { return Vec[3]; }

	/// <summary>
	/// Gets a reference to R, to read and write to. R is the same as X.
	/// </summary>
	/// <returns>A reference to R to modify.</returns>
	inline T& R() { return Vec[0]; }

	/// <summary>
	/// Gets a reference to G, to read and write to. G is the same as Y.
	/// </summary>
	/// <returns>A reference to G to modify.</returns>
	inline T& G() { return Vec[1]; }

	/// <summary>
	/// Gets a reference to B, to read and write to. B is the same as Z.
	/// </summary>
	/// <returns>A reference to B to modify.</returns>
	inline T& B() { return Vec[2]; }

	/// <summary>
	/// Gets a reference to A, to read and write to. A is the same as W.
	/// </summary>
	/// <returns>A reference to A to modify.</returns>
	inline T& A() { return Vec[3]; }
};

#endif // __cplusplus

#endif // !__VECTOR_H__