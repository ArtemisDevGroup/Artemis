//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __KEYBIND_MANAGER_H__
#define __KEYBIND_MANAGER_H__

#include "..\Definitions.h"
#include "..\Interfaces.h"
#include "..\Exceptions.h"

namespace Artemis {
	/// <summary>
	/// An interface for binding key presses to operations.
	/// </summary>
	class ARTEMIS_API IKeybind : public IRegisterable {
	public:
		IKeybind(_In_ WORD wKey);

		/// <summary>
		/// The virtual key to trigger on.
		/// </summary>
		const WORD wKey;

		/// <summary>
		/// A purely virtual function that handles what should happen when the key is pressed.
		/// </summary>
		virtual void WhenPressed() = 0;

		/// <summary>
		/// Invokes the keybind and checks if the key is pressed.
		/// </summary>
		void Invoke();
	};

	/// <summary>
	/// A class for binding key presses to operations.
	/// </summary>
	class ARTEMIS_API KeybindManager : public IDisposable {
		IKeybind* lpszKeybindArray[MAX_INVOKE];

	public:
		KeybindManager();

		/// <summary>
		/// Registers a keybind instance.
		/// <para>The pointer MUST be to memory created with the "new" keyword.</para>
		/// </summary>
		/// <param name="lpKeybindInst">- A pointer to the derived class.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="AttributeException"/>
		void RegisterKeybind(_In_ IKeybind* lpKeybindInst);

		/// <summary>
		/// Unregisters a previously registered keybind instance.
		/// <para>The pointer MUST be to memory created with the "new" keyword.</para>
		/// </summary>
		/// <param name="lpKeybindInst">- A pointer to the derived class.</param>
		/// <exception cref="ObjectNotFoundException"/>
		void UnregisterKeybind(_In_ IKeybind* lpKeybindInst);

		/// <summary>
		/// Deletes all memory pointed to by the registered pointers.
		/// </summary>
		void Release();

		/// <summary>
		/// Invokes all registered keybinds.
		/// </summary>
		void InvokeKeybinds();

		/// <summary>
		/// Gets the pointer to a keybind instance by its associated virtual key.
		/// </summary>
		/// <param name="wKey">- The virtual key of the keybind to get.</param>
		/// <exception cref="ObjectNotFoundException"/>
		_Check_return_ _Ret_maybenull_ IKeybind* GetKeybindByVirtualKey(_In_ WORD wKey);

	};
}

#endif // !__KEYBIND_MANAGER_H__