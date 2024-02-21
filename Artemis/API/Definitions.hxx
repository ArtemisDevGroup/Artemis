#ifndef ARTEMIS_API_DEFINITIONS_HXX
#define ARTEMIS_API_DEFINITIONS_HXX

#ifdef ARTEMIS_EXPORTS
#define ARTEMIS_API __declspec(dllexport)
#else
#define ARTEMIS_API __declspec(dllimport)
#endif // ARTEMIS_EXPORT

#endif // !ARTEMIS_API_DEFINTIONS_HXX