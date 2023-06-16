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

#ifndef __AURORA_BINARY_H__
#define __AURORA_BINARY_H__

#include "Definitions.h"

namespace Aurora {
	/// <summary>
	/// A class for doing binary operations on a value.
	/// </summary>
	/// <typeparam name="BufferType">- An integral type with the buffer size desired.</typeparam>
	template<BitwiseType BufferType>
	class Binary {
		BufferType Buffer;

	public:
		// The size of the buffer in bytes.
		static constexpr A_I32 BufferByteCount = sizeof(BufferType);

		// The size of the buffer in bits.
		static constexpr A_I32 BufferBitCount = sizeof(BufferType) * 8;

		constexpr Binary() noexcept : Buffer(0) {}

		/// <summary></summary>
		/// <param name="Buffer">- The value to do binary operations on.</param>
		constexpr Binary(_In_ BufferType Buffer) noexcept : Buffer(Buffer) {}

		/// <summary>
		/// Checks if a bit is set.
		/// </summary>
		/// <param name="nBit">- The index of the bit.</param>
		/// <returns>True if the bit is set, false if it's not.</returns>
		AURORA_NDWR_PURE("CheckBit") constexpr A_BOOL CheckBit(_In_range_(0, BufferBitCount) A_I32 nBit) const noexcept { return !!(Buffer & (1 << nBit)); }

		/// <summary>
		/// Sets a bit.
		/// </summary>
		/// <param name="nBit">- The index of the bit.</param>
		constexpr A_VOID SetBit(_In_range_(0, BufferBitCount) A_I32 nBit) noexcept { Buffer = Buffer | (1 << nBit); }

		/// <summary>
		/// Resets or unsets a bit.
		/// </summary>
		/// <param name="nBit">- The index of the bit.</param>
		constexpr A_VOID ResetBit(_In_range_(0, BufferBitCount) A_I32 nBit) noexcept { Buffer = Buffer ^ (1 << nBit); }

		/// <summary>
		/// Rotates all bits left.
		/// </summary>
		/// <param name="uShift">- The number of bits to shift.</param>
		/// <returns>A Binary instance with the rotated bits.</returns>
		AURORA_NDWR_PURE("Rotl") constexpr Binary Rotl(A_U32 uShift) const noexcept { return Binary(Buffer << uShift | Buffer >> (BufferBitCount - uShift)); }

		/// <summary>
		/// Rotates all bits right.
		/// </summary>
		/// <param name="uShift">- The number of bits to shift.</param>
		/// <returns>A Binary instance with the rotated bits.</returns>
		AURORA_NDWR_PURE("Rotr") constexpr Binary Rotr(A_U32 uShift) const noexcept { return Binary(Buffer >> uShift | Buffer << (BufferBitCount - uShift)); }

		/// <summary>
		/// Gets the value of the buffer.
		/// </summary>
		/// <returns>The value of the buffer.</returns>
		AURORA_NDWR_GET("GetBuffer") constexpr BufferType GetBuffer() const noexcept { return Buffer; }

		/// <summary>
		/// Gets a string representation of the buffer in base 2.
		/// </summary>
		/// <param name="lpBuffer">- A reference to the buffer to receive the string.</param>
		A_VOID GetBinaryString(_Out_writes_z_(BufferBitCount + 1) A_CHAR(&lpBuffer)[BufferBitCount + 1]) const {
			for (A_I32 i = 0; i < BufferBitCount; i++)
				lpBuffer[i] = CheckBit(i) ? '1' : '0';
			lpBuffer[BufferBitCount] = '\0';
		}

		AURORA_NDWR_PURE("operator+") constexpr Binary operator+(_In_ const Binary& right) const noexcept { return Binary(Buffer + right.Buffer); }
		AURORA_NDWR_PURE("operator-") constexpr Binary operator-(_In_ const Binary& right) const noexcept { return Binary(Buffer - right.Buffer); }
		AURORA_NDWR_PURE("operator*") constexpr Binary operator*(_In_ const Binary& right) const noexcept { return Binary(Buffer * right.Buffer); }
		AURORA_NDWR_PURE("operator/") constexpr Binary operator/(_In_ const Binary& right) const noexcept { return Binary(Buffer / right.Buffer); }
		AURORA_NDWR_PURE("operator%") constexpr Binary operator%(_In_ const Binary& right) const noexcept { return Binary(Buffer % right.Buffer); }

		AURORA_NDWR_PURE("operator&") constexpr Binary operator&(_In_ const Binary& right) const noexcept { return Binary(Buffer & right.Buffer); }
		AURORA_NDWR_PURE("operator|") constexpr Binary operator|(_In_ const Binary& right) const noexcept { return Binary(Buffer | right.Buffer); }
		AURORA_NDWR_PURE("operator^") constexpr Binary operator^(_In_ const Binary& right) const noexcept { return Binary(Buffer ^ right.Buffer); }

		AURORA_NDWR_PURE("operator<<") constexpr Binary operator<<(_In_ A_I32 nShiftCount) const noexcept { return Binary(Buffer << nShiftCount); }
		AURORA_NDWR_PURE("operator>>") constexpr Binary operator>>(_In_ A_I32 nShiftCount) const noexcept { return Binary(Buffer >> nShiftCount); }

		constexpr A_VOID operator+=(_In_ const Binary& right) noexcept { Buffer += right.Buffer; }
		constexpr A_VOID operator-=(_In_ const Binary& right) noexcept { Buffer -= right.Buffer; }
		constexpr A_VOID operator*=(_In_ const Binary& right) noexcept { Buffer *= right.Buffer; }
		constexpr A_VOID operator/=(_In_ const Binary& right) noexcept { Buffer /= right.Buffer; }
		constexpr A_VOID operator%=(_In_ const Binary& right) noexcept { Buffer %= right.Buffer; }

		constexpr A_VOID operator&=(_In_ const Binary& right) noexcept { Buffer &= right.Buffer; }
		constexpr A_VOID operator|=(_In_ const Binary& right) noexcept { Buffer |= right.Buffer; }
		constexpr A_VOID operator^=(_In_ const Binary& right) noexcept { Buffer ^= right.Buffer; }

		constexpr A_VOID operator~() const noexcept { ~Buffer; }
	};
}

#endif // !__AURORA_BINARY_H__