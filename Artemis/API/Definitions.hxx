#ifndef __ARTEMIS_API_DEFINITIONS_HXX__
#define __ARTEMIS_API_DEFINITIONS_HXX__

//*-----------------*
// Compile-time macro definitions changing the behaviour of the application:
// - ARTEMIS_EXPORTS
//		Sets the ARTEMIS_API macro to dllexport all functions instead of dllimport.
// - ARTEMIS_DISABLE_CALL_STACK
//		Makes the __stack_record() and __stack_escape() macros not call subsequent call_stack_manager functions.
// - ARTEMIS_TEST
//		Replaces the default entry point with an entry point that does not create a new main thread.
//*-----------------*

//*-----------------*
// Systematic logical rules:
// 1. If __stack_record() is called, __stack_escape() must also be called within the scope of the call.
//	  This means that __stack_escape() must ALWAYS be called before a return statement.
//	  This also means that the return statement itself HAS to be non-throwing.
//	  However, when an exception is thrown, __stack_escape() does not have to be called.
// 
// 2. An API function can omit calling __stack_record() and __stack_escape() if the function is non-throwing.
//    This means that the function does not throw, and does not call any throwing functions, and is declared noexcept.
// 
// 3. If a function that is potentially throwing an Artemis-exception, it has to be called
//    inside of a __stack_rethrow() statement in order for the call-stack to be rewound properly.
//
#if 0 // Example of 1, 2 and 3:
int boo(int x, int y) noexcept { return x * y * y * 0xDEADBEEF; }	// Does not throw, and is marked noexcept,
																	// so can omit calls to the call stack manager.

int foo() {
	__stack_record(); // Called in the beginning of every Artemis API function.

	if (error_state)
		throw exception("An error has occured.");	// Jumps out of the function,
													// but takes care of the call-stack itself,
													// so no need to call __stack_escape().

	__stack_escape(); // Called before the return statement.
	return boo(13, 37);	// Since the function is noexcept it does not have to be rethrown, and can be safely called in a return statement.
}

int bar() {
	__stack_record();

	int ret;
	__stack_rethrow(ret = foo());	// Since foo is potentially throwing an
									// Artemis-exception, the statement calling it
									// is put inside of a rethrow statement.
									// This makes sure that the call stack is properly
									// rewound when a throw in a nested function happens.

	__stack_escape();				// __stack_escape() is yet again called right before
									// the return statement, in order to rewind the call stack
									// if foo() succeeds.
	return ret;
}
#endif
//*-----------------*

#ifdef ARTEMIS_EXPORTS
#define ARTEMIS_API __declspec(dllexport)
#else
#define ARTEMIS_API __declspec(dllimport)
#endif // ARTEMIS_EXPORT

#ifndef NAMEOF
#define NAMEOF(x) [](auto, auto _) consteval { return _; }(sizeof(decltype(x)), #x)
#endif // NAMEOF

#ifndef CONSTANT
#define CONSTANT(x) const_cast<const decltype(x)&>(x)
#endif // !CONST

#endif // !__ARTEMIS_API_DEFINITIONS_HXX__