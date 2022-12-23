//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#include "Configuration.h"

#include <Windows.h>

bool strcmp(
	_In_z_ const char* _Str1,
	_In_z_ const char* _Str2,
	_In_ int _MaxCharacters
) {
	size_t _Length1 = strlen(_Str1), _Length2 = strlen(_Str2);
	int _Limiter = (_Length1 > _MaxCharacters) && (_Length2 > _MaxCharacters) ? _MaxCharacters : (_Length1 > _Length2 ? _Length2 : _Length1);

	for (int i = 0; i < _Limiter; i++)
		if (_Str1[i] != _Str2[i])
			return false;

	return true;
}

int GetIndexOfChar(_In_z_ LPCSTR lpString, _In_ CHAR c, _In_ INT nStartIndex = 0) {
	for (INT i = nStartIndex; i < strlen(lpString); i++)
		if (lpString[i] == c) return i;
	return -1;
}

bool StringIsHexNumeral(_In_z_ LPCSTR lpNumeral) {
	for (INT i = 0; i < strlen(lpNumeral); i++)
		if (!IS_DIGIT(lpNumeral[i]) && !IN_RANGE(lpNumeral[i], 'A', 'F')) return false;

	return true;
}

bool IsProperty(_In_z_ LPCSTR lpProperty, _In_opt_z_ LPCSTR lpTargetPropertyName) {
	if (lpTargetPropertyName)
		return strcmp(lpProperty, lpTargetPropertyName, GetIndexOfChar(lpProperty, '='));
	else {
		int nIndex = GetIndexOfChar(lpProperty, '=');
		return nIndex != -1 ? StringIsHexNumeral(lpProperty + nIndex + 1) : false;
	}
}

bool IsSection(_In_z_ LPCSTR lpSection, _In_opt_z_ LPCSTR lpTargetSectionName) {
	return strcmp(lpSection, "#section", 8) && (lpTargetSectionName ? strcmp(lpSection + 10, lpTargetSectionName, strlen(lpSection) - 10 - 2) : true);
}

void MoveCharacters(
	_In_z_ LPSTR lpBuffer,
	_In_ INT nOldIndex,
	_In_ INT nNewIndex,
	_In_ INT nMoveCount = -1
) {
	CONTEXT_BEGIN;

	INT nSize = lstrlenA(lpBuffer);

	if (nOldIndex > nNewIndex) {
		bool bMoveNullTerminator = nMoveCount == -1;

		if (nMoveCount == -1) nMoveCount = nSize - nOldIndex;
		if (nOldIndex + nMoveCount > nSize) throw Artemis::ParameterException("nMoveCount");

		for (INT i = 0; i < nMoveCount; i++) {
			lpBuffer[nNewIndex + i] = lpBuffer[nOldIndex + i];
		}
		if (bMoveNullTerminator) lpBuffer[nNewIndex + nMoveCount] = '\0';
	}

	CONTEXT_END;
}

namespace Artemis {
	//----------------------------------------->ConfigurationHelper<-----------------------------------------------------
	void ConfigurationHelper::OpenFile(_In_ INT nMode) {
		CONTEXT_BEGIN;

		CHAR szMode[3] = { '\0', '\0', '\0' };

		if (nMode == Update) throw ParameterException("nMode");

		if (nMode & Read) szMode[0] = 'r';
		else if (nMode & Write) szMode[0] = 'w';
		else if (nMode & Append) szMode[0] = 'a';
		else throw ParameterException("nMode");

		if (nMode & Update) szMode[1] = '+';

		if (fopen_s(&pFile, szConfigFile, szMode)) throw Exception("fopen_s failed for unknown reason.", ExceptionCode::Unknown);

		CONTEXT_END;
	}

	void ConfigurationHelper::CloseFile() {
		if (pFile) {
			fclose(pFile);
			pFile = nullptr;
		}
	}

	//----------------------------------------->ConfigurationSection<-----------------------------------------------------
	ConfigurationSection::ConfigurationSection() : nPos(0) {}
	ConfigurationSection::ConfigurationSection(_In_z_ LPCSTR lpConfigFile, _In_ fpos_t nPosition) : nPos(nPosition) { strcpy_s(szConfigFile, lpConfigFile); }

	_Check_return_ INT ConfigurationSection::GetPropertyValue(_In_z_ LPCSTR lpPropertyName) {
		CONTEXT_BEGIN;

		CHAR szBuffer[64] = { '\0' };
		INT nReturn = -1;
		bool bReturn = false;

		OpenFile(Read);
		fseek(pFile, nPos, SEEK_SET);

		fscanf_s(pFile, "%s", szBuffer, (UINT)sizeof(szBuffer));
		while (szBuffer[0] != '}') {
			if (IsProperty(szBuffer, lpPropertyName)) {
				sscanf_s(szBuffer + GetIndexOfChar(szBuffer, '=') + 1, "%X", &nReturn);
				bReturn = true;
				break;
			}

			fscanf_s(pFile, "%s", szBuffer, (UINT)sizeof(szBuffer));
		}
		CloseFile();

		if (!bReturn) throw ObjectNotFoundException(lpPropertyName, "ConfigurationSegment property");

		CONTEXT_END;
		return nReturn;
	}

	void ConfigurationSection::SetPropertyValue(_In_z_ LPCSTR lpPropertyName, _In_ INT nValue) {
		CONTEXT_BEGIN;

		CHAR szBuffer[64] = { '\0' };
		bool bSuccess = false;

		OpenFile(Read | Update);
		fseek(pFile, nPos, SEEK_SET);

		fscanf_s(pFile, "%s", szBuffer, (UINT)sizeof(szBuffer));
		while (szBuffer[0] != '}') {
			if (IsProperty(szBuffer, lpPropertyName)) {
				fseek(pFile, -8 + 1, SEEK_CUR);

				fprintf(pFile, "%0*X", 8, nValue);
				bSuccess = true;
				break;
			}

			fscanf_s(pFile, "%s", szBuffer, (UINT)sizeof(szBuffer));
		}
		CloseFile();

		if (!bSuccess) throw ObjectNotFoundException(lpPropertyName, "ConfigurationSegment property");

		CONTEXT_END;
	}

	void ConfigurationSection::AddProperty(_In_z_ LPCSTR lpPropertyName, _In_ INT nValue) {
		try {
			GetPropertyValue(lpPropertyName);
		}
		catch (ObjectNotFoundException& e) {
			CONTEXT_BEGIN;
			throw ParameterException("lpPropertyName");
		}
		CONTEXT_BEGIN;

		CHAR szBuffer[64] = { '\0' };
		bool bFailed = false;

		OpenFile(Read | Update);
		fseek(pFile, nPos, SEEK_SET);

		do {
			if (fscanf_s(pFile, "%s", szBuffer, (UINT)sizeof(szBuffer)) == EOF) {
				bFailed = true;
				break;
			}
		} while (szBuffer[0] != '}');

		if (!bFailed) {
			fseek(pFile, -1, SEEK_CUR);
			fprintf(pFile, "%s=%0*X\n}", lpPropertyName, 8, nValue);
		}

		CloseFile();

		if (bFailed) throw InstanceInvalidException();

		CONTEXT_END;
	}

	void ConfigurationSection::RemoveProperty(_In_z_ LPCSTR lpPropertyName) {
		CONTEXT_BEGIN;

		OpenFile(Read);
		fseek(pFile, 0, SEEK_END);

		fpos_t nSize;
		fgetpos(pFile, &nSize);

		fseek(pFile, 0, SEEK_SET);

		LPSTR lpString = new CHAR[nSize + 1];
		memset(lpString, 0x00, nSize + 1);
		fread(lpString, sizeof(CHAR), nSize, pFile);

		CloseFile();

		INT nPropertyPos = -1;
		INT nPropertyNameLength = lstrlenA(lpPropertyName);
		for (INT i = nPos; lpString[i] != '}'; i++) {
			if (strcmp(&lpString[i], lpPropertyName, nPropertyNameLength)) {
				nPropertyPos = i;
				break;
			}
		}

		if (nPropertyPos == -1) throw ObjectNotFoundException(lpPropertyName, "ConfigurationSection property");

		MoveCharacters(
			lpString,
			nPropertyPos + nPropertyNameLength + 1 + 8 + 1, // Current pos + property name length + equals + 8 digits + newline.
			nPropertyPos
		);

		OpenFile(Write);

		fprintf(pFile, "%s", lpString);

		CloseFile();

		delete[] lpString;

		CONTEXT_END;
	}

	//----------------------------------------->Configuration<-----------------------------------------------------
	Configuration::Configuration(_In_z_ LPCSTR lpConfigFile) {
		strcpy_s(szConfigFile, lpConfigFile);
		if (fopen_s(&pFile, lpConfigFile, "r"))
			fopen_s(&pFile, lpConfigFile, "w");
		if (pFile) fclose(pFile);
	}

	bool Configuration::ValidateFile(_In_z_ LPCSTR lpConfigFile) {
		CONTEXT_BEGIN;

		FILE* pFile;
		if (fopen_s(&pFile, lpConfigFile, "r")) throw Exception("fopen_s failed for unknown reason.", ExceptionCode::Unknown);

		bool bValid = true;

		CHAR szBuffer[64] = { '\0' };
		while (fscanf_s(pFile, "%s", szBuffer, (int)sizeof(szBuffer)) != EOF) {
			if (
				IsSection(szBuffer, nullptr) ||
				IsProperty(szBuffer, nullptr) ||
				szBuffer[0] == '{' || szBuffer[0] == '}'
				) continue;
			else {
				bValid = false;
				break;
			}
		}

		fclose(pFile);

		CONTEXT_END;
		return bValid;
	}

	_Check_return_ ConfigurationSection Configuration::GetSection(_In_z_ LPCSTR lpSectionName) {
		CONTEXT_BEGIN;

		CHAR szBuffer[64] = { '\0' };
		ConfigurationSection Return;
		bool bReturn = false;

		OpenFile(Read);

		while (fscanf_s(pFile, "%s", szBuffer, (UINT)sizeof(szBuffer)) != EOF) {
			if (IsSection(szBuffer, lpSectionName)) {
				fpos_t nPos;
				fgetpos(pFile, &nPos);

				Return = ConfigurationSection(szConfigFile, nPos + 2);
				bReturn = true;
				break;
			}
		}
		CloseFile();

		if (!bReturn) throw ObjectNotFoundException(lpSectionName, "ConfigurationSection");

		CONTEXT_END;
		return Return;
	}

	ConfigurationSection Configuration::AddSection(_In_z_ LPCSTR lpSectionName) {
		CONTEXT_BEGIN;
		try {
			GetSection(lpSectionName);
			throw ParameterException("lpSectionName");
		}
		catch (ObjectNotFoundException& e) {}
		CONTEXT_BEGIN;

		OpenFile(Append);

		fprintf(pFile, "#section(\"%s\") {\n}\n", lpSectionName);

		CloseFile();

		CONTEXT_END;
		return GetSection(lpSectionName);
	}

	void Configuration::RemoveSection(_In_z_ LPCSTR lpSectionName) {
		CONTEXT_BEGIN;

		OpenFile(Read);
		fseek(pFile, 0, SEEK_END);

		fpos_t nSize;
		fgetpos(pFile, &nSize);

		fseek(pFile, 0, SEEK_SET);

		LPSTR lpString = new CHAR[nSize + 1];
		memset(lpString, 0x00, nSize + 1);
		fread(lpString, sizeof(CHAR), nSize, pFile);

		CloseFile();

		INT nSectionPos = -1;
		for (INT i = 0; i < strlen(lpString); i++) {
			if (IsSection(&lpString[i], lpSectionName)) {
				nSectionPos = i;
				break;
			}
		}

		if (nSectionPos == -1) throw ObjectNotFoundException(lpSectionName, "ConfigurationSection");

		MoveCharacters(
			lpString,
			GetIndexOfChar(lpString, '}', nSectionPos) + 1,
			nSectionPos - 1
		);

		OpenFile(Write);

		fprintf(pFile, "%s", lpString);

		CloseFile();

		delete[] lpString;

		CONTEXT_END;
	}
}