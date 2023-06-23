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

#ifndef __AURORA_EVENTS_H__
#define __AURORA_EVENTS_H__

#include "Definitions.h"

namespace Aurora {
	template<typename T, typename... U>
	A_ADDR GetAddressOfFunction(Function<T(U...)> lpfnFunction) {
		typedef T(fnType)(U...);
		fnType** fnPointer = lpfnFunction.template target<fnType*>();
		return (A_ADDR)*fnPointer;
	}

	/// <summary>
	/// A class for subscribing and invoking to events.
	/// </summary>
	/// <typeparam name="InstanceEventArgs">- A class or struct containing event arguments.</typeparam>
	template<ClassType InstanceEventArgs = NullClass_t>
	class Event {
		Function<A_VOID(_In_opt_ A_LPVOID lpSender, _In_opt_ const InstanceEventArgs* lpEventArgs)> lpszfnEventHandlers[MAX_INVOKE];

	public:
		Event() noexcept {
			for (A_I32 i = 0; i < MAX_INVOKE; i++) lpszfnEventHandlers[i] = nullptr;
		}

		/// <summary>
		/// Invokes all registered event handlers.
		/// </summary>
		/// <param name="lpSender">- A pointer to the sender. Can be null.</param>
		/// <param name="lpArgs">- A pointer to an instance of the event args.</param>
		A_VOID Invoke(_In_opt_ A_LPVOID lpSender, _In_opt_ const InstanceEventArgs* lpArgs) const {
			for (A_I32 i = 0; i < MAX_INVOKE; i++)
				if (lpszfnEventHandlers[i] != nullptr)
					lpszfnEventHandlers[i](lpSender, lpArgs);
		}

		/// <summary>
		/// Clears the list of registered event handlers.
		/// </summary>
		A_VOID Clear() noexcept {
			for (A_I32 i = 0; i < MAX_INVOKE; i++) lpszfnEventHandlers[i] = nullptr;
		}

		A_VOID operator+=(_In_ Function<A_VOID(_In_opt_ A_LPVOID lpSender, _In_opt_ const InstanceEventArgs* lpEventArgs)> lpfnEventHandler) noexcept {
			for (A_I32 i = 0; i < MAX_INVOKE; i++)
				if (lpszfnEventHandlers[i] == nullptr) {
					lpszfnEventHandlers[i] = lpfnEventHandler;
					break;
				}
		}

		A_VOID operator-=(_In_ Function<A_VOID(_In_opt_ A_LPVOID lpSender, _In_opt_ const InstanceEventArgs* lpEventArgs)> lpfnEventHandler) noexcept {
			for (A_I32 i = 0; i < MAX_INVOKE; i++)
				if (GetAddressOfFunction(lpszfnEventHandlers[i]) == GetAddressOfFunction(lpfnEventHandler))
					lpszfnEventHandlers[i] = nullptr;
		}
	};

	/// <summary>
	/// A class for subscribing and invoking to events.
	/// </summary>
	template<>
	class Event<NullClass_t> {
		Function<A_VOID(A_LPVOID lpSender)> lpszfnEventHandlers[MAX_INVOKE];

	public:
		Event() noexcept {
			for (A_I32 i = 0; i < MAX_INVOKE; i++) lpszfnEventHandlers[i] = nullptr;
		}

		/// <summary>
		/// Invokes all registered event handlers.
		/// </summary>
		/// <param name="lpSender">- A pointer to the sender. Can be null.</param>
		A_VOID Invoke(_In_opt_ A_LPVOID lpSender) const {
			for (A_I32 i = 0; i < MAX_INVOKE; i++)
				if (lpszfnEventHandlers[i] != nullptr)
					lpszfnEventHandlers[i](lpSender);
		}

		/// <summary>
		/// Clears the list of registered event handlers.
		/// </summary>
		A_VOID Clear() noexcept {
			for (A_I32 i = 0; i < MAX_INVOKE; i++) lpszfnEventHandlers[i] = nullptr;
		}

		A_VOID operator+=(_In_ Function<A_VOID(_In_opt_ A_LPVOID lpSender)> lpfnEventHandler) noexcept {
			for (A_I32 i = 0; i < MAX_INVOKE; i++)
				if (lpszfnEventHandlers[i] == nullptr) {
					lpszfnEventHandlers[i] = lpfnEventHandler;
					break;
				}
		}

		A_VOID operator-=(_In_ Function<A_VOID(_In_opt_ A_LPVOID lpSender)> lpfnEventHandler) noexcept {
			for (A_I32 i = 0; i < MAX_INVOKE; i++)
				if (GetAddressOfFunction(lpszfnEventHandlers[i]) == GetAddressOfFunction(lpfnEventHandler))
					lpszfnEventHandlers[i] = nullptr;
		}
	};
}

#endif // !__AURORA_EVENTS_H__