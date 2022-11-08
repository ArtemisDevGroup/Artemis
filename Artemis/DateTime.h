#ifndef __DATETIME_H__
#define __DATETIME_H__

#include "Definitions.h"
#include <Windows.h>

namespace Artemis {
	class DateTime {
		static DateTime ProcessTimeData(_In_ LPSYSTEMTIME lpSystemTime);

	public:
		static DateTime Now();
		static DateTime NowUTC();

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