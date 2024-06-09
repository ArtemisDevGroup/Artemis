#ifndef ARTEMIS_API_MEMORY_HXX
#define ARTEMIS_API_MEMORY_HXX

#include "Definitions.hxx"
#include "Error.hxx"

#include <Windows.h>

#include <array>
#include <vector>
#include <string>

#define _In_addr_ _In_range_(0x0000000000010000, 0x00007FFFFFFEFFFF)

namespace Artemis::API {
	using ADDRESS = unsigned __int64;

	namespace RT {
		using MemoryCopyRoutine = BOOL(*)(_In_ SIZE_T cb, _Out_writes_bytes_(cb) LPVOID lpDestination, _In_reads_bytes_(cb) LPCVOID lpSource);

		_Success_(return) ARTEMIS_API BOOL MemoryCopy_Unsafe(
			_In_ SIZE_T cb,
			_Out_writes_bytes_(cb) LPVOID lpDestination,
			_In_reads_bytes_(cb) LPCVOID lpSource
		);

		_Success_(return) ARTEMIS_API BOOL MemoryCopy_SEH(
			_In_ SIZE_T cb,
			_Out_writes_bytes_(cb) LPVOID lpDestination,
			_In_reads_bytes_(cb) LPCVOID lpSource
		);
	}

	namespace Impl {
		template<typename T>
		T Read(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_addr_ ADDRESS uAddress
		) {
			T ret;
			if (!lpfnMemoryCopy(sizeof(T), &ret, (LPCVOID)uAddress))
				return T();

			SetLastArtemisError(__FUNCTION__, ErrorCode::Success);
			return ret;
		}

		template<typename T>
		_Success_(return) BOOL Read(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_addr_ ADDRESS uAddress,
			_Out_writes_(sizeof(T)) T* const lpReturn
		) {
			if (!lpReturn) {
				SetLastArtemisError(__FUNCTION__, ErrorCode::ParameterNull);
				return FALSE;
			}

			return lpfnMemoryCopy(sizeof(T), lpReturn, (LPCVOID)uAddress);
		}

		template<typename T>
		_Success_(return) BOOL ReadArray(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_addr_ ADDRESS uAddress,
			_Out_writes_(uCount) T* const lpReturn,
			_In_ SIZE_T uCount
		) {
			if (!lpReturn) {
				SetLastArtemisError(__FUNCTION__, ErrorCode::ParameterNull);
				return FALSE;
			}

			return lpfnMemoryCopy(uCount * sizeof(T), lpReturn, (LPCVOID)uAddress);
		}

		template<typename T, int nCount>
		_Success_(return) BOOL ReadArray(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_addr_ ADDRESS uAddress,
			_Out_writes_(uCount) T(&lpReturn)[nCount]
		) {
			return ReadArray(lpfnMemoryCopy, uAddress, lpReturn, nCount);
		}

		template<typename T, int nCount>
		_Success_(return) BOOL ReadArray<std::array<T, nCount>>(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_addr_ ADDRESS uAddress,
			_Out_writes_(nCount) std::array<T, nCount>* const lpReturn
		) {
			if (!lpReturn) {
				SetLastArtemisError(__FUNCTION__, ErrorCode::ParameterNull);
				return FALSE;
			}

			return lpfnMemoryCopy(sizeof(T) * nCount, lpReturn->data(), (LPCVOID)uAddress);
		}

		template<typename T, int nCount>
		std::array<T, nCount> ReadArray(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_addr_ ADDRESS uAddress
		) {
			std::array<T, nCount> ret;
			if (!ReadArray(lpfnMemoryCopy, uAddress, &ret))
				return std::array<T, nCount>();
			else
				SetLastArtemisError(__FUNCTION__, ErrorCode::Success);
			return ret;
		}

		template<typename T>
		_Success_(return) BOOL ReadArray<std::vector<T>>(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_addr_ ADDRESS uAddress,
			_Out_writes_(uCount) std::vector<T>* const lpReturn,
			_In_ SIZE_T uCount
		) {
			if (!lpReturn) {
				SetLastArtemisError(__FUNCTION__, ErrorCode::ParameterNull);
				return FALSE;
			}

			lpReturn->clear();
			lpReturn->reserve(uCount);
			
			return lpfnMemoryCopy(uCount * sizeof(T), lpReturn->data(), (LPCSTR)uAddress);
		}

		template<typename T>
		std::vector<T> ReadArray(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_addr_ ADDRESS uAddress,
			_In_ SIZE_T uCount
		) {
			std::vector<T> ret;
			if (!ReadArray(lpfnMemoryCopy, uAddress, &ret, uCount))
				return std::vector<T>();
			else
				SetLastArtemisError(__FUNCTION__, ErrorCode::Success);
			return ret;
		}

		_Success_(return) BOOL ReadString(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_ ADDRESS uAddress,
			_Out_writes_z_(uCount) LPSTR lpReturn,
			_In_ SIZE_T uCount
		);

		template<int nCount>
		_Success_(return) BOOL ReadString(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_ ADDRESS uAddress,
			_Out_writes_z_(nCount) CHAR(&lpReturn)[nCount]
		) {
			return ReadString(lpfnMemoryCopy, uAddress, lpReturn, nCount);
		}

		_Success_(return) BOOL ReadString(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_ ADDRESS uAddress,
			_Out_writes_z_(uMaxCount) std::string* const lpReturn,
			_In_ SIZE_T uMaxCount
		);

		std::string ReadString(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_ ADDRESS uAddress,
			_In_ SIZE_T uMaxCount
		);

		_Success_(return) BOOL ReadWideString(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_ ADDRESS uAddress,
			_Out_writes_z_(uCount) LPWSTR lpReturn,
			_In_ SIZE_T uCount
		);

		template<int nCount>
		_Success_(return) BOOL ReadWideString(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_ ADDRESS uAddress,
			_Out_writes_z_(nCount) WCHAR(&lpReturn)[nCount]
		) {	return ReadWideString(lpfnMemoryCopy, uAddress, lpReturn, nCount); }

		_Success_(return) BOOL ReadWideString(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_ ADDRESS uAddress,
			_Out_writes_z_(uMaxCount) std::wstring* const lpReturn,
			_In_ SIZE_T uMaxCount
		);

		std::wstring ReadWideString(
			_In_ RT::MemoryCopyRoutine lpfnMemoryCopy,
			_In_ ADDRESS uAddress,
			_In_ SIZE_T uMaxCount
		);
	}

	template<typename T>
	T Read(_In_addr_ ADDRESS uAddress) { return Impl::Read<T>(RT::MemoryCopy_Unsafe, uAddress); }

	template<typename T>
	_Success_(return) BOOL Read(
		_In_addr_ ADDRESS uAddress,
		_Out_writes_(sizeof(T)) T* const lpReturn
	) { return Impl::Read<T>(RT::MemoryCopy_Unsafe, uAddress, lpReturn); }

	template<typename T>
	_Success_(return) BOOL ReadArray(
		_In_addr_ ADDRESS uAddress,
		_Out_writes_(uCount) T* const lpReturn,
		_In_ SIZE_T uCount
	) { return Impl::ReadArray<T>(RT::MemoryCopy_Unsafe, uAddress, lpReturn, uCount); }

	template<typename T, int nCount>
	_Success_(return) BOOL ReadArray(
		_In_addr_ ADDRESS uAddress,
		_Out_writes_(uCount) T(&lpReturn)[nCount]
	) { return Impl::ReadArray<T, nCount>(RT::MemoryCopy_Unsafe, uAddress, lpReturn); }

	template<typename T, int nCount>
	_Success_(return) BOOL ReadArray<std::array<T, nCount>>(
		_In_addr_ ADDRESS uAddress,
		_Out_writes_(nCount) std::array<T, nCount>* const lpReturn
	) { return Impl::ReadArray<T, nCount>(RT::MemoryCopy_Unsafe, uAddress, lpReturn); }

	template<typename T, int nCount>
	std::array<T, nCount> ReadArray(_In_addr_ ADDRESS uAddress) { return Impl::ReadArray<T, nCount>(RT::MemoryCopy_Unsafe, uAddress); }

	template<typename T>
	_Success_(return) BOOL ReadArray(
		_In_addr_ ADDRESS uAddress,
		_Out_writes_(uCount) std::vector<T>* const lpReturn,
		_In_ SIZE_T uCount
	) {	return Impl::ReadArray<T>(RT::MemoryCopy_Unsafe, uAddress, lpReturn, uCount); }

	template<typename T>
	std::vector<T> ReadArray(
		_In_addr_ ADDRESS uAddress,
		_In_ SIZE_T uCount
	) { return Impl::ReadArray<T>(RT::MemoryCopy_Unsafe, uAddress, uCount); }

	namespace SEH {

	}
}

#endif // !ARTEMIS_API_MEMORY_HXX