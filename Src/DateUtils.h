#if !defined(DATE_UTILS_H_)
#define DATE_UTILS_H_
//
//    $VER: DateUtils.h 1.0 (10.6.2002)
//
//    Various date functions.
//
//==============================================================================
//
#include <time.h>

/*-------------------------------------------------------------------------------
*  Types
*/
enum TDayOfWeek {
   eDowMonday    = 0,
   eDowTuesday   = 1,
   eDowWednesday = 2,
   eDowThursday  = 3,
   eDowFriday    = 4,
   eDowSaturday  = 5,
   eDowSunday    = 6
};

/*-------------------------------------------------------------------------------
*  Functions
*/
bool IsLeapYear (int year);
int  DaysInMonth(int month, int year);

TDayOfWeek DayOfWeek(int day, int month, int year);   // in range [0..6]
int        DayOfYear(int day, int month, int year);   // in range [0..365]

void GetEasterOfYear(int year, int &rDay, int &rMonth);

void IncDate(int &rDay, int &rMonth, int &rYear, int nDays);
void DecDate(int &rDay, int &rMonth, int &rYear, int nDays);

void IncDate1(int &rDay, int &rMonth, int &rYear);
void DecDate1(int &rDay, int &rMonth, int &rYear);

#endif //DATE_UTILS_H_
