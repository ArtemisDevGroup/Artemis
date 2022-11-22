#include "DateTime.h"

#include <Windows.h>

namespace Artemis {
	DateTime DateTime::ProcessTimeData(_In_ LPSYSTEMTIME lpSystemTime) {
		DateTime dt;
		dt.wYear = lpSystemTime->wYear;
		dt.wMonth = lpSystemTime->wMonth;
		dt.wDay = lpSystemTime->wDay;
		dt.wDayOfWeek = lpSystemTime->wDayOfWeek;

		switch (dt.wDayOfWeek) {
		case 1:
			strcpy_s(dt.szDayOfWeek, "Monday");
			break;
		case 2:
			strcpy_s(dt.szDayOfWeek, "Tuesday");
			break;
		case 3:
			strcpy_s(dt.szDayOfWeek, "Wednesday");
			break;
		case 4:
			strcpy_s(dt.szDayOfWeek, "Thursday");
			break;
		case 5:
			strcpy_s(dt.szDayOfWeek, "Friday");
			break;
		case 6:
			strcpy_s(dt.szDayOfWeek, "Saturday");
			break;
		case 7:
			strcpy_s(dt.szDayOfWeek, "Sunday");
			break;
		}

		dt.wHour = lpSystemTime->wHour;
		dt.wMinute = lpSystemTime->wMinute;
		dt.wSecond = lpSystemTime->wSecond;
		dt.wMilliseconds = lpSystemTime->wMilliseconds;

		return dt;
	}

	DateTime DateTime::Now() {
		SYSTEMTIME st;
		GetLocalTime(&st);
		return ProcessTimeData(&st);
	}

	DateTime DateTime::NowUTC() {
		SYSTEMTIME st;
		GetSystemTime(&st);
		return ProcessTimeData(&st);
	}
}