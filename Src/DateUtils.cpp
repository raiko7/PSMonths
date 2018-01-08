//
//    $VER: DateUtils.cpp 1.0 (10.6.2002)
//
//    various date functions
//
//==============================================================================
//
#include "DateUtils.h"

#include <stdlib.h>
#include <string.h>

/*-------------------------------------------------------------------------------
*  Constants
*/
static const int cMonthsPerYear = 12;

/*-------------------------------------------------------------------------------
*  Functions
*/
/******************************************************************************/
/**  leap year test
*
*  @param[in] year   year to test
*
*  @return true if \em year is a leap year, else false
*/
bool IsLeapYear(int year) {

   return ((year & 3) == 0) && ((year % 100 != 0) || (year % 400 == 0));
}

/******************************************************************************/
/**  returns number of days in a month in a given year
*
*  @param[in] month   month from 1 to 12
*  @param[in] year    year context
*
*  @return number of days
*/
int DaysInMonth(int month, int year) {
   
   if (month==4 || month==6 || month==9 || month==11) {
      return 30;
   } else if (month != 2) {
      return 31;
   } else if (IsLeapYear(year)) {
      return 29;
   } else {
      return 28;
   }
}

/******************************************************************************/
/**  returns the week day for a given date
*
*  @param[in] day     day from 1 to 31
*  @param[in] month   month from 1 to 12
*  @param[in] year    year
*
*  @return week day from \em eDowMonday to \em eDowSunday
*/
TDayOfWeek DayOfWeek(int day, int month, int year) {

   int dayOfWeek; 

   // January and February count as months 13 and 14 of previous year
   if (month==1 || month==2) {
      month += 12;
      --year;
   }
   // universal formula from Kim S. Larsen (Dr Dobb's Journal, April 1995)
   dayOfWeek = (day + 2*month + 3*(month+1)/5 +
      year + year/4 - year/100 + year/400) % 7;

   return (TDayOfWeek)dayOfWeek;
}

/******************************************************************************/
/**  returns the day of year for a given date
*
*  @param[in] day     day from 1 to 31
*  @param[in] month   month from 1 to 12
*  @param[in] year    year
*
*  @return day of year from 0 to 365
*/
int DayOfYear(int day, int month, int year) {
   
   int days, i;
   
   days = -1;
   for (i=1; i<month; i++) {
      days += DaysInMonth(i, year);
   }
   return days + day;
}

/******************************************************************************/
/**  computes the date of easter for a year from 1970 to 2037
*
*  @param[in]  year     year
*  @param[out] rDay     computed day of easter
*  @param[out] rMonth   computed month of easter
*/
void GetEasterOfYear(int year, int &rDay, int &rMonth) {

   int corr = 0;

   if (year < 1700)      corr = 4;
   else if (year < 1800) corr = 5;
   else if (year < 1900) corr = 6;
   else if (year < 2100) corr = 0;
   else if (year < 2200) corr = 1;
   else if (year < 2300) corr = 2;
   else if (year < 2500) corr = 3;

   rDay = (19 * (year % 19) + 24) % 30;        
   rDay = 22 + rDay + ((2 * (year % 4) + 4 * (year % 7) + 6 * rDay + 5 + corr) % 7);

   /* April on overflow, else March */
   if (rDay > 31) {
      rMonth = 4;
      rDay -= 31;
   } else {
      rMonth = 3;
   }
}

/******************************************************************************/
/**  increments a date by a number of days
*
*  @param[in,out]  rDay     day component of date
*  @param[in,out]  rMonth   month component of date
*  @param[in,out]  rYear    year component of date
*  @param[in]      nDays    number of days (positive)
*/
void IncDate(int &rDay, int &rMonth, int &rYear, int nDays) {

   int diff;

   while (nDays > 0) {
      diff = DaysInMonth(rMonth, rYear) - rDay + 1;
      if (nDays >= diff) {
         rDay = 1;
         ++rMonth;
         if (rMonth > cMonthsPerYear) {
            rMonth = 1;
            ++rYear;
         }
      } else {
         rDay += nDays;
      }
      nDays -= diff;
   }
}

/******************************************************************************/
/**  decrements a date by a number of days
*
*  @param[in,out]  rDay     day component of date
*  @param[in,out]  rMonth   month component of date
*  @param[in,out]  rYear    year component of date
*  @param[in]      nDays    number of days (positive)
*/
void DecDate(int &rDay, int &rMonth, int &rYear, int nDays) {

   int diff;

   while (nDays > 0) {
      diff = rDay;
      if (nDays >= rDay) {
         --rMonth;
         if (rMonth < 1) {
            rMonth = cMonthsPerYear;
            --rYear;
         }
         rDay = DaysInMonth(rMonth, rYear);
      } else {
         rDay -= nDays;
      }
      nDays -= diff;
   }
}

/******************************************************************************/
/**  increments a date by one day
*
*  @param[in,out]  rDay     day component of date
*  @param[in,out]  rMonth   month component of date
*  @param[in,out]  rYear    year component of date
*/
void IncDate1(int &rDay, int &rMonth, int &rYear) {

   ++rDay;
   if (rDay > DaysInMonth(rMonth, rYear)) {
      rDay = 1;
      ++rMonth;
      if (rMonth > cMonthsPerYear) {
         rMonth = 1;
         ++rYear;
      }
   }
}

/******************************************************************************/
/**  decrements a date by one day
*
*  @param[in,out]  rDay     day component of date
*  @param[in,out]  rMonth   month component of date
*  @param[in,out]  rYear    year component of date
*/
void DecDate1(int &rDay, int &rMonth, int &rYear) {

   --rDay;
   if (rDay < 1) {
      --rMonth;
      if (rMonth < 1) {
         rMonth = cMonthsPerYear;
         --rYear;
      }
      rDay = DaysInMonth(rMonth, rYear);
   }
}
