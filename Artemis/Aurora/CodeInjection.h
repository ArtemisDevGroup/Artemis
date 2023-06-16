//------------------------------------------------------------------------>
// MIT License
// 
// Copyright (c) 2023 Artemis Group
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------>
// Aurora: https://github.com/ArtemisDevGroup/Aurora
// This file was authored by:
// @Astrea0014: https://github.com/Astrea0014
//------------------------------------------------------------------------>

#ifndef __AURORA_CODE_INJECTION_H__
#define __AURORA_CODE_INJECTION_H__

#include "Definitions.h"
#include "Exceptions.h"
#include "MemoryTypes.h"
#include "Trampoline.h"
#include "SharedHandle.h"
#include "MemoryWin32.h"
#include "ProcessInfo.h"

#include "HDE/hde32.h"
#include "HDE/hde64.h"

namespace Aurora {
	namespace Helpers {
		constexpr A_DWORD c_dwRelJmp32Size = 5;

		AURORA_NDWR_PURE("Rel32_JmpOffset32") constexpr A_DWORD Rel32_JmpOffset32(_In_a32_ A_ADDR32 uFrom, _In_a32_ A_ADDR32 uTo) noexcept { return uTo - (uFrom + 5); }
		AURORA_NDWR_PURE("Rel32_JmpOffset64") constexpr A_DWORD Rel32_JmpOffset64(_In_a64_ A_ADDR64 uFrom, _In_a64_ A_ADDR64 uTo) noexcept { return *(A_LPDWORD)&uTo - (*(A_LPDWORD)&uFrom + 5); }

		AURORA_API A_BOOL SEHMemoryCopy(
			_Out_writes_bytes_(dwSize) A_LPVOID lpDestination,
			_In_reads_bytes_(dwSize) A_LPCVOID lpSource,
			_In_ A_DWORD dwSize
		);
	}

	/// <summary>
	/// Used for injecting code at an address.
	/// </summary>
	class AURORA_API CodeInjection32 {
		TrampolineManager32* lpTrampolineManager;
		TrampolineDescriptor32* lpTrampoline;

		A_ADDR32 uInjectionPoint;
		A_LPBYTE lpInjectionPayload;
		A_LPBYTE lpOriginalCode;
		A_DWORD dwInjectionSize;

		HANDLE hProcess;

		A_BOOL bEnabled;

		A_BOOL ReadMemory(_In_a32_ A_ADDR32 uAddress, _Out_writes_bytes_(dwSize) A_LPVOID lpBuffer, _In_ A_DWORD dwSize) noexcept;
		A_BOOL WriteMemory(_In_a32_ A_ADDR32 uAddress, _In_reads_bytes_(dwSize) A_LPCVOID lpBuffer, _In_ A_DWORD dwSize) noexcept;

	public:
		CodeInjection32();

		/// <summary>
		/// Creates a code injection in the local process.
		/// </summary>
		/// <typeparam name="nSize">- The size of the injection.</typeparam>
		/// <param name="uInjectionPoint">- The address of the injection point.</param>
		/// <param name="refInjectionCode">- The code to inject.</param>
		/// <param name="bKeepOverwrittenCode">- Determines whether to keep the code overwritten by the jmp or not.</param>
		/// <exception cref="WindowsApiException"/>
		/// <exception cref="Read32Exception"/>
		/// <exception cref="Write32Exception"/>
		template<A_I32 nSize>
		CodeInjection32(
			_In_a32_ A_ADDR32 uInjectionPoint,
			_In_ const InstructionBytes<nSize>& refInjectionCode,
			_In_ A_BOOL bKeepOverwrittenCode = false
		) : lpTrampolineManager(nullptr), lpTrampoline(nullptr), lpInjectionPayload(nullptr), lpOriginalCode(nullptr), hProcess(nullptr), uInjectionPoint(uInjectionPoint), dwInjectionSize(0), bEnabled(false) {
			AuroraContextStart();

			// Create new trampoline manager.

			lpTrampolineManager = new TrampolineManager32();

			// Get size of overwritten instructions.

			A_BYTE szTempBuffer[32];
			if (!ReadMemory(uInjectionPoint, szTempBuffer, sizeof(szTempBuffer))) AuroraThrow(Read32Exception, uInjectionPoint, sizeof(szTempBuffer));

			do {
				hde32s hde;
				hde32_disasm((const void*)szTempBuffer, &hde);
				dwInjectionSize += hde.len;
			} while (dwInjectionSize < Helpers::c_dwRelJmp32Size);

			// Copy original code into buffer.

			lpOriginalCode = new A_BYTE[dwInjectionSize];
			memcpy(lpOriginalCode, szTempBuffer, dwInjectionSize);

			// Create a trampoline with the size of the injection + the relative offset jump instruction + the original code if specified.

			lpTrampoline = lpTrampolineManager->CreateTrampoline(refInjectionCode.size() + Helpers::c_dwRelJmp32Size + (bKeepOverwrittenCode ? dwInjectionSize : 0));

			// Prepares the injection payload for the assembly patch.

			lpInjectionPayload = new A_BYTE[dwInjectionSize];
			lpInjectionPayload[0] = 0xE9;
			*(A_LPDWORD)&lpInjectionPayload[1] = Helpers::Rel32_JmpOffset32(uInjectionPoint, lpTrampoline->uTrampolineAddress);

			for (A_I32 i = 5; i < dwInjectionSize; i++)
				lpInjectionPayload[i] = 0x90; // Padding with nops.

			A_ADDR32 uIndexedAddress = lpTrampoline->uTrampolineAddress;

			// Writes overwritten code if specified.

			if (bKeepOverwrittenCode) {
				if (!WriteMemory(uIndexedAddress, lpOriginalCode, dwInjectionSize)) AuroraThrow(Write32Exception, uIndexedAddress, dwInjectionSize);
				uIndexedAddress += dwInjectionSize;
			}

			// Writes injection code.

			if (!WriteMemory(uIndexedAddress, refInjectionCode.szBytes, refInjectionCode.size())) AuroraThrow(Write32Exception, uIndexedAddress, refInjectionCode.size());
			uIndexedAddress += refInjectionCode.size();

			// Gets and writes relative jmp code.

			A_BYTE szJmpBackCode[Helpers::c_dwRelJmp32Size] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
			*(A_LPDWORD)&szJmpBackCode[1] = Helpers::Rel32_JmpOffset32(uIndexedAddress, uInjectionPoint + Helpers::c_dwRelJmp32Size);

			if (!WriteMemory(uIndexedAddress, szJmpBackCode, sizeof(szJmpBackCode))) AuroraThrow(Write32Exception, uIndexedAddress, Helpers::c_dwRelJmp32Size);

			AuroraContextEnd();
		}

		/// <summary>
		/// Creates a code injection in a remote process.
		/// </summary>
		/// <typeparam name="nSize">- The size of the injection.</typeparam>
		/// <param name="refTargetProcess">- A reference to the target process.</param>
		/// <param name="uInjectionPoint">- The address of the injection point.</param>
		/// <param name="refInjectionCode">- The code to inject.</param>
		/// <param name="bKeepOverwrittenCode">- Determines whether to keep the code overwritten by the jmp or not.</param>
		/// <exception cref="WindowsApiException"/>
		/// <exception cref="Read32Exception"/>
		/// <exception cref="Write32Exception"/>
		template<A_I32 nSize>
		CodeInjection32(
			_In_ const ProcessInfo32& refTargetProcess,
			_In_a32_ A_ADDR32 uInjectionPoint,
			_In_ const InstructionBytes<nSize>& refInjectionCode,
			_In_ A_BOOL bKeepOverwrittenCode = false
		) : lpTrampolineManager(nullptr), lpTrampoline(nullptr), lpInjectionPayload(nullptr), lpOriginalCode(nullptr), hProcess(nullptr), uInjectionPoint(uInjectionPoint), dwInjectionSize(0), bEnabled(false) {
			AuroraContextStart();

			// Get handle to remote process.

			hProcess = OpenSharedProcess(ProcessAccessFlags::Vm, false, refTargetProcess.GetProcessId());

			// Create new trampoline manager.

			lpTrampolineManager = new TrampolineManager32(refTargetProcess);

			// Get size of overwritten instructions.

			A_BYTE szTempBuffer[32];
			if (!ReadMemory(uInjectionPoint, szTempBuffer, sizeof(szTempBuffer))) AuroraThrow(Read32Exception, uInjectionPoint, sizeof(szTempBuffer));

			do {
				hde32s hde;
				hde32_disasm((const void*)szTempBuffer, &hde);
				dwInjectionSize += hde.len;
			} while (dwInjectionSize < Helpers::c_dwRelJmp32Size);

			// Copy original code into buffer.

			lpOriginalCode = new A_BYTE[dwInjectionSize];
			memcpy(lpOriginalCode, szTempBuffer, dwInjectionSize);

			// Create a trampoline with the size of the injection + the relative offset jump instruction + the original code if specified.

			lpTrampoline = lpTrampolineManager->CreateTrampoline(refInjectionCode.size() + Helpers::c_dwRelJmp32Size + (bKeepOverwrittenCode ? dwInjectionSize : 0));

			// Prepares the injection payload for the assembly patch.

			lpInjectionPayload = new A_BYTE[dwInjectionSize];
			lpInjectionPayload[0] = 0xE9;
			*(A_LPDWORD)&lpInjectionPayload[1] = Helpers::Rel32_JmpOffset32(uInjectionPoint, lpTrampoline->uTrampolineAddress);

			for (A_I32 i = 5; i < dwInjectionSize; i++)
				lpInjectionPayload[i] = 0x90; // Padding with nops.

			A_ADDR32 uIndexedAddress = lpTrampoline->uTrampolineAddress;

			// Writes overwritten code if specified.

			if (bKeepOverwrittenCode) {
				if (!WriteMemory(uIndexedAddress, lpOriginalCode, dwInjectionSize)) AuroraThrow(Write32Exception, uIndexedAddress, dwInjectionSize);
				uIndexedAddress += dwInjectionSize;
			}

			// Writes injection code.

			if (!WriteMemory(uIndexedAddress, refInjectionCode.szBytes, refInjectionCode.size())) AuroraThrow(Write32Exception, uIndexedAddress, refInjectionCode.size());
			uIndexedAddress += refInjectionCode.size();

			// Gets and writes relative jmp code.

			A_BYTE szJmpBackCode[Helpers::c_dwRelJmp32Size] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
			*(A_LPDWORD)&szJmpBackCode[1] = Helpers::Rel32_JmpOffset32(uIndexedAddress, uInjectionPoint + Helpers::c_dwRelJmp32Size);

			if (!WriteMemory(uIndexedAddress, szJmpBackCode, sizeof(szJmpBackCode))) AuroraThrow(Write32Exception, uIndexedAddress, Helpers::c_dwRelJmp32Size);

			AuroraContextEnd();
		}

		CodeInjection32(const CodeInjection32&) = delete;

		~CodeInjection32();

		/// <summary>
		/// Enables the code injection.
		/// </summary>
		/// <exception cref="Write32Exception"/>
		A_VOID Enable();

		/// <summary>
		/// Disables the code injection.
		/// </summary>
		/// <exception cref="Write32Exception"/>
		A_VOID Disable();
	};

	/// <summary>
	/// Used for injecting code at an address.
	/// </summary>
	class AURORA_API CodeInjection64 {
		TrampolineManager64* lpTrampolineManager;
		TrampolineDescriptor64* lpTrampoline;

		A_ADDR64 uInjectionPoint;
		A_LPBYTE lpInjectionPayload;
		A_LPBYTE lpOriginalCode;
		A_DWORD dwInjectionSize;

		HANDLE hProcess;

		A_BOOL bEnabled;

		A_BOOL ReadMemory(_In_a64_ A_ADDR64 uAddress, _Out_writes_bytes_(dwSize) A_LPVOID lpBuffer, _In_ A_DWORD dwSize) noexcept;
		A_BOOL WriteMemory(_In_a64_ A_ADDR64 uAddress, _In_reads_bytes_(dwSize) A_LPCVOID lpBuffer, _In_ A_DWORD dwSize) noexcept;

	public:
		CodeInjection64();

		/// <summary>
		/// Creates a code injection in the local process.
		/// </summary>
		/// <typeparam name="nSize">- The size of the injection.</typeparam>
		/// <param name="uInjectionPoint">- The address of the injection point.</param>
		/// <param name="refInjectionCode">- The code to inject.</param>
		/// <param name="bKeepOverwrittenCode">- Determines whether to keep the code overwritten by the jmp or not.</param>
		/// <exception cref="WindowsApiException"/>
		/// <exception cref="Read64Exception"/>
		/// <exception cref="Write64Exception"/>
		template<A_I32 nSize>
		CodeInjection64(
			_In_a64_ A_ADDR64 uInjectionPoint,
			_In_ const InstructionBytes<nSize>& refInjectionCode,
			_In_ A_BOOL bKeepOverwrittenCode = false
		) : lpTrampolineManager(nullptr), lpTrampoline(nullptr), lpInjectionPayload(nullptr), lpOriginalCode(nullptr), hProcess(nullptr), uInjectionPoint(uInjectionPoint), dwInjectionSize(0), bEnabled(false) {
			AuroraContextStart();

			// Create new trampoline manager.

			lpTrampolineManager = new TrampolineManager64();

			// Get size of overwritten instructions.

			A_BYTE szTempBuffer[32];
			if (!ReadMemory(uInjectionPoint, szTempBuffer, sizeof(szTempBuffer))) AuroraThrow(Read64Exception, uInjectionPoint, sizeof(szTempBuffer));

			do {
				hde64s hde;
				hde64_disasm((const void*)szTempBuffer, &hde);
				dwInjectionSize += hde.len;
			} while (dwInjectionSize < Helpers::c_dwRelJmp32Size);

			// Copy original code into buffer.

			lpOriginalCode = new A_BYTE[dwInjectionSize];
			memcpy(lpOriginalCode, szTempBuffer, dwInjectionSize);

			// Create a trampoline with the size of the injection + the relative offset jump instruction + the original code if specified.

			lpTrampoline = lpTrampolineManager->CreateTrampoline(refInjectionCode.size() + Helpers::c_dwRelJmp32Size + (bKeepOverwrittenCode ? dwInjectionSize : 0));

			// Prepares the injection payload for the assembly patch.

			lpInjectionPayload = new A_BYTE[dwInjectionSize];
			lpInjectionPayload[0] = 0xE9;
			*(A_LPDWORD)&lpInjectionPayload[1] = Helpers::Rel32_JmpOffset64(uInjectionPoint, lpTrampoline->uTrampolineAddress);

			for (A_I32 i = 5; i < dwInjectionSize; i++)
				lpInjectionPayload[i] = 0x90; // Padding with nops.

			A_ADDR64 uIndexedAddress = lpTrampoline->uTrampolineAddress;

			// Writes overwritten code if specified.

			if (bKeepOverwrittenCode) {
				if (!WriteMemory(uIndexedAddress, lpOriginalCode, dwInjectionSize)) AuroraThrow(Write64Exception, uIndexedAddress, dwInjectionSize);
				uIndexedAddress += dwInjectionSize;
			}

			// Writes injection code.

			if (!WriteMemory(uIndexedAddress, refInjectionCode.szBytes, refInjectionCode.size())) AuroraThrow(Write64Exception, uIndexedAddress, refInjectionCode.size());
			uIndexedAddress += refInjectionCode.size();

			// Gets and writes relative jmp code.

			A_BYTE szJmpBackCode[Helpers::c_dwRelJmp32Size] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
			*(A_LPDWORD)&szJmpBackCode[1] = Helpers::Rel32_JmpOffset64(uIndexedAddress, uInjectionPoint + Helpers::c_dwRelJmp32Size);

			if (!WriteMemory(uIndexedAddress, szJmpBackCode, sizeof(szJmpBackCode))) AuroraThrow(Write64Exception, uIndexedAddress, Helpers::c_dwRelJmp32Size);

			AuroraContextEnd();
		}

		/// <summary>
		/// Creates a code injection in a remote process.
		/// </summary>
		/// <typeparam name="nSize">- The size of the injection.</typeparam>
		/// <param name="refTargetProcess">- A reference to the target process.</param>
		/// <param name="uInjectionPoint">- The address of the injection point.</param>
		/// <param name="refInjectionCode">- The code to inject.</param>
		/// <param name="bKeepOverwrittenCode">- Determines whether to keep the code overwritten by the jmp or not.</param>
		/// <exception cref="WindowsApiException"/>
		/// <exception cref="Read64Exception"/>
		/// <exception cref="Write64Exception"/>
		template<A_I32 nSize>
		CodeInjection64(
			_In_ const ProcessInfo64& refTargetProcess,
			_In_a64_ A_ADDR64 uInjectionPoint,
			_In_ const InstructionBytes<nSize>& refInjectionCode,
			_In_ A_BOOL bKeepOverwrittenCode = false
		) : lpTrampolineManager(nullptr), lpTrampoline(nullptr), lpInjectionPayload(nullptr), lpOriginalCode(nullptr), hProcess(nullptr), uInjectionPoint(uInjectionPoint), dwInjectionSize(0), bEnabled(false) {
			AuroraContextStart();

			// Get handle to remote process if remote.

			hProcess = OpenSharedProcess(ProcessAccessFlags::Vm, false, refTargetProcess.GetProcessId());

			// Create new trampoline manager.

			lpTrampolineManager = new TrampolineManager64(refTargetProcess);

			// Get size of overwritten instructions.

			A_BYTE szTempBuffer[32];
			if (!ReadMemory(uInjectionPoint, szTempBuffer, sizeof(szTempBuffer))) AuroraThrow(Read64Exception, uInjectionPoint, sizeof(szTempBuffer));

			do {
				hde64s hde;
				hde64_disasm((const void*)szTempBuffer, &hde);
				dwInjectionSize += hde.len;
			} while (dwInjectionSize < Helpers::c_dwRelJmp32Size);

			// Copy original code into buffer.

			lpOriginalCode = new A_BYTE[dwInjectionSize];
			memcpy(lpOriginalCode, szTempBuffer, dwInjectionSize);

			// Create a trampoline with the size of the injection + the relative offset jump instruction + the original code if specified.

			lpTrampoline = lpTrampolineManager->CreateTrampoline(refInjectionCode.size() + Helpers::c_dwRelJmp32Size + (bKeepOverwrittenCode ? dwInjectionSize : 0));

			// Prepares the injection payload for the assembly patch.

			lpInjectionPayload = new A_BYTE[dwInjectionSize];
			lpInjectionPayload[0] = 0xE9;
			*(A_LPDWORD)&lpInjectionPayload[1] = Helpers::Rel32_JmpOffset64(uInjectionPoint, lpTrampoline->uTrampolineAddress);

			for (A_I32 i = 5; i < dwInjectionSize; i++)
				lpInjectionPayload[i] = 0x90; // Padding with nops.

			A_ADDR64 uIndexedAddress = lpTrampoline->uTrampolineAddress;

			// Writes overwritten code if specified.

			if (bKeepOverwrittenCode) {
				if (!WriteMemory(uIndexedAddress, lpOriginalCode, dwInjectionSize)) AuroraThrow(Write64Exception, uIndexedAddress, dwInjectionSize);
				uIndexedAddress += dwInjectionSize;
			}

			// Writes injection code.

			if (!WriteMemory(uIndexedAddress, refInjectionCode.szBytes, refInjectionCode.size())) AuroraThrow(Write64Exception, uIndexedAddress, refInjectionCode.size());
			uIndexedAddress += refInjectionCode.size();

			// Gets and writes relative jmp code.

			A_BYTE szJmpBackCode[Helpers::c_dwRelJmp32Size] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
			*(A_LPDWORD)&szJmpBackCode[1] = Helpers::Rel32_JmpOffset64(uIndexedAddress, uInjectionPoint + Helpers::c_dwRelJmp32Size);

			if (!WriteMemory(uIndexedAddress, szJmpBackCode, sizeof(szJmpBackCode))) AuroraThrow(Write64Exception, uIndexedAddress, Helpers::c_dwRelJmp32Size);

			AuroraContextEnd();
		}

		CodeInjection64(const CodeInjection64&) = delete;

		~CodeInjection64();

		/// <summary>
		/// Enables the code injection.
		/// </summary>
		/// <exception cref="Write64Exception"/>
		A_VOID Enable();

		/// <summary>
		/// Disables the code injection.
		/// </summary>
		/// <exception cref="Write64Exception"/>
		A_VOID Disable();
	};


#ifdef _WIN64
	/// <summary>
	/// Used for injecting code at an address.
	/// </summary>
	using CodeInjection = CodeInjection64;
#else
	/// <summary>
	/// Used for injecting code at an address.
	/// </summary>
	using CodeInjection = CodeInjection32;
#endif
}

#endif // !__AURORA_CODE_INJECTION_H__