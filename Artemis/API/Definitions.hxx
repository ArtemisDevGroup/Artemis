#ifndef __ARTEMIS_API_DEFINITIONS_HXX__
#define __ARTEMIS_API_DEFINITIONS_HXX__

//*-----------------*
// Compile-time macro definitions changing the behaviour of the application:
// - ARTEMIS_EXPORTS
//		Sets the ARTEMIS_API macro to dllexport all functions instead of dllimport.
// - ARTEMIS_TEST
//		Replaces the default entry point with an entry point that does not create a new main thread.
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