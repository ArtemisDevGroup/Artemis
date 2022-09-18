#ifndef __VECTOR_OPERATOR_H__
#define __VECTOR_OPERATOR_H__

#include "Vector.h"

#ifdef __cplusplus

template<typename T>
inline Vec2<T> operator+(Vec2<T> vLeft, Vec2<T> vRight) { return Vec2<T>(vLeft.X() + vRight.X(), vLeft.Y() + vRight.Y()); }
template<typename T>
inline Vec3<T> operator+(Vec2<T> vLeft, Vec3<T> vRight) { return Vec3<T>(vLeft.X() + vRight.X(), vLeft.Y() + vRight.Y(), vRight.Z()); }
template<typename T>
inline Vec4<T> operator+(Vec2<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() + vRight.X(), vLeft.Y() + vRight.Y(), vRight.Z(), vRight.W()); }

template<typename T>
inline Vec3<T> operator+(Vec3<T> vLeft, Vec2<T> vRight) { return Vec3<T>(vLeft.X() + vRight.X(), vLeft.Y() + vRight.Y(), vLeft.Z()); }
template<typename T>
inline Vec3<T> operator+(Vec3<T> vLeft, Vec3<T> vRight) { return Vec3<T>(vLeft.X() + vRight.X(), vLeft.Y() + vRight.Y(), vLeft.Z() + vRight.Z()); }
template<typename T>
inline Vec4<T> operator+(Vec3<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() + vRight.X(), vLeft.Y() + vRight.Y(), vLeft.Z() + vRight.Z(), vRight.W()); }

template<typename T>
inline Vec4<T> operator+(Vec4<T> vLeft, Vec2<T> vRight) { return Vec4<T>(vLeft.X() + vRight.X(), vLeft.Y() + vRight.Y(), vLeft.Z(), vLeft.W()); }
template<typename T>
inline Vec4<T> operator+(Vec4<T> vLeft, Vec3<T> vRight) { return Vec4<T>(vLeft.X() + vRight.X(), vLeft.Y() + vRight.Y(), vLeft.Z() + vRight.Z(), vLeft.W()); }
template<typename T>
inline Vec4<T> operator+(Vec4<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() + vRight.X(), vLeft.Y() + vRight.Y(), vLeft.Z() + vRight.Z(), vLeft.W() + vRight.W()); }

template<typename T>
inline Vec2<T> operator-(Vec2<T> vLeft, Vec2<T> vRight) { return Vec2<T>(vLeft.X() - vRight.X(), vLeft.Y() - vRight.Y()); }
template<typename T>
inline Vec3<T> operator-(Vec2<T> vLeft, Vec3<T> vRight) { return Vec3<T>(vLeft.X() - vRight.X(), vLeft.Y() - vRight.Y(), vRight.Z()); }
template<typename T>
inline Vec4<T> operator-(Vec2<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() - vRight.X(), vLeft.Y() - vRight.Y(), vRight.Z(), vRight.W()); }

template<typename T>
inline Vec3<T> operator-(Vec3<T> vLeft, Vec2<T> vRight) { return Vec3<T>(vLeft.X() - vRight.X(), vLeft.Y() - vRight.Y(), vLeft.Z()); }
template<typename T>
inline Vec3<T> operator-(Vec3<T> vLeft, Vec3<T> vRight) { return Vec3<T>(vLeft.X() - vRight.X(), vLeft.Y() - vRight.Y(), vLeft.Z() - vRight.Z()); }
template<typename T>
inline Vec4<T> operator-(Vec3<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() - vRight.X(), vLeft.Y() - vRight.Y(), vLeft.Z() - vRight.Z(), vRight.W()); }

template<typename T>
inline Vec4<T> operator-(Vec4<T> vLeft, Vec2<T> vRight) { return Vec4<T>(vLeft.X() - vRight.X(), vLeft.Y() - vRight.Y(), vLeft.Z(), vLeft.W()); }
template<typename T>
inline Vec4<T> operator-(Vec4<T> vLeft, Vec3<T> vRight) { return Vec4<T>(vLeft.X() - vRight.X(), vLeft.Y() - vRight.Y(), vLeft.Z() - vRight.Z(), vLeft.W()); }
template<typename T>
inline Vec4<T> operator-(Vec4<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() - vRight.X(), vLeft.Y() - vRight.Y(), vLeft.Z() - vRight.Z(), vLeft.W() - vRight.W()); }

template<typename T>
inline Vec2<T> operator*(Vec2<T> vLeft, Vec2<T> vRight) { return Vec2<T>(vLeft.X() * vRight.X(), vLeft.Y() * vRight.Y()); }
template<typename T>
inline Vec3<T> operator*(Vec2<T> vLeft, Vec3<T> vRight) { return Vec3<T>(vLeft.X() * vRight.X(), vLeft.Y() * vRight.Y(), vRight.Z()); }
template<typename T>
inline Vec4<T> operator*(Vec2<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() * vRight.X(), vLeft.Y() * vRight.Y(), vRight.Z(), vRight.W()); }

template<typename T>
inline Vec3<T> operator*(Vec3<T> vLeft, Vec2<T> vRight) { return Vec3<T>(vLeft.X() * vRight.X(), vLeft.Y() * vRight.Y(), vLeft.Z()); }
template<typename T>
inline Vec3<T> operator*(Vec3<T> vLeft, Vec3<T> vRight) { return Vec3<T>(vLeft.X() * vRight.X(), vLeft.Y() * vRight.Y(), vLeft.Z() * vRight.Z()); }
template<typename T>
inline Vec4<T> operator*(Vec3<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() * vRight.X(), vLeft.Y() * vRight.Y(), vLeft.Z() * vRight.Z(), vRight.W()); }

template<typename T>
inline Vec4<T> operator*(Vec4<T> vLeft, Vec2<T> vRight) { return Vec4<T>(vLeft.X() * vRight.X(), vLeft.Y() * vRight.Y(), vLeft.Z(), vLeft.W()); }
template<typename T>
inline Vec4<T> operator*(Vec4<T> vLeft, Vec3<T> vRight) { return Vec4<T>(vLeft.X() * vRight.X(), vLeft.Y() * vRight.Y(), vLeft.Z() * vRight.Z(), vLeft.W()); }
template<typename T>
inline Vec4<T> operator*(Vec4<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() * vRight.X(), vLeft.Y() * vRight.Y(), vLeft.Z() * vRight.Z(), vLeft.W() * vRight.W()); }

template<typename T>
inline Vec2<T> operator/(Vec2<T> vLeft, Vec2<T> vRight) { return Vec2<T>(vLeft.X() / vRight.X(), vLeft.Y() / vRight.Y()); }
template<typename T>
inline Vec3<T> operator/(Vec2<T> vLeft, Vec3<T> vRight) { return Vec3<T>(vLeft.X() / vRight.X(), vLeft.Y() / vRight.Y(), vRight.Z()); }
template<typename T>
inline Vec4<T> operator/(Vec2<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() / vRight.X(), vLeft.Y() / vRight.Y(), vRight.Z(), vRight.W()); }

template<typename T>
inline Vec3<T> operator/(Vec3<T> vLeft, Vec2<T> vRight) { return Vec3<T>(vLeft.X() / vRight.X(), vLeft.Y() / vRight.Y(), vLeft.Z()); }
template<typename T>
inline Vec3<T> operator/(Vec3<T> vLeft, Vec3<T> vRight) { return Vec3<T>(vLeft.X() / vRight.X(), vLeft.Y() / vRight.Y(), vLeft.Z() / vRight.Z()); }
template<typename T>
inline Vec4<T> operator/(Vec3<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() / vRight.X(), vLeft.Y() / vRight.Y(), vLeft.Z() / vRight.Z(), vRight.W()); }

template<typename T>
inline Vec4<T> operator/(Vec4<T> vLeft, Vec2<T> vRight) { return Vec4<T>(vLeft.X() / vRight.X(), vLeft.Y() / vRight.Y(), vLeft.Z(), vLeft.W()); }
template<typename T>
inline Vec4<T> operator/(Vec4<T> vLeft, Vec3<T> vRight) { return Vec4<T>(vLeft.X() / vRight.X(), vLeft.Y() / vRight.Y(), vLeft.Z() / vRight.Z(), vLeft.W()); }
template<typename T>
inline Vec4<T> operator/(Vec4<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() / vRight.X(), vLeft.Y() / vRight.Y(), vLeft.Z() / vRight.Z(), vLeft.W() / vRight.W()); }

template<typename T>
inline Vec2<T> operator&(Vec2<T> vLeft, Vec2<T> vRight) { return Vec2<T>(vLeft.X() & vRight.X(), vLeft.Y() & vRight.Y()); }
template<typename T>
inline Vec3<T> operator&(Vec2<T> vLeft, Vec3<T> vRight) { return Vec3<T>(vLeft.X() & vRight.X(), vLeft.Y() & vRight.Y(), vRight.Z()); }
template<typename T>
inline Vec4<T> operator&(Vec2<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() & vRight.X(), vLeft.Y() & vRight.Y(), vRight.Z(), vRight.W()); }

template<typename T>
inline Vec3<T> operator&(Vec3<T> vLeft, Vec2<T> vRight) { return Vec3<T>(vLeft.X() & vRight.X(), vLeft.Y() & vRight.Y(), vLeft.Z()); }
template<typename T>
inline Vec3<T> operator&(Vec3<T> vLeft, Vec3<T> vRight) { return Vec3<T>(vLeft.X() & vRight.X(), vLeft.Y() & vRight.Y(), vLeft.Z() & vRight.Z()); }
template<typename T>
inline Vec4<T> operator&(Vec3<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() & vRight.X(), vLeft.Y() & vRight.Y(), vLeft.Z() & vRight.Z(), vRight.W()); }

template<typename T>
inline Vec4<T> operator&(Vec4<T> vLeft, Vec2<T> vRight) { return Vec4<T>(vLeft.X() & vRight.X(), vLeft.Y() & vRight.Y(), vLeft.Z(), vLeft.W()); }
template<typename T>
inline Vec4<T> operator&(Vec4<T> vLeft, Vec3<T> vRight) { return Vec4<T>(vLeft.X() & vRight.X(), vLeft.Y() & vRight.Y(), vLeft.Z() & vRight.Z(), vLeft.W()); }
template<typename T>
inline Vec4<T> operator&(Vec4<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() & vRight.X(), vLeft.Y() & vRight.Y(), vLeft.Z() & vRight.Z(), vLeft.W() & vRight.W()); }

template<typename T>
inline Vec2<T> operator|(Vec2<T> vLeft, Vec2<T> vRight) { return Vec2<T>(vLeft.X() | vRight.X(), vLeft.Y() | vRight.Y()); }
template<typename T>
inline Vec3<T> operator|(Vec2<T> vLeft, Vec3<T> vRight) { return Vec3<T>(vLeft.X() | vRight.X(), vLeft.Y() | vRight.Y(), vRight.Z()); }
template<typename T>
inline Vec4<T> operator|(Vec2<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() | vRight.X(), vLeft.Y() | vRight.Y(), vRight.Z(), vRight.W()); }

template<typename T>
inline Vec3<T> operator|(Vec3<T> vLeft, Vec2<T> vRight) { return Vec3<T>(vLeft.X() | vRight.X(), vLeft.Y() | vRight.Y(), vLeft.Z()); }
template<typename T>
inline Vec3<T> operator|(Vec3<T> vLeft, Vec3<T> vRight) { return Vec3<T>(vLeft.X() | vRight.X(), vLeft.Y() | vRight.Y(), vLeft.Z() | vRight.Z()); }
template<typename T>
inline Vec4<T> operator|(Vec3<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() | vRight.X(), vLeft.Y() | vRight.Y(), vLeft.Z() | vRight.Z(), vRight.W()); }

template<typename T>
inline Vec4<T> operator|(Vec4<T> vLeft, Vec2<T> vRight) { return Vec4<T>(vLeft.X() | vRight.X(), vLeft.Y() | vRight.Y(), vLeft.Z(), vLeft.W()); }
template<typename T>
inline Vec4<T> operator|(Vec4<T> vLeft, Vec3<T> vRight) { return Vec4<T>(vLeft.X() | vRight.X(), vLeft.Y() | vRight.Y(), vLeft.Z() | vRight.Z(), vLeft.W()); }
template<typename T>
inline Vec4<T> operator|(Vec4<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() | vRight.X(), vLeft.Y() | vRight.Y(), vLeft.Z() | vRight.Z(), vLeft.W() | vRight.W()); }

template<typename T>
inline Vec2<T> operator^(Vec2<T> vLeft, Vec2<T> vRight) { return Vec2<T>(vLeft.X() ^ vRight.X(), vLeft.Y() ^ vRight.Y()); }
template<typename T>
inline Vec3<T> operator^(Vec2<T> vLeft, Vec3<T> vRight) { return Vec3<T>(vLeft.X() ^ vRight.X(), vLeft.Y() ^ vRight.Y(), vRight.Z()); }
template<typename T>
inline Vec4<T> operator^(Vec2<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() ^ vRight.X(), vLeft.Y() ^ vRight.Y(), vRight.Z(), vRight.W()); }

template<typename T>
inline Vec3<T> operator^(Vec3<T> vLeft, Vec2<T> vRight) { return Vec3<T>(vLeft.X() ^ vRight.X(), vLeft.Y() ^ vRight.Y(), vLeft.Z()); }
template<typename T>
inline Vec3<T> operator^(Vec3<T> vLeft, Vec3<T> vRight) { return Vec3<T>(vLeft.X() ^ vRight.X(), vLeft.Y() ^ vRight.Y(), vLeft.Z() ^ vRight.Z()); }
template<typename T>
inline Vec4<T> operator^(Vec3<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() ^ vRight.X(), vLeft.Y() ^ vRight.Y(), vLeft.Z() ^ vRight.Z(), vRight.W()); }

template<typename T>
inline Vec4<T> operator^(Vec4<T> vLeft, Vec2<T> vRight) { return Vec4<T>(vLeft.X() ^ vRight.X(), vLeft.Y() ^ vRight.Y(), vLeft.Z(), vLeft.W()); }
template<typename T>
inline Vec4<T> operator^(Vec4<T> vLeft, Vec3<T> vRight) { return Vec4<T>(vLeft.X() ^ vRight.X(), vLeft.Y() ^ vRight.Y(), vLeft.Z() ^ vRight.Z(), vLeft.W()); }
template<typename T>
inline Vec4<T> operator^(Vec4<T> vLeft, Vec4<T> vRight) { return Vec4<T>(vLeft.X() ^ vRight.X(), vLeft.Y() ^ vRight.Y(), vLeft.Z() ^ vRight.Z(), vLeft.W() ^ vRight.W()); }

template<typename T>
inline Vec2<T> operator~(Vec2<T> vVec) { return Vec2<T>(~vVec.X(), ~vVec.Y()); }
template<typename T>
inline Vec3<T> operator~(Vec3<T> vVec) { return Vec3<T>(~vVec.X(), ~vVec.Y(), ~vVec.Z()); }
template<typename T>
inline Vec4<T> operator~(Vec4<T> vVec) { return Vec4<T>(~vVec.X(), ~vVec.Y(), ~vVec.Z(), ~vVec.W()); }

#endif // __cplusplus

#endif // !__VECTOR_OPERATOR_H__