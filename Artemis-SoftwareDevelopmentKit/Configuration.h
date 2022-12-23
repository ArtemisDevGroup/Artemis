//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#pragma message("TODO: Add proper exception handling and inline document.")

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <stdio.h>

#include "Definitions.h"
#include "Exceptions.h"

namespace Artemis {
	class ARTEMIS_API ConfigurationHelper {
	public:
		CHAR szConfigFile[MAX_NAME];
		FILE* pFile;

		enum Mode {
			Read		= 1 << 0,
			Write		= 1 << 1,
			Append		= 1 << 2,
			Update		= 1 << 3
		};

		void OpenFile(_In_ INT nMode);
		void CloseFile();
	};

	class ARTEMIS_API ConfigurationSection : ConfigurationHelper {
		fpos_t nPos;

	public:
		ConfigurationSection();
		ConfigurationSection(_In_z_ LPCSTR lpConfigFile, _In_ fpos_t nPosition);

		INT GetPropertyValue(_In_z_ LPCSTR lpPropertyName);
		void SetPropertyValue(_In_z_ LPCSTR lpPropertyName, _In_ INT nValue);
		void AddProperty(_In_z_ LPCSTR lpPropertyName, _In_ INT nValue);
		void RemoveProperty(_In_z_ LPCSTR lpPropertyName);
	};

	class ARTEMIS_API Configuration : ConfigurationHelper {
	public:
		Configuration(_In_z_ LPCSTR lpConfigFile);

		static bool ValidateFile(_In_z_ LPCSTR lpConfigFile);

		_Check_return_ ConfigurationSection GetSection(_In_z_ LPCSTR lpSectionName);
		ConfigurationSection AddSection(_In_z_ LPCSTR lpSectionName);
		void RemoveSection(_In_z_ LPCSTR lpSectionName);
	};
}

#endif // !__CONFIGURATION_H__