//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->


#ifndef __EXTENSION_FRAMEWORK_H__
#define __EXTENSION_FRAMEWORK_H__

#include "..\Definitions.h"
#include "..\Exceptions.h"
#include "..\Interfaces.h"

namespace Artemis {
	
	typedef BOOL(APIENTRY* InitializeFunction)(_In_ HMODULE hModule);				// A function template for an extension Initialize function.
	typedef VOID(APIENTRY* ReleaseFunction)();										// A function template for an extension Release function.

	/// <summary>
	/// A struct that describes an extension.
	/// </summary>
	struct ExtensionDescriptor {
		CHAR szExtensionName[MAX_NAME];		// The name of the extension.
		CHAR szAuthorName[MAX_NAME];		// The name of the extension author.
		InitializeFunction Initialize;		// A pointer to the Initialize function.
		ReleaseFunction Release;			// A pointer to the Release function. Can be null.
	};

	/// <summary>
	/// A class that describes an extension.
	/// </summary>
	class ARTEMIS_API Extension : public IDisposable {
		typedef VOID(APIENTRY* GetExtensionDescriptorFunction)(_Out_ ExtensionDescriptor* pDescriptor);

		HMODULE hModule;
		GetExtensionDescriptorFunction GetExtensionDescriptor;
		ExtensionDescriptor Descriptor;

		Extension(const Extension&) = delete;

	public:
		/// <summary>
		/// Constructs an Extension object and prepares the extension for initialization.
		/// </summary>
		/// <param name="lpDllName">- The name of the dll to load.</param>
		/// <extension cref="WindowsApiException"/>
		Extension(_In_z_ LPCSTR lpDllName);

		/// <summary>
		/// Gets the name of the extension.
		/// </summary>
		/// <returns>The extension name.</returns>
		_Check_return_ _Ret_z_ LPCSTR GetExtensionName() const;

		/// <summary>
		/// Gets the author name of the extension.
		/// </summary>
		/// <returns>The author name.</returns>
		_Check_return_ _Ret_z_ LPCSTR GetAuthorName() const;

		/// <summary>
		/// Initializes the extension.
		/// </summary>
		/// <returns>
		/// <para>TRUE if initialization succeeds. Otherwise FALSE.</para>
		/// <para>If this function returns FALSE, Release shall be called immediately.</para>
		/// </returns>
		_Check_return_ BOOL Initialize() const;

		/// <summary>
		/// Releases the library.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void Release();

		~Extension();
	};

	/// <summary>
	/// A class for managing all the extensions.
	/// </summary>
	class ARTEMIS_API ExtensionManager : public IDisposable {
		Extension* lpszExtensions[MAX_INVOKE];
		INT nExtensionCount;

	public:
		ExtensionManager();

		/// <summary>
		/// Loads all extensions in the Artemis-Extensions sub directory.
		/// </summary>
		/// <exception cref="WindowsApiException"/>
		void LoadAll();

		/// <summary>
		/// Gets a pointer to a loaded extension instance by its name.
		/// </summary>
		/// <returns>A pointer to the Exception instance.</returns>
		/// <exception cref="ObjectNotFoundException"/>
		_Check_return_ _Ret_maybenull_ Extension* GetExtensionByName(_In_z_ LPCSTR lpExtensionName);

		/// <summary>
		/// Releases all loaded extensions.
		/// </summary>
		void Release();

		~ExtensionManager();
	};
}

#endif // !__EXTENSION_FRAMEWORK_H__