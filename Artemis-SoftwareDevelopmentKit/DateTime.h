//-------------------------------------------------------------------------------------->
// Copyright (c) 2022 Artemis Group														|
// This file is licensed under the MIT license.											|
// Read more here: https://github.com/ArtemisDevGroup/Artemis/blob/master/LICENSE.md	|
//-------------------------------------------------------------------------------------->
// This file was authored by @Sigma0014.												|
// @Sigma0014: https://github.com/Sigma0014												|
//-------------------------------------------------------------------------------------->

#ifndef __DATETIME_H__
#define __DATETIME_H__

#include "Definitions.h"
#include <Windows.h>

namespace Artemis {
	class ARTEMIS_API DateTime {
		static DateTime ProcessTimeData(_In_ LPSYSTEMTIME lpSystemTime);

	public:
		static _Check_return_ DateTime Now();
		static _Check_return_ DateTime NowUTC();

		WORD wMilliseconds;
		WORD wSecond;
		WORD wMinute;
		WORD wHour;
		WORD wDay;
		WORD wDayOfWeek;
		CHAR szDayOfWeek[MAX_NAME];
		WORD wMonth;
		WORD wYear;
	};
}

#endif // !__DATETIME_H__