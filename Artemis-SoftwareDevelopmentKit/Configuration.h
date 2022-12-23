//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <stdio.h>

#include "Definitions.h"
#include "Exceptions.h"

namespace Artemis {
	/// <summary>
	/// A class that contains helper functions for the Configuration classes.
	/// </summary>
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

		/// <summary>
		/// Opens the target file.
		/// </summary>
		/// <param name="nMode">- The open mode. See the Mode enum.</param>
		/// <exception cref="Exception"/>
		/// <exception cref="ParameterException"/>
		void OpenFile(_In_ INT nMode);

		/// <summary>
		/// Closes the previously opened target file.
		/// </summary>
		void CloseFile();
	};

	/// <summary>
	/// A class that represents a section of a Configuration file.
	/// </summary>
	class ARTEMIS_API ConfigurationSection : ConfigurationHelper {
		fpos_t nPos;

	public:
		ConfigurationSection();

		/// <summary>
		/// Constructs a ConfigurationSection.
		/// </summary>
		/// <param name="lpConfigFile">- The configuration file containing the section.</param>
		/// <param name="nPosition">- The seeker position of the beginning of the section.</param>
		ConfigurationSection(_In_z_ LPCSTR lpConfigFile, _In_ fpos_t nPosition);

		/// <summary>
		/// Gets the value of a property inside this section.
		/// </summary>
		/// <param name="lpPropertyName">- The name of the property to get the value of.</param>
		/// <returns>The value of the property.</returns>
		/// <exception cref="Exception"/>
		/// <exception cref="ParameterException"/>
		/// <exception cref="ObjectNotFoundException"/>
		_Check_return_ INT GetPropertyValue(_In_z_ LPCSTR lpPropertyName);

		/// <summary>
		/// Sets the value of a property inside this section.
		/// </summary>
		/// <param name="lpPropertyName">- The name of the property to set the value of.</param>
		/// <param name="nValue">- The value to set.</param>
		/// <exception cref="Exception"/>
		/// <exception cref="ParameterException"/>
		/// <exception cref="ObjectNotFoundException"/>
		void SetPropertyValue(_In_z_ LPCSTR lpPropertyName, _In_ INT nValue);

		/// <summary>
		/// Adds a new property to the section.
		/// </summary>
		/// <param name="lpPropertyName">- The name of the property to add.</param>
		/// <param name="nValue">- The value of the property.</param>
		/// <exception cref="Exception"/>
		/// <exception cref="ParameterException"/>
		/// <exception cref="ObjectNotFoundException"/>
		void AddProperty(_In_z_ LPCSTR lpPropertyName, _In_ INT nValue);

		/// <summary>
		/// Removes a property from the section.
		/// </summary>
		/// <param name="lpPropertyName">- The name of the property to remove.</param>
		/// <exception cref="Exception"/>
		/// <exception cref="ParameterException"/>
		/// <exception cref="ObjectNotFoundException"/>
		void RemoveProperty(_In_z_ LPCSTR lpPropertyName);
	};

	/// <summary>
	/// A class for managing a Configuration file.
	/// </summary>
	class ARTEMIS_API Configuration : ConfigurationHelper {
	public:
		/// <summary>
		/// Constructs a Configuration object.
		/// </summary>
		/// <param name="lpConfigFile">- The name of the configuration file.</param>
		Configuration(_In_z_ LPCSTR lpConfigFile);

		/// <summary>
		/// Validates a configuration file.
		/// </summary>
		/// <param name="lpConfigFile">- The name of the configuration file.</param>
		/// <returns>True if the file is valid, otherwise false.</returns>
		/// <exception cref="Exception"/>
		static bool ValidateFile(_In_z_ LPCSTR lpConfigFile);

		/// <summary>
		/// Gets a section of the configuration file.
		/// </summary>
		/// <param name="lpSectionName">- The name of the section to get.</param>
		/// <returns>The section instance.</returns>
		/// <exception cref="Exception"/>
		/// <exception cref="ParameterException"/>
		/// <exception cref="ObjectNotFoundException"/>
		_Check_return_ ConfigurationSection GetSection(_In_z_ LPCSTR lpSectionName);

		/// <summary>
		/// Adds a section to the configuration file.
		/// </summary>
		/// <param name="lpSectionName">- The name of the section to add.</param>
		/// <returns>The section instance.</returns>
		/// <exception cref="Exception"/>
		/// <exception cref="ParameterException"/>
		ConfigurationSection AddSection(_In_z_ LPCSTR lpSectionName);

		/// <summary>
		/// Removes a section from the configuration file.
		/// </summary>
		/// <param name="lpSectionName">- The name of the section to remove.</param>
		/// <exception cref="Exception"/>
		/// <exception cref="ParameterException"/>
		/// <exception cref="ObjectNotFoundException"/>
		void RemoveSection(_In_z_ LPCSTR lpSectionName);
	};
}

#endif // !__CONFIGURATION_H__