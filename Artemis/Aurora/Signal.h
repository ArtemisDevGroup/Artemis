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

#ifndef __AURORA_SIGNAL_H__
#define __AURORA_SIGNAL_H__

#include "Definitions.h"
#include "Exceptions.h"
#include "Events.h"

namespace Aurora {
	/// <summary>
	/// Represents a process signal.
	/// </summary>
	enum class Signal : A_I32 {
		Abort = 22,				// Abnormal termination.
		Float = 8,				// Floating-point error.
		SegmentViolation = 11,	// Illegal storage access.
		All = 0					// All handlers.
	};

	/// <summary>
	/// The event arguments for a signal event.
	/// </summary>
	class SignalEventArgs {
	public:
		Signal nSignal; // The signal handler that invoked the event.
	};

	/// <summary>
	/// Enables a library defined signal handler for the provided signal.
	/// </summary>
	/// <param name="nSignal">- The signal handler to enable.</param>
	/// <exception cref="ErrnoException"/>
	AURORA_API A_VOID EnableSignalHandler(_In_ Signal nSignal);

	/// <summary>
	/// Disables a library defined signal handler for the provided signal.
	/// </summary>
	/// <param name="nSignal">- The signal handler to disable.</param>
	/// <exception cref="ErrnoException"/>
	AURORA_API A_VOID DisableSignalHandler(_In_ Signal nSignal);

	/// <summary>
	/// Gets the event of a signal handler.
	/// </summary>
	/// <param name="nSignal">- The signal handler event to get.</param>
	/// <returns>The event of the specified signal handler.</returns>
	AURORA_NDWR_GET("GetSignalEvent") AURORA_API Event<SignalEventArgs>& GetSignalEvent(_In_ Signal nSignal) noexcept;
}

#endif // !__AURORA_SIGNAL_H__