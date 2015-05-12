/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"
#include "Common.h"

namespace vega
{
	typedef	char	string16[16];
	typedef	char	string256[256];
	// computing build id
	const char* build_date;
	int build_id;


	static const char* month_id[12] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};

	static int days_in_month[12] = {
		31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};

	static int start_day = 7;	// 7
	static int start_month = 6;	// Jun
	static int start_year = 2010;	// 2010

	void ComputeBuildId()
	{
		build_date = __DATE__;

		int					days;
		int					months = 0;
		int					years;
		string16			month;
		string256			buffer;
		strcpy(buffer, __DATE__);
		sscanf(buffer, "%s %d %d", month, &days, &years);

		for (int i = 0; i < 12; i++) {
			if (stricmp(month_id[i], month))
				continue;

			months = i;
			break;
		}

		build_id = (years - start_year) * 365 + days - start_day;

		for (int i = 0; i < months; ++i)
			build_id += days_in_month[i];

		for (int i = 0; i < start_month - 1; ++i)
			build_id -= days_in_month[i];
	}
}