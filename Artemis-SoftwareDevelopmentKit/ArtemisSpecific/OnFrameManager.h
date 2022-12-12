//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __ONFRAME_MANAGER_H__
#define __ONFRAME_MANAGER_H__

#include "..\Definitions.h"
#include "..\Interfaces.h"
#include "..\Exceptions.h"

namespace Artemis {
	/// <summary>
	/// An abstract class for running an action at the beginning of every frame.
	/// </summary>
	class ARTEMIS_API IOnFrame : public IRegisterable {
	public:
		/// <summary>
		/// Sets the data fields.
		/// </summary>
		/// <param name="dwOnFrameId">- The OnFrame action id.</param>
		inline IOnFrame(_In_ DWORD dwOnFrameId) : IRegisterable(), dwOnFrameId(dwOnFrameId) {}

		const DWORD dwOnFrameId; // The OnFrame action id.
		
		/// <summary>
		/// A purely virtual function that shall contain the action to execute.
		/// </summary>
		virtual void OnFrame() = 0;
	};

	/// <summary>
	/// A class for registering actions to run at the beginning of every frame.
	/// </summary>
	class ARTEMIS_API OnFrameManager : public IDisposable {
		IOnFrame* lpszOnFrameArray[MAX_INVOKE];

	public:
		OnFrameManager();

		/// <summary>
		/// Registers an OnFrame action to be ran every frame.
		/// <para>The pointer MUST be to memory created with the "new" keyword.</para>
		/// </summary>
		/// <param name="lpOnFrameAction">- A pointer to the derived class.</param>
		/// <exception cref="ParameterException"/>
		/// <exception cref="AttributeException"/>
		void RegisterOnFrameAction(_In_ IOnFrame* lpOnFrameAction);

		/// <summary>
		/// Unregisters a previously registered OnFrame class instance.
		/// <para>The pointer MUST be to memory created with the "new" keyword.</para>
		/// </summary>
		/// <param name="lpOnFrameAction">- A pointer to the derived class.</param>
		/// <exception cref="ObjectNotFoundException"/>
		void UnregisterOnFrameAction(_In_ IOnFrame* lpOnFrameAction);

		/// <summary>
		/// Deletes all memory pointed to by the registered pointers.
		/// </summary>
		void Release();

		/// <summary>
		/// Invokes all registered OnFrame actions.
		/// </summary>
		void InvokeOnFrameActions();

		/// <summary>
		/// Gets the pointer to an OnFrame instance by its unique identifier.
		/// </summary>
		/// <param name="dwOnFrameId">- The identifier of the OnFrame action to get.</param>
		/// <exception cref="ObjectNotFoundException"/>
		_Check_return_ _Ret_maybenull_ IOnFrame* GetOnFrameActionById(_In_ DWORD dwOnFrameId);
	};
}

#endif // !__ONFRAME_MANAGER_H__