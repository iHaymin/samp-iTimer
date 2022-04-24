/* iTimer Make By iToKcik */
/* Credit To arYzal For Helping */

/*

Functions:

    OnTimeUpdate();
    OnPlayerTimeUpdate(playerid);
    TimestampToDate(Timestamp, &year, &month, &day, &hour, &minute, &second, HourGMT, MinuteGMT = 0);
    DateToTimestamp(str[11]);
    GetElapsedTime(time, to_time, type = CONVERT_TIME_TO_MINUTES);

Created On : 24/04/2022

*/

//==================================================================================
//Convert
#define CONVERT_TIME_TO_SECONDS 	1
#define CONVERT_TIME_TO_MINUTES 	2
#define CONVERT_TIME_TO_HOURS 		3
#define CONVERT_TIME_TO_DAYS 		4
#define CONVERT_TIME_TO_MONTHS 		5
#define CONVERT_TIME_TO_YEARS 		6
//==================================================================================
//Seconds
#define SECONDS_DAYS                86400
#define SECONDS_HOURS               3600
#define SECONDS_MINUTES             60
//==================================================================================
//New
new PlayerTimer[MAX_PLAYERS], CountTimer[3];
//==================================================================================
//Array
new MonthTimes[12][4] =
{
	{ 31, 31, 2678400, 2678400 },
	{ 28, 29, 2419200, 2505600 },
	{ 31, 31, 2678400, 2678400 },
	{ 30, 30, 2592000, 2592000 },
	{ 31, 31, 2678400, 2678400 },
	{ 30, 30, 2592000, 2592000 },
	{ 31, 31, 2678400, 2678400 },
	{ 31, 31, 2678400, 2678400 },
	{ 30, 30, 2592000, 2592000 },
	{ 31, 31, 2678400, 2678400 },
	{ 30, 30, 2592000, 2592000 },
	{ 31, 31, 2678400, 2678400 }
};
//==================================================================================
//Callback
public OnPlayerConnect(playerid)
{
    if(funcidx("OnPlayerTimeUpdate") != -1)
    {
		PlayerTimer[playerid] = SetTimerEx("OnPlayerTimeUpdate", (1* 1000), true, "i", playerid);
	}
	else
	{
		PlayerTimer[playerid] = -1;
	}
	return CallLocalFunction("TIMER_OnPlayerConnect", "i", playerid);
}

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect TIMER_OnPlayerConnect
forward TIMER_OnPlayerConnect(playerid);

public OnPlayerDisconnect(playerid, reason)
{
    KillTimer(PlayerTimer[playerid]);
	return CallLocalFunction("TIMER_OnPlayerDisconnect", "ii", playerid, reason);
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect TIMER_OnPlayerDisconnect
forward TIMER_OnPlayerDisconnect(playerid, reason);
forward OnPlayerTimeUpdate(playerid);
forward OnTimeUpdate();
public OnTimeUpdate()
{
    CountTimer[2] += 1;
    if(CountTimer[2] >= 60)
	{
	    CountTimer[2] = 0;
	    CountTimer[1] += 1;

	    if(CountTimer[1] >= 60)
	    {
	        CountTimer[1] = 0;
	        CountTimer[0] += 1;
	    }
	}
	return CallLocalFunction("TIMER_OnTimeUpdate", "");
}
#if defined _ALS_OnTimeUpdate
	#undef OnTimeUpdate
#else
	#define _ALS_OnTimeUpdate
#endif
#define OnTimeUpdate TIMER_OnTimeUpdate
forward TIMER_OnTimeUpdate(hours, minutes, seconds);
//==================================================================================
//Stock
stock IsLeapYear(year)
{
	if(year % 4 == 0) return 1;
	else return 0;
}

stock TimestampToDate(Timestamp, &year, &month, &day, &hour, &minute, &second, HourGMT, MinuteGMT = 0)
{
	new tmp = 2;
	year = 1970;
	month = 1;
	Timestamp -= 172800; 
	for(;;)
	{
		if(Timestamp >= 31536000)
		{
			year ++;
			Timestamp -= 31536000;
			tmp ++;
			if(tmp == 4)
			{
				if(Timestamp >= 31622400)
				{
					tmp = 0;
					year ++;
					Timestamp -= 31622400;
				}
				else break;
			}
		}
		else break;
	}
	for(new i = 0; i < 12; i ++)
	{
		if(Timestamp >= MonthTimes[i][2 + IsLeapYear(year)])
		{
			month ++;
			Timestamp -= MonthTimes[i][2 + IsLeapYear(year)];
		}
		else break;
	}
	day = 1 + (Timestamp / 86400);
	Timestamp %= 86400;
	hour = HourGMT + (Timestamp / 3600);
	Timestamp %= 3600;
	minute = MinuteGMT + (Timestamp / 60);
	second = (Timestamp % 60);
	if(minute > 59)
	{
		minute = 0;
		hour ++;
	}
	if(hour > 23)
	{
		hour -= 24;
		day ++;
	}
	if(day > MonthTimes[month][IsLeapYear(year)])
	{
		day = 1;
		month ++;
	}
	if(month > 12)
	{
		month = 1;
		year ++;
	}
	return 1;
}

stock DateToTimestamp(str[11])
{
	new date[3];
	if(!sscanf(str, "p<"#SPLITTER">ddd", date[0], date[1], date[2]))
	{
		new total = 0, tmp = 0;
		total += date[0] * 86400;
		if(date[1] == 2 && date[0] < 29) tmp = ((date[2] - 1968) / 4 - 2);
		else tmp = ((date[2] - 1968) / 4 - 1);
		total += tmp * 31622400;
		total += (date[2] - 1970 - tmp) * 31536000;
		for(new i = 1; i < date[1]; i ++) total += MonthTimes[i][0 + IsLeapYear(date[2])] * 86400;
		return total;
	}
	else return -1;
}

stock GetElapsedTime(time, to_time, type = CONVERT_TIME_TO_MINUTES)
{
	new result;
	switch(type)
	{
		case CONVERT_TIME_TO_MINUTES:
		{
			result = ((time - (time % 60)) - (to_time - (to_time % 60))) / 60;
		}
		case CONVERT_TIME_TO_HOURS:
		{
			result = ((time - (time % 3600)) - (to_time - (to_time % 3600))) / 3600;
		}
		case CONVERT_TIME_TO_DAYS:
		{
			result = ((time - (time % 86400)) - (to_time - (to_time % 86400))) / 86400;
		}
		default:
			result = -1;
	}
	return result;
}
//==================================================================================
