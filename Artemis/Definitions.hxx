#ifndef ARTEMIS_DEFINITIONS_HXX
#define ARTEMIS_DEFINITIONS_HXX

#ifdef ARTEMIS_EXPORTS
#define ARTEMIS_FRAMEWORK __declspec(dllexport)
#define ARTEMIS_INTERNAL_HEADER
#else
#define ARTEMIS_FRAMEWORK __declspec(dllimport)
#define ARTEMIS_INTERNAL_HEADER static_assert(false, "This header is only for internal use when compiling the main Artemis module. If you see this message, delete the header propagating this error.");
#endif // ARTEMIS_EXPORTS

#endif // !ARTEMIS_DEFINITIONS_HXX