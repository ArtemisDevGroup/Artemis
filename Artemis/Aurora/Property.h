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

#ifndef __AURORA_PROPERTY_H__
#define __AURORA_PROPERTY_H__

#include "Definitions.h"

namespace Aurora {
	/// <summary>
	/// A class for setting custom get and set behaviour for a data field.
	/// </summary>
	/// <typeparam name="T">- The type of the variable.</typeparam>
	template<NonVoidType T>
	class Property {
		Function<const T& (const T&)> Get;
		Function<void(T&, const T&)> Set;

		T value;

	public:
		/// <summary>
		/// Sets the get and set behaviour of the property.
		/// </summary>
		/// <param name="GetFunction">- A lambda or function pointer defining the get behaviour of the property.</param>
		/// <param name="SetFunction">- A lambda or function pointer defining the set behaviour of the property.</param>
		Property(
			_In_ Function<const T& (const T&)> GetFunction,
			_In_ Function<void(T&, const T&)> SetFunction
		) : value(T()), Get(GetFunction), Set(SetFunction) {}

		/// <summary>
		/// Calls the get routine and returns the value.
		/// </summary>
		/// <returns>The value of the property.</returns>
		AURORA_NDWR_GET("GetValue") const T& GetValue() const { return Get(this->value); }

		/// <summary>
		/// Calls the set routine.
		/// </summary>
		/// <param name="refValue">- The value to set.</param>
		void SetValue(const T& refValue) { Set(this->value, refValue); }

		void operator=(const T& refValue) { SetValue(refValue); }
		operator const T& () const { return GetValue(); }
	};

	/// <summary>
	/// A class for setting custom get and set behaviour for an external variable.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<NonVoidType T>
	class FullProperty {
		Function<const T& ()> Get;
		Function<void(const T&)> Set;

	public:
		/// <summary>
		/// Sets the get and set behaviour of the property.
		/// </summary>
		/// <param name="GetFunction">- A lambda or function pointer defining the get behaviour of the property.</param>
		/// <param name="SetFunction">- A lambda or function pointer defining the set behaviour of the property.</param>
		FullProperty(
			_In_ Function<const T& ()> GetFunction,
			_In_ Function<void(const T&)> SetFunction
		) : Get(GetFunction), Set(SetFunction) {}

		/// <summary>
		/// Calls the get routine and returns the value.
		/// </summary>
		/// <returns>The value of the property.</returns>
		AURORA_NDWR_GET("GetValue") const T& GetValue() const { return Get(); }

		/// <summary>
		/// Calls the set routine.
		/// </summary>
		/// <param name="refValue">- The value to set.</param>
		void SetValue(const T& refValue) { Set(refValue); }

		void operator=(const T& refValue) { SetValue(refValue); }
		operator const T& () const { return GetValue(); }
	};
}

#endif // !__AURORA_PROPERTY_H__