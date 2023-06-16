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

#ifndef __AURORA_TIME_H__
#define __AURORA_TIME_H__

#include "Definitions.h"
#include "Exceptions.h"

namespace Aurora {
    /// <summary>
    /// Represents a moment in time.
    /// </summary>
    struct AURORA_API Time {
        A_WORD wYear;               // The year.
        A_WORD wMonth;              // An integral representation of a month ranging from 1 to 12 depending on the month.
        A_CHAR szMonthString[16];   // A string representation of a month.
        A_WORD wDayOfWeek;          // An integral representation of the day ranging from 1 to 7 depending on the weekday.
        A_CHAR szDayOfWeekString[16]; // A string representation of the weekday.
        A_WORD wDay;                // The day of the month.
        A_WORD wHour;               // The hour.
        A_WORD wMinute;             // The minute.
        A_WORD wSecond;             // The second.
        A_WORD wMilliseconds;       // The millisecond.

        /// <summary>
        /// Gets the current time dependent on the time zone of the local computer.
        /// </summary>
        /// <returns>The current local time.</returns>
        AURORA_NDWR_GET("GetLocal") static Time GetLocal() noexcept;

        /// <summary>
        /// Gets the system time (UTC).
        /// </summary>
        /// <returns>The current system time.</returns>
        AURORA_NDWR_GET("GetSystem") static Time GetSystem() noexcept;

        /// <summary>
        /// <para>Creates a string representation of the time depending on the format provided.</para>
        /// <para>The following format specifiers are valid:</para>
        /// <para>"%y" - The year.</para>
        /// <para>"%mo" / "%mz" - The month / its string representation.</para>
        /// <para>"%w" / "%wz" - The day of week / its string representation.</para>
        /// <para>"%d" - The day.</para>
        /// <para>"%h" - The hour.</para>
        /// <para>"%m" - The minute.</para>
        /// <para>"%s" - The second.</para>
        /// <para>"%ms" - The millisecond.</para>
        /// </summary>
        /// <param name="lpBuffer">- A pointer to a buffer to receive the formatted time.</param>
        /// <param name="dwSize">- The size of the buffer in elements.</param>
        /// <param name="lpFormat">- The format.</param>
        A_VOID FormatString(_Out_writes_z_(dwSize) A_LPSTR lpBuffer, _In_ A_DWORD dwSize, _In_z_ A_LPCSTR lpFormat) const;
        
        /// <summary>
        /// <para>Creates a string representation of the time depending on the format provided.</para>
        /// <para>The following format specifiers are valid:</para>
        /// <para>"%y" - The year.</para>
        /// <para>"%mo" / "%mz" - The month / its string representation.</para>
        /// <para>"%w" / "%wz" - The day of week / its string representation.</para>
        /// <para>"%d" - The day.</para>
        /// <para>"%h" - The hour.</para>
        /// <para>"%m" - The minute.</para>
        /// <para>"%s" - The second.</para>
        /// <para>"%ms" - The millisecond.</para>
        /// </summary>
        /// <typeparam name="nSize">- The size of the buffer in elements.</typeparam>
        /// <param name="lpBuffer">- A reference to a buffer to receive the formatted time.</param>
        /// <param name="lpFormat">- The format.</param>
        template<A_I32 nSize>
        A_VOID FormatString(_Out_writes_z_(nSize) A_CHAR(&lpBuffer)[nSize], _In_z_ A_LPCSTR lpFormat) const {
            FormatString(lpBuffer, nSize, lpFormat);
        }
    };
}

#endif // !__AURORA_TIME_H__