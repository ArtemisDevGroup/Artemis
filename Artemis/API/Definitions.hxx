#ifndef ARTEMIS_API_DEFINITIONS_HXX
#define ARTEMIS_API_DEFINITIONS_HXX

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
// - Nested calls HAVE to rethrow exceptions by throwing the object returned by the e.rethrow() method in the catch clause.
#if 0 // Ex:
try {
	nested_call(); // This call potentially throws an Artemis exception.
}
catch (exception& e) {
	throw e.rethrow<decltype(e)>(); // The caller rethrows the exception.
									// NOTE: The exception event is only invoked for the original throw, and not for the rethrow.
}
#endif
//*-----------------*

#ifdef ARTEMIS_EXPORTS
#define ARTEMIS_API __declspec(dllexport)
#else
#define ARTEMIS_API __declspec(dllimport)
#endif // ARTEMIS_EXPORT

#endif // !ARTEMIS_API_DEFINTIONS_HXX