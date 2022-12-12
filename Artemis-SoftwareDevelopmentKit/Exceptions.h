//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include "Definitions.h"
#include "MemoryDefinitions.h"
#include "Events.h"

namespace Artemis {

	/// <summary>
	/// The different exception codes.
	/// </summary>
	enum class ExceptionCode {
		Unknown = -1,				// Used to indicate an unknown or unspecified error that doesn't fit under a specified exception type.
		Success = 0,				// Used to indicate success.
		Parameter,					// Used to indicate that a parameter contains an invalid value.
		InstanceInvalid,			// Used to indicate that a class instance is invalid and has not been constructed properly.
		MemoryAccessViolation,		// Used to indicate that the function has attempted to access memory that is either invalid or protected.
		WindowsApi,					// Used to indicate that a subsequent call to the Windows API has failed.
		ObjectNotFound,				// Used to indicate that an object searched for could not be located.
		NotImplemented,				// Used to indicate that the program has attempted to use a feature that has not yet been implemented.
		Compatibility,				// Used to indicate that two objects are incompatible with each other, or that an object has attributes that are not compatible with the current operation.
		Attribute,					// Used to indicate that an object has incompatible attributes.
		IndexOutOfRange,			// Used to indicate that an index that was out of range has been specified.

		AlreadyInitialized,			// Hooking library is already initialized.
		NotInitialized,				// Hooking library is not initialized.
		AlreadyCreated,				// Hook is already created.
		NotCreated,					// Hook is not created.
		AlreadyEnabled,				// Hook is already enabled.
		NotEnabled,					// Hook is not enabled.
		NotExecutable,				// Function pointer points to invalid and/or non-executable memory.
		UnsupportedFunction,		// The target function type is unsupported.
		MemoryAlloc,				// Failed to allocate memory.
		MemoryProtect,				// Failed to change memory protection.
		ModuleNotFound,				// Failed to locate the specified module.
		FunctionNotFound			// Failed to locate the specified function.
	};

	//------------------------
	// Rules for context:
	// 1. Set context in the beginning of the call.
	// 2. ALWAYS check if there is a previously set context.
	// 3. ALWAYS reset the context at the end of the function, but ONLY if the context is set in that same function.
	//------------------------

	/// <summary>
	/// A static class for managing the context of thrown exceptions.
	/// </summary>
	class ARTEMIS_API ExceptionContext {
		inline ExceptionContext() {}
		inline ExceptionContext(const ExceptionContext&) {}

	public:
		/// <summary>
		/// Sets the call context. This means that the function listed will be the context function and not the subsequently called function.
		/// </summary>
		/// <param name="lpSender">- The sender function.</param>
		static void SetContext(_In_ LPCSTR lpSender);

		/// <summary>
		/// Resets the context back to null.
		/// </summary>
		static void ResetContext();

		/// <summary>
		/// Gets a bool that indicates wether a context has already been set or not.
		/// </summary>
		/// <returns>TRUE if context is set, FALSE otherwise.</returns>
		static _Check_return_ BOOL HasContext();
	};

	// Initializes the exception context.
#define CONTEXT_BEGIN	BOOL b = Artemis::ExceptionContext::HasContext(); \
						if (!b) Artemis::ExceptionContext::SetContext(__FUNCTION__)
	// Resets the exception context. Has to be run after the throw but before the return.
#define CONTEXT_END		if (!b) Artemis::ExceptionContext::ResetContext()

	/// <summary>
	/// A generic Exception class. All other exception types inherit from this class.
	/// </summary>
	class ARTEMIS_API Exception {
		ExceptionCode ec;
		CHAR szMessage[MAX_MESSAGE];
		CHAR szFunction[MAX_NAME];

	public:
		/// <summary>
		/// Constructs an exception.
		/// </summary>
		/// <param name="lpMessage">- The exception message associated with this exception.</param>
		/// <param name="ec">- The exception code associated with this exception.</param>
		/// <param name="bNested">- A bool that defines wether the construction call is nested in another constructor or not.</param>
		Exception(
			_In_z_ LPCSTR lpMessage,
			_In_ ExceptionCode ec,
			_In_ BOOL bNested = FALSE
		);

#undef GetExceptionCode
		inline _Check_return_ ExceptionCode GetExceptionCode() const { return this->ec; }
		inline _Check_return_ _Ret_notnull_ LPCSTR GetExceptionMessage() const { return (LPCSTR)this->szMessage; }
		inline _Check_return_ _Ret_notnull_ LPCSTR GetFunctionName() const { return (LPCSTR)this->szFunction; }
	};

	/// <summary>
	/// An exception type for exceptions regarding parameters containing invalid values.
	/// </summary>
	class ARTEMIS_API ParameterException : public Exception {
		CHAR szParameter[MAX_NAME];
	public:
		/// <summary>
		/// Constructs a ParameterException.
		/// </summary>
		/// <param name="lpParameter">- The name of the parameter that contains an invalid value.</param>
		ParameterException(
			_In_ LPCSTR lpParameter
		);

		/// <summary>
		/// Gets the name of the parameter associated with this exception.
		/// </summary>
		/// <returns>The name of the parameter.</returns>
		inline _Check_return_ _Ret_notnull_ LPCSTR GetParameterName() const { return (LPCSTR)this->szParameter; }
	};

	/// <summary>
	/// An exception type for exceptions regarding a class instance being invalid.
	/// </summary>
	class ARTEMIS_API InstanceInvalidException : public Exception { public: InstanceInvalidException(); };

	/// <summary>
	/// Defines the type of operation the violation is associated with.
	/// </summary>
	enum class OperationType { Read, Write };

	/// <summary>
	/// An exception type for exceptions regarding memory access violations.
	/// </summary>
	class ARTEMIS_API MemoryAccessViolationException : public Exception {
		ADDRESS uAddress;
		DWORD dwSize;
		OperationType ot;

	public:
		/// <summary>
		/// Constructs a MemoryAccessViolationException.
		/// </summary>
		/// <param name="uAddress">- The address that threw an exception.</param>
		/// <param name="dwSize">- The size of the operation.</param>
		/// <param name="ot">- The type of operation that threw an exception.</param>
		MemoryAccessViolationException(
			_In_ ADDRESS uAddress,
			_In_ DWORD dwSize,
			_In_ OperationType ot
		);

		/// <summary>
		/// Gets the address associated with this exception.
		/// </summary>
		/// <returns>The associated exception.</returns>
		inline _Check_return_ ADDRESS GetAddress() const { return uAddress; }

		/// <summary>
		/// Gets the size of the operation associated with this exception.
		/// </summary>
		/// <returns>The associated size.</returns>
		inline _Check_return_ DWORD GetSize() const { return dwSize; }

		/// <summary>
		/// Gets the type of the associated operation.
		/// </summary>
		/// <returns></returns>
		inline _Check_return_ OperationType GetOperationType() const { return ot; }
	};

	/// <summary>
	/// An exception type for exceptions regarding subsequent calls to the Windows API failing.
	/// </summary>
	class ARTEMIS_API WindowsApiException : public Exception {
		CHAR szName[MAX_NAME];

		DWORD dwWinCode;
		CHAR szWinMessage[MAX_MESSAGE];
	public:
		/// <summary>
		/// Constructs a WindowsApiException.
		/// </summary>
		/// <param name="lpFunction">- The name of the Windows API function that failed.</param>
		WindowsApiException(_In_z_ LPCSTR lpFunction);

		/// <summary>
		/// Constructs a WindowsApiException.
		/// </summary>
		/// <param name="lpFunction">- The name of the Windows API function that failed.</param>
		/// <param name="dwWindowsCode">- The code returned by GetLastError.</param>
		WindowsApiException(_In_z_ LPCSTR lpFunction, _In_ DWORD dwWindowsCode);

		/// <summary>
		/// Gets the name of the Windows API function associated with this exception.
		/// </summary>
		/// <returns>The name of the associated function.</returns>
		inline _Check_return_ _Ret_notnull_ LPCSTR GetWindowsFunction() const { return szName; }

		/// <summary>
		/// Gets the Windows API error code associated with this exception.
		/// </summary>
		/// <returns>The associated error code.</returns>
		inline _Check_return_ DWORD GetWindowsCode() const { return dwWinCode; }

		/// <summary>
		/// Gets the Windows API error message associated with this exception.
		/// </summary>
		/// <returns>The associated error message.</returns>
		inline _Check_return_ _Ret_notnull_ LPCSTR GetWindowsMessage() const { return szWinMessage; }
	};

	/// <summary>
	/// An exception type for exceptions regarding an object searched for not being found.
	/// </summary>
	class ARTEMIS_API ObjectNotFoundException : public Exception {
		CHAR szName[MAX_NAME];
		CHAR szType[MAX_NAME];

	public:
		/// <summary>
		/// Constructs an ObjectNotFoundException.
		/// </summary>
		/// <param name="lpName">- The name of the object associated with this exception.</param>
		/// <param name="lpType">- The type of the object associated with this exception.</param>
		ObjectNotFoundException(_In_ LPCSTR lpName, _In_ LPCSTR lpType);

		/// <summary>
		/// Gets the name of the object associated with this exception.
		/// </summary>
		/// <returns>The associated name.</returns>
		inline _Check_return_ _Ret_notnull_ LPCSTR GetObjectName() const { return szName; }

		/// <summary>
		/// Gets the type of the object associated with this exception.
		/// </summary>
		/// <returns>the associated type.</returns>
		inline _Check_return_ _Ret_notnull_ LPCSTR GetObjectType() const { return szType; }
	};

	/// <summary>
	/// An exception type for exceptions regarding a feature not being implemented yet.
	/// </summary>
	class ARTEMIS_API NotImplementedException : public Exception { public: NotImplementedException(); };

	/// <summary>
	/// An exception type for exceptions regarding different parameter values not being compatible with each other.
	/// </summary>
	class ARTEMIS_API CompatibilityException : public Exception { public: CompatibilityException(); };

	/// <summary>
	/// An exception type for exceptions regarding object attributes being incompatible with the application they are being used in.
	/// </summary>
	class ARTEMIS_API AttributeException : public Exception {
		CHAR szAttributeName[MAX_NAME];

	public:
		/// <summary>
		/// Constructs an AttributeException.
		/// </summary>
		/// <param name="lpAttributeName">- The name of the incompatible attribute.</param>
		AttributeException(_In_z_ LPCSTR lpAttributeName);

		/// <summary>
		/// Gets the name of the attribute that was incompatible.
		/// </summary>
		/// <returns>The attribute name.</returns>
		inline _Check_return_ _Ret_notnull_ LPCSTR GetAttributeName() const { return szAttributeName; }
	};

	/// <summary>
	/// An exception type for exceptions regarding an index being out of range.
	/// </summary>
	class ARTEMIS_API IndexOutOfRangeException : public Exception {
		INT nIndex;
		INT nMaxIndex;

	public:
		/// <summary>
		/// Constructs an IndexOutOfRangeException.
		/// </summary>
		/// <param name="nIndex">- The invalid index.</param>
		/// <param name="nMaxIndex">- The maximum index.</param>
		IndexOutOfRangeException(_In_ INT nIndex, _In_ INT nMaxIndex);

		/// <summary>
		/// Gets the index that was invalid.
		/// </summary>
		/// <returns>The invalid index.</returns>
		inline _Check_return_ INT GetIndex() const { return nIndex; }

		/// <summary>
		/// Gets the maximum valid index.
		/// </summary>
		/// <returns>The maximum valid index.</returns>
		inline _Check_return_ INT GetMaxIndex() const { return nMaxIndex; }
	};

	/// <summary>
	/// An exception type for all exceptions regarding hooking functions.
	/// </summary>
	class ARTEMIS_API HookException : public Exception {
		HookException(_In_z_ LPCSTR lpMessage, _In_ ExceptionCode ec);

	public:
		/// <summary>
		/// A static subclass containing functions to quickly create different instances of the HookException class.
		/// </summary>
		class Create {
		public:
			static inline _Check_return_ HookException AlreadyInitialized() { return HookException("Hooking library is already initialized.", ExceptionCode::AlreadyInitialized); }
			static inline _Check_return_ HookException NotInitialized() { return HookException("Hooking library is not initialized.", ExceptionCode::NotInitialized); }
			static inline _Check_return_ HookException AlreadyCreated() { return HookException("Hook is already created.", ExceptionCode::AlreadyCreated); }
			static inline _Check_return_ HookException NotCreated() { return HookException("Hook is not created.", ExceptionCode::NotCreated); }
			static inline _Check_return_ HookException AlreadyEnabled() { return HookException("Hook is already enabled.", ExceptionCode::AlreadyEnabled); }
			static inline _Check_return_ HookException NotEnabled() { return HookException("Hook is not enabled.", ExceptionCode::NotEnabled); }
			static inline _Check_return_ HookException NotExecutable() { return HookException("Function pointer points to invalid and/or non-executable memory.", ExceptionCode::NotExecutable); }
			static inline _Check_return_ HookException UnsupportedFunction() { return HookException("The target function is unsupported.", ExceptionCode::UnsupportedFunction); }
			static inline _Check_return_ HookException MemoryAlloc() { return HookException("Failed to allocate memory.", ExceptionCode::MemoryAlloc); }
			static inline _Check_return_ HookException MemoryProtect() { return HookException("Failed to change memory protection.", ExceptionCode::MemoryProtect); }
			static inline _Check_return_ HookException ModuleNotFound() { return HookException("Failed to locate the specified module.", ExceptionCode::ModuleNotFound); }
			static inline _Check_return_ HookException FunctionNotFound() { return HookException("Failed to locate the specified function.", ExceptionCode::FunctionNotFound); }
		};
	};

	typedef void(*ExceptionEventHandler)(Exception* lpException);												// A template for an Exception event handler.
	typedef void(*ParameterExceptionEventHandler)(ParameterException* lpException);								// A template for a ParameterException event handler.
	typedef void(*InstanceInvalidExceptionEventHandler)(InstanceInvalidException* lpException);					// A template for a InstanceInvalidException event handler.
	typedef void(*MemoryAccessViolationExceptionEventHandler)(MemoryAccessViolationException* lpException);		// A template for a MemoryAccessViolationException event handler.
	typedef void(*WindowsApiExceptionEventHandler)(WindowsApiException* lpException);							// A template for a WindowsApiException event handler.
	typedef void(*ObjectNotFoundExceptionEventHandler)(ObjectNotFoundException* lpException);					// A template for an ObjectNotFoundException event handler.
	typedef void(*NotImplementedExceptionEventHandler)(NotImplementedException* lpException);					// A template for a NotImplementedException event handler.
	typedef void(*CompatibilityExceptionEventHandler)(CompatibilityException* lpException);						// A template for a CompatibilityException event handler.
	typedef void(*AttributeExceptionEventHandler)(AttributeException* lpException);								// A template for an AttributeException event handler.
	typedef void(*IndexOutOfRangeExceptionEventHandler)(IndexOutOfRangeException* lpException);					// A template for an IndexOutOfRangeException event handler.
	typedef void(*HookExceptionEventHandler)(HookException* lpException);										// A template for a HookException event handler.

	/// <summary>
	/// A static class for registering and unregistering exception event handler.
	/// </summary>
	class ARTEMIS_API ExceptionEventManager {
		ExceptionEventManager() {}
		ExceptionEventManager(const ExceptionEventManager&) {}

	public:
		/// <summary>
		/// Gets a reference to the Exception event.
		/// </summary>
		/// <returns>A reference to the event.</returns>
		static _Check_return_ Event<ExceptionEventHandler>& GetExceptionEvent();

		/// <summary>
		/// Gets a reference to the ParameterException event.
		/// </summary>
		/// <returns>A reference to the event.</returns>
		static _Check_return_ Event<ParameterExceptionEventHandler>& GetParameterExceptionEvent();

		/// <summary>
		/// Gets a reference to the InstanceInvalidException event.
		/// </summary>
		/// <returns>A reference to the event.</returns>
		static _Check_return_ Event<InstanceInvalidExceptionEventHandler>& GetInstanceInvalidExceptionEvent();

		/// <summary>
		/// Gets a reference to the MemoryAccessViolationException event.
		/// </summary>
		/// <returns>A reference to the event.</returns>
		static _Check_return_ Event<MemoryAccessViolationExceptionEventHandler>& GetMemoryAccessViolationExceptionEvent();

		/// <summary>
		/// Gets a reference to the WindowsApiException event.
		/// </summary>
		/// <returns>A reference to the event.</returns>
		static _Check_return_ Event<WindowsApiExceptionEventHandler>& GetWindowsApiExceptionEvent();

		/// <summary>
		/// Gets a reference to the ObjectNotFoundException event.
		/// </summary>
		/// <returns>A reference to the event.</returns>
		static _Check_return_ Event<ObjectNotFoundExceptionEventHandler>& GetObjectNotFoundExceptionEvent();

		/// <summary>
		/// Gets a reference to the NotImplementedException event.
		/// </summary>
		/// <returns>A reference to the event.</returns>
		static _Check_return_ Event<NotImplementedExceptionEventHandler>& GetNotImplementedExceptionEvent();

		/// <summary>
		/// Gets a reference to the CompatibilityException event.
		/// </summary>
		/// <returns>A reference to the event.</returns>
		static _Check_return_ Event<CompatibilityExceptionEventHandler>& GetCompatibilityExceptionEvent();

		/// <summary>
		/// Gets a reference to the AttributeException event.
		/// </summary>
		/// <returns>A reference to the event.</returns>
		static _Check_return_ Event<AttributeExceptionEventHandler>& GetAttributeExceptionEvent();

		/// <summary>
		/// Gets a reference to the IndexOutOfRangeException event.
		/// </summary>
		/// <returns>A reference to the event.</returns>
		static _Check_return_ Event<IndexOutOfRangeExceptionEventHandler>& GetIndexOutOfRangeExceptionEvent();

		/// <summary>
		/// Gets a reference to the HookException event.
		/// </summary>
		/// <returns>A reference to the event.</returns>
		static _Check_return_ Event<HookExceptionEventHandler>& GetHookExceptionEvent();
	};
}

#endif // !__EXCEPTIONS_H__