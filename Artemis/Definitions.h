#ifndef __ARTEMIS_DEFINITIONS_H__
#define __ARTEMIS_DEFINITIONS_H__

#define ARTEMIS_IMPORT __declspec(dllimport)
#define ARTEMIS_EXPORT __declspec(dllexport)

#ifdef _ARTEMIS_EXPORT
#define ARTEMIS_API ARTEMIS_EXPORT
#else
#define ARTEMIS_API ARTEMIS_IMPORT
#endif // _ARTEMIS_EXPORT

#define INVALID_INDEX (-1)

#endif // !__ARTEMIS_DEFINITIONS_H__