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

#ifndef __AURORA_EXCEPTIONS_H__
#define __AURORA_EXCEPTIONS_H__

#include "Definitions.h"

#include <string.h>
#include <Windows.h>
#undef GetMessage

/// <summary>
/// <para>Sets the context of all thrown exceptions to report the current function as the origin of the call.</para>
/// <para>If this function is called inside an already contextualized call, this function will be added to the call trace.</para>
/// </summary>
#define AuroraContextStart() A_DWORD dwKey = Aurora::GlobalExceptionContext::SetContext(__FUNCSIG__)
// Ends the contextualized call. Should be called right before returning.
#define AuroraContextEnd() Aurora::GlobalExceptionContext::ResetContext(dwKey)
// Throws an exception inside of a contextualized call.
#define AuroraThrow(Exception, ...) throw *Exception(__VA_ARGS__).WithContext(__FUNCSIG__, __FILE__, __LINE__)
// Creates a failsafe block that watches out for certain kinds of exceptions. One try block can have multiple catch blocks.
#define AuroraTry try
// Catches an exception as a constant reference. Use the variable 'e' to access the exception.
#define AuroraCatch(exception) catch (const exception& e)

#define MAX_THREADCOUNT 64
#define MAX_CALL_TRACE 64
#define MAX_FUNCTION_SIGNATURE 1024

namespace Aurora {
	/// <summary>
	/// An enumeration containing constants for identifying different types of exceptions.
	/// </summary>
	enum class ExceptionIdentifier : A_DWORD {
		WindowsApi,
		ParameterInvalid,
		Errno,
		IndexOutOfBounds,
		NotImplemented,
		PointerChainInvalid,
		Read32,
		Read64,
		Write32,
		Write64,
		ObjectNotFound,
		Compatibility,
		Resource
	};

	/// <summary>
	/// A static class for globally managing a call stack trace.
	/// </summary>
	class AURORA_API GlobalExceptionContext {
	public:
		GlobalExceptionContext() = delete;
		GlobalExceptionContext(const GlobalExceptionContext&) = delete;

		typedef const A_CHAR(&FunctionsArray)[MAX_CALL_TRACE][MAX_FUNCTION_SIGNATURE];

		/// <summary>
		/// Sets the call context to the provided string.
		/// </summary>
		/// <param name="lpContext">- The context to set.</param>
		/// <returns>A key later used to reset the context.</returns>
		AURORA_NDWR_DISP("SetContext") static A_DWORD SetContext(_In_z_ A_LPCSTR lpContext);

		/// <summary>
		/// Gets the currently set context for the current thread.
		/// </summary>
		/// <returns>A reference to the context array.</returns>
		AURORA_NDWR_GET("GetContext") static FunctionsArray GetContext() noexcept;

		/// <summary>
		/// Gets the number of valid context strings.
		/// </summary>
		/// <returns>The number of contexts.</returns>
		AURORA_NDWR_GET("GetcontextCount") static A_I32 GetContextCount() noexcept;

		/// <summary>
		/// Resets the previously set context.
		/// </summary>
		/// <param name="dwKey">- The context key.</param>
		static A_VOID ResetContext(_In_ A_DWORD dwKey) noexcept;
	};

	/// <summary>
	/// A base class for all exceptions.
	/// </summary>
	class AURORA_API Exception {
		A_CHAR szMessage[MAX_MSG];
		ExceptionIdentifier dwId;

	public:
		/// <summary>
		/// Constructs an Exception.
		/// </summary>
		/// <param name="lpMessage">- The exception message.</param>
		/// <param name="Id">- The exception identifier.</param>
		Exception(_In_z_ A_LPCSTR lpMessage, _In_ const ExceptionIdentifier& refdwId);

		/// <summary>
		/// Gets the message associated with the exception instance.
		/// </summary>
		/// <returns>The exception message.</returns>
		AURORA_NDWR_GET("GetMessage") constexpr _Ret_z_ A_LPCSTR GetMessage() const noexcept;

		/// <summary>
		/// Gets the identifier associated with the exception instance.
		/// </summary>
		/// <returns>The exception identifier.</returns>
		AURORA_NDWR_GET("GetIdentifier") constexpr const ExceptionIdentifier& GetIdentifier() const noexcept;

		A_BOOL operator==(const Exception&) const;
		constexpr A_BOOL operator==(const ExceptionIdentifier&) const noexcept;
		A_BOOL operator==(_In_z_ A_LPCSTR) const;

		A_BOOL operator!=(const Exception&) const;
		constexpr A_BOOL operator!=(const ExceptionIdentifier&) const noexcept;
		A_BOOL operator!=(_In_z_ A_LPCSTR) const;
	};

	/// <summary>
	/// A function template for a next throw event handler.
	/// </summary>
	using ThrowEventHandler = Function<A_VOID(Exception*)>;

	/// <summary>
	/// A static class for managing handlers to be called on the next exception thrown.
	/// </summary>
	class AURORA_API NextThrowEventManager {
	public:
		NextThrowEventManager() = delete;
		NextThrowEventManager(const NextThrowEventManager&) = delete;

		/// <summary>
		/// Queues an event handler to be called on the next exception thrown.
		/// </summary>
		/// <param name="lpfnEventHandler">- A pointer to an event handler.</param>
		/// <param name="bRemoveOnInvoke">- Determines whether the event handler shall remain in the queue or be removed after it is invoked.</param>
		/// <returns>The index of the event handler.</returns>
		static A_I32 QueueEvent(_In_ ThrowEventHandler lpfnEventHandler, _In_ A_BOOL bRemoveOnInvoke = true);

		/// <summary>
		/// Invokes all registered event handlers.
		/// </summary>
		/// <param name="lpException">- The sender.</param>
		static A_VOID Invoke(_In_ Exception* lpException);

		/// <summary>
		/// Clears one or all indexes and resets them to nullptr.
		/// </summary>
		/// <param name="nIndex">- The index of the event handler to clear. If set to INVALID_INDEX (-1) all event handlers will be cleared.</param>
		static A_VOID ClearEventQueue(_In_ A_I32 nIndex = INVALID_INDEX);
	};

	class OnThrowEventManager;

	/// <summary>
	/// A base class for all exceptions. Contains functionality to fetch and manage a context.
	/// </summary>
	/// <typeparam name="Derived">- The derived exception class.</typeparam>
	template<class Derived>
	class ExceptionContext {
		A_CHAR* lpszFunctions[MAX_CALL_TRACE];
		A_CHAR szCoreFunction[MAX_FUNCTION_SIGNATURE];
		A_CHAR szFilePath[MAX_PATH];
		A_I32 nFunctionCount;
		A_I32 nLine;

		A_BOOL bContextSet;

	public:
		inline ExceptionContext() : lpszFunctions(), szCoreFunction(), szFilePath(), nFunctionCount(0), nLine(0), bContextSet(false) {}

		inline ExceptionContext(const ExceptionContext<Derived>& cpy) {
			nFunctionCount = cpy.nFunctionCount;
			for (A_I32 i = 0; i < nFunctionCount; i++) {
				lpszFunctions[i] = new A_CHAR[MAX_FUNCTION_SIGNATURE];
				strcpy_s(lpszFunctions[i], MAX_FUNCTION_SIGNATURE, cpy.lpszFunctions[i]);
			}

			strcpy_s(szCoreFunction, cpy.szCoreFunction);
			strcpy_s(szFilePath, cpy.szFilePath);
			nLine = cpy.nLine;

			bContextSet = cpy.bContextSet;
		}

		inline ~ExceptionContext() {
			for (A_I32 i = 0; i < nFunctionCount; i++)
				delete[] lpszFunctions[i];
		}

		/// <summary>
		/// Sets the context of the exception. This function shall only be called once and only be called through the 'AuroraThrow' macro.
		/// </summary>
		/// <param name="lpFunction">- The signature of the function that failed.</param>
		/// <param name="lpFile">- The name of the file where the function that failed is contained.</param>
		/// <param name="nLine">- The line where the throw happened.</param>
		/// <returns>A pointer to the derived class instance.</returns>
		inline Derived* WithContext(_In_z_ A_LPCSTR lpFunction, _In_z_ A_LPCSTR lpFile, _In_ A_I32 nLine) {
			if (!bContextSet) {
				ZeroMemory(lpszFunctions, sizeof(lpszFunctions));

				nFunctionCount = GlobalExceptionContext::GetContextCount();
				for (A_I32 i = 0; i < nFunctionCount; i++) {
					lpszFunctions[i] = new A_CHAR[MAX_FUNCTION_SIGNATURE];
					strcpy_s(lpszFunctions[i], MAX_FUNCTION_SIGNATURE, GlobalExceptionContext::GetContext()[i]);
				}

				GlobalExceptionContext::ResetContext(GetCurrentThreadId());

				strcpy_s(szCoreFunction, lpFunction);
				strcpy_s(szFilePath, lpFile);
				this->nLine = nLine;

				NextThrowEventManager::Invoke((Derived*)this);
				OnThrowEventManager::Invoke((Derived*)this);

				bContextSet = true;
			}
			return (Derived*)this;
		}

		/// <summary>
		/// Gets a double pointer to the function signatures in the call stack trace.
		/// </summary>
		/// <returns>A double pointer to the call stack trace.</returns>
		AURORA_NDWR_GET("GetFunctions") constexpr A_LPCSTR const* GetFunctions() const noexcept { return lpszFunctions; }

		/// <summary>
		/// Gets the number of function signatures present in the call stack trace.
		/// </summary>
		/// <returns>The number of function signatures.</returns>
		AURORA_NDWR_GET("GetFunctionCount") constexpr A_I32 GetFunctionCount() const noexcept { return nFunctionCount; }

		/// <summary>
		/// Gets the signature of the function where the exception was thrown.
		/// </summary>
		/// <returns>The core function signature.</returns>
		AURORA_NDWR_GET("GetCoreFunction") constexpr _Ret_z_ A_LPCSTR GetCoreFunction() const noexcept { return szCoreFunction; }

		/// <summary>
		/// Gets the name of the file where the exception was thrown.
		/// </summary>
		/// <returns>The file name.</returns>
		AURORA_NDWR_GET("GetFile") constexpr _Ret_z_ A_LPCSTR GetFile() const noexcept { return szFilePath; }

		/// <summary>
		/// Gets the line where the exception was thrown.
		/// </summary>
		/// <returns>The line number.</returns>
		AURORA_NDWR_GET("GetLine") constexpr A_I32 GetLine() const noexcept { return nLine; }
	};

	/// <summary>
	/// An exception types thrown when a Windows API function fails.
	/// </summary>
	class AURORA_API WindowsApiException : public Exception, public ExceptionContext<WindowsApiException> {
		A_DWORD dwWindowsApiCode;
		A_CHAR szWindowsApiMessage[MAX_MSG];
		A_CHAR szWindowsApiFunction[MAX_NAME];

	public:
		/// <summary>
		/// Constructs a WindowsApiException.
		/// </summary>
		/// <param name="lpWindowsApiFunction">- The name of the Windows API function that failed.</param>
		WindowsApiException(_In_z_ A_LPCSTR lpWindowsApiFunction);

		/// <summary>
		/// Gets the Windows API error code returned from 'GetLastError'.
		/// </summary>
		/// <returns>The Windows API error code.</returns>
		AURORA_NDWR_GET("GetWindowsCode") constexpr A_DWORD GetWindowsCode() const noexcept;

		/// <summary>
		/// Gets the Windows API error message returned from 'FormatMessageA'.
		/// </summary>
		/// <returns>The Windows API error message.</returns>
		AURORA_NDWR_GET("GetWindowsMessage") constexpr _Ret_z_ A_LPCSTR GetWindowsMessage() const noexcept;

		/// <summary>
		/// Gets the name of the Windows API function that failed.
		/// </summary>
		/// <returns>The name fo the Windows API function.</returns>
		AURORA_NDWR_GET("GetWindowsFunction") constexpr _Ret_z_ A_LPCSTR GetWindowsFunction() const noexcept;
	};

	/// <summary>
	/// An exception type thrown when parameter validation fails.
	/// </summary>
	class AURORA_API ParameterInvalidException : public Exception, public ExceptionContext<ParameterInvalidException> {
		A_CHAR szParameterName[MAX_NAME];

	public:
		/// <summary>
		/// Constructs a ParameterInvalidException.
		/// </summary>
		/// <param name="lpParameterName">- The name of the parameter causing the throw.</param>
		ParameterInvalidException(_In_z_ A_LPCSTR lpParameterName);

		/// <summary>
		/// Gets the name of the invalid parameter.
		/// </summary>
		/// <returns>The name of the parameter.</returns>
		AURORA_NDWR_GET("GetParameterName") constexpr _Ret_z_ A_LPCSTR GetParameterName() const noexcept;
	};

	/// <summary>
	/// An exception type thrown when a C-standard function fails.
	/// </summary>
	class AURORA_API ErrnoException : public Exception, public ExceptionContext<ErrnoException> {
		errno_t nErrorCode;
		A_CHAR szErrnoString[MAX_MSG];

	public:
		/// <summary>
		/// Constructs an ErrnoException.
		/// </summary>
		/// <param name="nErrorCode">- The errno code.</param>
		ErrnoException(_In_ errno_t nErrorCode);

		/// <summary>
		/// Gets the errno error code.
		/// </summary>
		/// <returns>The error code.</returns>
		AURORA_NDWR_GET("GetErrorCode") constexpr errno_t GetErrorCode() const noexcept;

		/// <summary>
		/// Gets the errno error message.
		/// </summary>
		/// <returns>The error message.</returns>
		AURORA_NDWR_GET("GetErrorMessage") constexpr _Ret_z_ A_LPCSTR GetErrorMessage() const noexcept;
	};

	/// <summary>
	/// An exception type thrown when a feature is not implemented yet but used.
	/// </summary>
	class AURORA_API NotImplementedException : public Exception, public ExceptionContext<NotImplementedException> { public: NotImplementedException(); };

	/// <summary>
	/// An exception type thrown when a pointer chain has an invalid pointer.
	/// </summary>
	class AURORA_API PointerChainInvalidException : public Exception, public ExceptionContext<PointerChainInvalidException> {
		A_I32 nChainLevel;

	public:
		/// <summary>
		/// Constructs a PointerChainInvalidException.
		/// </summary>
		/// <param name="nChainLevel">- The invalid offset level.</param>
		PointerChainInvalidException(_In_ A_I32 nChainLevel);

		/// <summary>
		/// Gets the level at where the pointer chain turned invalid.
		/// </summary>
		/// <returns>The invalid pointer chain level.</returns>
		AURORA_NDWR_GET("GetChainLevel") constexpr A_I32 GetChainLevel() const noexcept;
	};

	namespace Helpers {
		/// <summary>
		/// A base class for all memory related exceptions.
		/// </summary>
		/// <typeparam name="T">- The address width.</typeparam>
		template<AddressType T>
		class MemoryExceptionImpl {
			T uAddress;
			A_DWORD dwSize;

		public:
			/// <summary>
			/// Constructs a MemoryExceptionImpl.
			/// </summary>
			/// <param name="uAddress">- The operation address.</param>
			/// <param name="dwSize">- The operation size.</param>
			MemoryExceptionImpl(_In_ T uAddress, _In_ A_DWORD dwSize) : uAddress(uAddress), dwSize(dwSize) {}

			/// <summary>
			/// Gets the inaccessible address.
			/// </summary>
			/// <returns>The address.</returns>
			AURORA_NDWR_GET("GetAddress") constexpr T GetAddress() const noexcept { return uAddress; }

			/// <summary>
			/// Gets the size of the attempted operation.
			/// </summary>
			/// <returns>The operation size.</returns>
			AURORA_NDWR_GET("GetSize") constexpr A_DWORD GetSize() const noexcept { return dwSize; }
		};
	}

	/// <summary>
	/// An exception type thrown when a read operation fails for a 32-bit process.
	/// </summary>
	class AURORA_API Read32Exception : public Helpers::MemoryExceptionImpl<A_ADDR32>, public Exception, public ExceptionContext<Read32Exception> {
	public:
		/// <summary>
		/// Constructs a Read32Exception.
		/// </summary>
		/// <param name="uAddress">- The operation address.</param>
		/// <param name="dwSize">- The operation size.</param>
		Read32Exception(_In_a32_ A_ADDR32 uAddress, _In_ A_DWORD dwSize);
	};

	/// <summary>
	/// An exception type thrown when a read operation fails for a 64-bit process.
	/// </summary>
	class AURORA_API Read64Exception : public Helpers::MemoryExceptionImpl<A_ADDR64>, public Exception, public ExceptionContext<Read64Exception> {
	public:
		/// <summary>
		/// Constructs a Read64Exception.
		/// </summary>
		/// <param name="uAddress">- The operation address.</param>
		/// <param name="dwSize">- The operation size.</param>
		Read64Exception(_In_a64_ A_ADDR64 uAddress, _In_ A_DWORD dwSize);
	};

	/// <summary>
	/// An exception type thrown when a write operation fails for a 32-bit process.
	/// </summary>
	class AURORA_API Write32Exception : public Helpers::MemoryExceptionImpl<A_ADDR32>, public Exception, public ExceptionContext<Write32Exception> {
	public:
		/// <summary>
		/// Constructs a Write32Exception.
		/// </summary>
		/// <param name="uAddress">- The operation address.</param>
		/// <param name="dwSize">- The operation size.</param>
		Write32Exception(_In_a32_ A_ADDR32 uAddress, _In_ A_DWORD dwSize);
	};

	/// <summary>
	/// An exception type throwm when a write operation fails for a 64-bit process.
	/// </summary>
	class AURORA_API Write64Exception : public Helpers::MemoryExceptionImpl<A_ADDR64>, public Exception, public ExceptionContext<Write64Exception> {
	public:
		/// <summary>
		/// Constructs a Write64Exception.
		/// </summary>
		/// <param name="uAddress">- The operation address.</param>
		/// <param name="dwSize">- The operation size.</param>
		Write64Exception(_In_a64_ A_ADDR64 uAddress, _In_ A_DWORD dwSize);
	};

#ifdef _WIN64
	/// <summary>
	/// An exception type thrown when a read operation fails for a 64-bit process.
	/// </summary>
	using ReadException = Read64Exception;

	/// <summary>
	/// An exception type throwm when a write operation fails for a 64-bit process.
	/// </summary>
	using WriteException = Write64Exception;
#else
	/// <summary>
	/// An exception type thrown when a read operation fails for a 32-bit process.
	/// </summary>
	using ReadException = Read32Exception;

	/// <summary>
	/// An exception type throwm when a write operation fails for a 32-bit process.
	/// </summary>
	using WriteException = Write32Exception;
#endif

	/// <summary>
	/// An exception type thrown when an attempt has been made to fetch an object but it has not been found.
	/// </summary>
	class AURORA_API ObjectNotFoundException : public Exception, public ExceptionContext<ObjectNotFoundException> {
		A_CHAR szObjectType[MAX_NAME];
		A_CHAR szObjectName[MAX_NAME];

	public:
		/// <summary>
		/// Constructs an ObjectNotFoundException.
		/// </summary>
		/// <param name="lpObjectType">- The object type.</param>
		/// <param name="lpObjectName">- The object name.</param>
		ObjectNotFoundException(_In_z_ A_LPCSTR lpObjectType, _In_z_ A_LPCSTR lpObjectName);

		/// <summary>
		/// Gets the type of the object.
		/// </summary>
		/// <returns>The object type.</returns>
		AURORA_NDWR_GET("GetObjectType") constexpr _Ret_z_ A_LPCSTR GetObjectType() const noexcept;

		/// <summary>
		/// Gets the name of the object.
		/// </summary>
		/// <returns>The object name.</returns>
		AURORA_NDWR_GET("GetObjectName") constexpr _Ret_z_ A_LPCSTR GetObjectName() const noexcept;
	};

	/// <summary>
	/// An exception type thrown when either two objects or two parameters are incompatible with each other.
	/// </summary>
	class AURORA_API CompatibilityException : public Exception, public ExceptionContext<CompatibilityException> { public: CompatibilityException(); };

	/// <summary>
	/// Thrown when there are insufficient resources to complete an operation.
	/// </summary>
	class AURORA_API ResourceException : public Exception, public ExceptionContext<ResourceException> { public: ResourceException(); };

	/// <summary>
	/// Represents an index for an event handler.
	/// </summary>
	/// <typeparam name="T">- The exception type associated with the event handler.</typeparam>
	template<class ExceptionClass>
	struct EventHandlerIndex {
	private:
		A_I32 nIndex;

	public:
		/// <summary>
		/// Constructs an EventHandlerIndex.
		/// </summary>
		/// <param name="nIndex">- The index of the event handler.</param>
		constexpr EventHandlerIndex(_In_ A_I32 nIndex) noexcept : nIndex(nIndex) {}

		constexpr operator A_I32& () noexcept { return nIndex; }
		constexpr operator const A_I32& () const noexcept { return nIndex; }
	};

	/// <summary>
	/// Is only valid as an argument to 'ClearEventQueue()'. Clears all registered event handlers instead.
	/// </summary>
	template<class ExceptionClass>
	constexpr EventHandlerIndex<ExceptionClass> All = { -1 };

	namespace Helpers {
		AURORA_NDWR_GET("GetIndexOf") AURORA_API A_I32 GetIndexOf(_In_ A_DWORD dwThreadId);

		/// <summary>
		/// The implementation for an exception event manager.
		/// </summary>
		/// <typeparam name="ExceptionClass">- The exception type to implement an exception event manager for.</typeparam>
		template<class ExceptionClass>
		class AURORA_API OnThrowEventManagerImpl {
			static Function<A_VOID(const ExceptionClass*)> lpszfnOnThrowEventHandlers[MAX_THREADCOUNT][MAX_INVOKE];
			static A_BOOL szbRemoveOnInvoke[MAX_THREADCOUNT][MAX_INVOKE];

		public:
			OnThrowEventManagerImpl() = delete;
			OnThrowEventManagerImpl(const OnThrowEventManagerImpl&) = delete;

			/// <summary>
			/// Queues an event handler to be called on the next exception of the provided type thrown.
			/// </summary>
			/// <param name="lpfnEventHandler">- A pointer to an event handler.</param>
			/// <param name="bRemoveOnInvoke">- Determines whether the event handler shall remain in the queue or be removed after it is invoked.</param>
			/// <returns>The index of the event handler.</returns>
			static inline EventHandlerIndex<ExceptionClass> QueueEvent(_In_ Function<A_VOID(const ExceptionClass*)> lpfnEventHandler, _In_ A_BOOL bRemoveOnInvoke = true) {
				A_I32 nIndex = GetIndexOf(GetCurrentThreadId());
				if (nIndex != INVALID_INDEX)
					for (A_I32 i = 0; i < MAX_INVOKE; i++)
						if (!lpszfnOnThrowEventHandlers[nIndex][i].target<void>()) {
							lpszfnOnThrowEventHandlers[nIndex][i] = lpfnEventHandler;
							return i;
						}
				return INVALID_INDEX;
			}

			/// <summary>
			/// Clears one or all indexes and resets them to nullptr.
			/// </summary>
			/// <param name="nIndex">- The index of the event handler to clear. If set to 'All' (-1) all event handlers will be cleared.</param>
			static inline A_VOID ClearEventQueue(_In_ EventHandlerIndex<ExceptionClass> nIndex) {
				A_I32 nLevel1Index = GetIndexOf(GetCurrentThreadId());
				if (nLevel1Index != INVALID_INDEX) {
					if (nIndex == INVALID_INDEX)
						for (A_I32 i = 0; i < MAX_INVOKE; i++)
							lpszfnOnThrowEventHandlers[nLevel1Index][i] = nullptr;
					else lpszfnOnThrowEventHandlers[nLevel1Index][nIndex] = nullptr;
				}
			}

			/// <summary>
			/// Invokes all registered event handlers.
			/// </summary>
			/// <param name="lpException">- The sender.</param>
			static inline A_VOID Invoke(_In_ const ExceptionClass* lpException) {
				A_I32 nIndex = GetIndexOf(GetCurrentThreadId());
				if (nIndex != INVALID_INDEX)
					for (A_I32 i = MAX_INVOKE - 1; i >= 0; i--)
						if (lpszfnOnThrowEventHandlers[nIndex][i].target<void>()) {
							lpszfnOnThrowEventHandlers[nIndex][i](lpException);
							if (szbRemoveOnInvoke[nIndex][i])
								lpszfnOnThrowEventHandlers[nIndex][i] = nullptr;
						}
			}
		};
	}

	/// <summary>
	/// A static class for managing handlers to be called on the next exception of a particular type thrown.
	/// </summary>
	class OnThrowEventManager :
		public Helpers::OnThrowEventManagerImpl<WindowsApiException>,
		public Helpers::OnThrowEventManagerImpl<ParameterInvalidException>,
		public Helpers::OnThrowEventManagerImpl<ErrnoException>,
		public Helpers::OnThrowEventManagerImpl<NotImplementedException>,
		public Helpers::OnThrowEventManagerImpl<PointerChainInvalidException>,
		public Helpers::OnThrowEventManagerImpl<Read32Exception>,
		public Helpers::OnThrowEventManagerImpl<Read64Exception>,
		public Helpers::OnThrowEventManagerImpl<Write32Exception>,
		public Helpers::OnThrowEventManagerImpl<Write64Exception>,
		public Helpers::OnThrowEventManagerImpl<ObjectNotFoundException>,
		public Helpers::OnThrowEventManagerImpl<CompatibilityException>,
		public Helpers::OnThrowEventManagerImpl<ResourceException>
	{
	public:
		OnThrowEventManager() = delete;
		OnThrowEventManager(const OnThrowEventManager&) = delete;

		using OnThrowEventManagerImpl<WindowsApiException>			::QueueEvent;
		using OnThrowEventManagerImpl<ParameterInvalidException>	::QueueEvent;
		using OnThrowEventManagerImpl<ErrnoException>				::QueueEvent;
		using OnThrowEventManagerImpl<NotImplementedException>		::QueueEvent;
		using OnThrowEventManagerImpl<PointerChainInvalidException>	::QueueEvent;
		using OnThrowEventManagerImpl<Read32Exception>				::QueueEvent;
		using OnThrowEventManagerImpl<Read64Exception>				::QueueEvent;
		using OnThrowEventManagerImpl<Write32Exception>				::QueueEvent;
		using OnThrowEventManagerImpl<Write64Exception>				::QueueEvent;
		using OnThrowEventManagerImpl<ObjectNotFoundException>		::QueueEvent;
		using OnThrowEventManagerImpl<CompatibilityException>		::QueueEvent;
		using OnThrowEventManagerImpl<ResourceException>			::QueueEvent;

		using OnThrowEventManagerImpl<WindowsApiException>			::ClearEventQueue;
		using OnThrowEventManagerImpl<ParameterInvalidException>	::ClearEventQueue;
		using OnThrowEventManagerImpl<ErrnoException>				::ClearEventQueue;
		using OnThrowEventManagerImpl<NotImplementedException>		::ClearEventQueue;
		using OnThrowEventManagerImpl<PointerChainInvalidException>	::ClearEventQueue;
		using OnThrowEventManagerImpl<Read32Exception>				::ClearEventQueue;
		using OnThrowEventManagerImpl<Read64Exception>				::ClearEventQueue;
		using OnThrowEventManagerImpl<Write32Exception>				::ClearEventQueue;
		using OnThrowEventManagerImpl<Write64Exception>				::ClearEventQueue;
		using OnThrowEventManagerImpl<ObjectNotFoundException>		::ClearEventQueue;
		using OnThrowEventManagerImpl<CompatibilityException>		::ClearEventQueue;
		using OnThrowEventManagerImpl<ResourceException>			::ClearEventQueue;

		using OnThrowEventManagerImpl<WindowsApiException>			::Invoke;
		using OnThrowEventManagerImpl<ParameterInvalidException>	::Invoke;
		using OnThrowEventManagerImpl<ErrnoException>				::Invoke;
		using OnThrowEventManagerImpl<NotImplementedException>		::Invoke;
		using OnThrowEventManagerImpl<PointerChainInvalidException>	::Invoke;
		using OnThrowEventManagerImpl<Read32Exception>				::Invoke;
		using OnThrowEventManagerImpl<Read64Exception>				::Invoke;
		using OnThrowEventManagerImpl<Write32Exception>				::Invoke;
		using OnThrowEventManagerImpl<Write64Exception>				::Invoke;
		using OnThrowEventManagerImpl<ObjectNotFoundException>		::Invoke;
		using OnThrowEventManagerImpl<CompatibilityException>		::Invoke;
		using OnThrowEventManagerImpl<ResourceException>			::Invoke;
	};
}

#endif // !__AURORA_EXCEPTIONS_H__