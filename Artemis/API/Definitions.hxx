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

// If you find yourself looking at this NAMEOF macro and asking yourself;
// "why the hell is it designed like this, does not #x suffice?":
// the reason I have designed it as is below, is for it to meet the following requirements:
// 1. For the macro to produce the error "x is not defined" when x is not defined.
// 2. For it to evaluate at compile time.
// 
// The way it accomplishes this is that sizeof(decltype(x)) produces that exact warning
// when passed something undefined, and the consteval lambda is guaranteed to evaluate
// to the string literal #x at compile time.

#ifndef NAMEOF
#define NAMEOF(x) [](auto, auto _) consteval { return _; }(sizeof(decltype(x)), #x)
#endif // NAMEOF

#ifndef CONSTANT
#define CONSTANT(x) const_cast<const decltype(x)&>(x)
#endif // !CONST

#endif // !__ARTEMIS_API_DEFINITIONS_HXX__