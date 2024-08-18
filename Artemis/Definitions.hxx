#ifndef ARTEMIS_DEFINITIONS_HXX
#define ARTEMIS_DEFINITIONS_HXX

#ifdef ARTEMIS_EXPORTS
#define ARTEMIS_FRAMEWORK __declspec(dllexport)
#else
#define ARTEMIS_FRAMEWORK __declspec(dllimport)
#endif // ARTEMIS_EXPORTS

#endif // !ARTEMIS_DEFINITIONS_HXX