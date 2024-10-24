#ifndef __ARTEMIS_DEFINITIONS_HXX__
#define __ARTEMIS_DEFINITIONS_HXX__

#ifdef ARTEMIS_EXPORTS
#define ARTEMIS_FRAMEWORK __declspec(dllexport)
#define ARTEMIS_INTERNAL_HEADER
#else
#define ARTEMIS_FRAMEWORK __declspec(dllimport)
#define ARTEMIS_INTERNAL_HEADER static_assert(false, "This header is only for internal use when compiling the main Artemis module. If you see this message, delete the header propagating this error.");
#endif // ARTEMIS_EXPORTS

#ifndef NAMEOF
#define NAMEOF(x) [](auto, auto _) consteval { return _; }(sizeof(decltype(x)), #x)
#endif // NAMEOF

#ifndef CONSTANT
#define CONSTANT(x) const_cast<const decltype(x)&>(x)
#endif // !CONST

#endif // !__ARTEMIS_DEFINITIONS_HXX__