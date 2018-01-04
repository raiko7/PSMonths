//
//    $VER: PSMonths.cpp 1.0 (01.10.06)
//
//    PostScript Month Calendar Generator, pre-defined holidays for Austria
//
//    (C) Copyright 2006-2017 by Rainer Koppler
//
//==============================================================================
//
#include "DateUtils.h"
#include "PSMonths.h"
#include "Formatter.h"

#include <iostream>
#include <fstream>
#include <utility>

//------------------------------------------------------------------------------
//  Constants
//
static const char sEscChars[] =
   "\xe4\xc4\xf6\xd6\xfc\xdc\xdf()\\";   // ä Ä ö Ö ü Ü ß ( ) \ 

static const char * const spUmlauts[] = {
   "\\212", "\\200", "\\232", "\\205", "\\237", "\\206", "\\247"
};

static const int cUmlautCnt = sizeof spUmlauts / sizeof spUmlauts[0];

//------------------------------------------------------------------------------
//  Variables
//
static TDayMap sDays;

//------------------------------------------------------------------------------
//  Functions
//

//------------------------------------------------------------------------------
//  ModifyUmlauts
//
static void ModifyUmlauts(std::string &rStr) {

   std::string::size_type pos1 = 0, pos2;
   int                    i;

   while ((pos2 = rStr.find_first_of(sEscChars, pos1)) != std::string::npos) {
      for (i=0; i<cUmlautCnt && rStr[pos2]!=sEscChars[i]; ++i)
         ;
      if (i < cUmlautCnt) {
         rStr.replace(pos2, 1, spUmlauts[i]);
         pos2 += 3;
      } else {
         rStr.insert(pos2, "\\");
         pos2 += 1;
      }
      pos1 = pos2 + 1;
   }
   
}

//------------------------------------------------------------------------------
//  InsertDay
//
static void InsertDay(const TDayKey &key, const TDayData &data) {

   auto result = sDays.insert(std::make_pair(key, data));

   if (!result.second) {
      // day already defined: append description, update highlighting
      result.first->second.mDescr.append(", ");
      result.first->second.mDescr.append(data.mDescr);
      if (data.mHighlighted) {
         result.first->second.mHighlighted = true;
      }
   }
}

//------------------------------------------------------------------------------
//  InsertHoliday
//
static void InsertHoliday(
   int         day,
   int         month,
   const char *pDescr,
   bool        highlighted
   ) {

   TDayKey  key;
   TDayData data;

   key.mDay   = day;
   key.mMonth = month;

   data.mDescr.assign(pDescr);
   data.mHighlighted = highlighted;
   ModifyUmlauts(data.mDescr);

   InsertDay(key, data);
}

//------------------------------------------------------------------------------
//  InsertHolidays
//
static void InsertHolidays(int year) {

   static struct {
      int         mMonth;
      int         mDay;
      const char *mpDescr;
      bool        mHighlighted;
   } sFixedDays[] = {
      {  1,  1, "Neujahr",           true  },
      {  1,  6, "Dreikönigsfest",    true  },
      {  5,  1, "Staatsfeiertag",    true  },
      {  8, 15, "Mariä Himmelfahrt", true  },
      { 10, 26, "Nationalfeiertag",  true  },
      { 11,  1, "Allerheiligen",     true  },
      { 12,  8, "Mariä Empfängnis",  true  },
      { 12, 24, "Heiligabend",       false },
      { 12, 25, "Christtag",         true  },
      { 12, 26, "Stefanitag",        true  },
      { 12, 31, "Silvester",         false },
      {  0,  0, 0,                   false }   // sentinel value
   };

   int i, month, day, wDay;
   char descr[10];

   // fixed holidays
   for (i=0; sFixedDays[i].mpDescr!=0; ++i) {
      InsertHoliday(sFixedDays[i].mDay, sFixedDays[i].mMonth,
         sFixedDays[i].mpDescr, sFixedDays[i].mHighlighted);
   }

   // movable holidays
   GetEasterOfYear(year, day, month);
   InsertHoliday(day, month, "Ostersonntag", true);
   DecDate(day, month, year, 2);
   InsertHoliday(day, month, "Karfreitag", false);
   DecDate(day, month, year, 44);
   InsertHoliday(day, month, "Aschermittwoch", false);
   IncDate(day, month, year, 47);
   InsertHoliday(day, month, "Ostermontag", true);
   IncDate(day, month, year, 38);
   InsertHoliday(day, month, "Christi Himmelfahrt", true);
   IncDate(day, month, year, 10);
   InsertHoliday(day, month, "Pfingstsonntag", true);
   IncDate1(day, month, year);
   InsertHoliday(day, month, "Pfingstmontag", true);
   IncDate(day, month, year, 10);
   InsertHoliday(day, month, "Fronleichnam", true);

   day   = 24;
   month = 12;
   wDay = DayOfWeek(day, month, year);
   if (wDay != eDowSunday) {
      DecDate(day, month, year, wDay + 1);
   }
   for (i=4; i!=0; --i) {
      snprintf(descr, sizeof descr, "%d. Advent", i);
      InsertHoliday(day, month, descr, true);
      DecDate(day, month, year, 7);
   }

   day   = 1;
   month = 5;
   wDay = DayOfWeek(day, month, year);
   IncDate(day, month, year, 13 - wDay);   // 2nd sunday of month
   InsertHoliday(day, month, "Muttertag", true);

   day   = 1;
   month = 6;
   wDay = DayOfWeek(day, month, year);
   IncDate(day, month, year, 13 - wDay);   // 2nd sunday of month
   InsertHoliday(day, month, "Vatertag", true);
}

//------------------------------------------------------------------------------
//  ParseCfg
//
static int ParseCfg(std::ifstream &rCfg, bool withEvents) {

   int         year = 0;
   std::string inpLine, str;
   TDayKey     key;
   TDayData    data;

   while (!rCfg.fail()) {
      std::getline(rCfg, inpLine);
      if (rCfg.fail()) break;

      if (inpLine.size() != 0 && inpLine[0] != '#') {   // no comment line

         if (inpLine[0] == '$') {   // year specification

            year = atoi(inpLine.substr(1).c_str());
            InsertHolidays(year);

         } else if (withEvents) {   // event specification

            std::string::size_type pos1 = 0, pos2;

            // parse month of year [1:12]
            pos2 = inpLine.find(':', pos1);
            if (pos2 == std::string::npos) continue;
            key.mMonth = atoi(inpLine.substr(pos1, pos2 - pos1).c_str());
            pos1 = pos2 + 1;

            // parse day of month [1:31]
            pos2 = inpLine.find(':', pos1);
            if (pos2 == std::string::npos) continue;
            key.mDay = atoi(inpLine.substr(pos1, pos2 - pos1).c_str());
            pos1 = pos2 + 1;

            // parse day description and highlighting property
            str = inpLine.substr(pos1);
            ModifyUmlauts(str);
            data.mDescr = str;
            data.mHighlighted = false;
            if (str[str.size() - 1] == '*') {
               data.mDescr = str.substr(0, str.size() - 1);
               data.mHighlighted = true;
            }
            InsertDay(key, data);
         }
      }
   }

   return year;
}

//------------------------------------------------------------------------------
//  main
//
int main(int argc, char *argv[]) {

   if (argc < 2 || (argv[1][0]=='-' && argc < 3)) {
      std::cerr << "ERROR: required argument missing: <CfgFile>\n";
      return 1;
   }

   // CfgFile argument preceded by dash argument: no user-defined events
   bool withEvents = argv[1][0] != '-';
   int  cfgIndex   = withEvents ? 1 : 2;
   int  year       = 0;

   std::ifstream inp(argv[cfgIndex]);
   if (inp) {
      year = ParseCfg(inp, withEvents);
   } else {
      std::cerr << "ERROR: could not open <CfgFile>\n";
      return 1;
   }

   if (year == 0) {
      std::cerr << "ERROR: no valid year defined in <CfgFile> (syntax: $<year>)\n";
      return 1;
   }

   try {
      // create output with one month per page (12 pages, portrait)
      CMonthPerPageFormatter(std::to_string(year) + "m.ps").Run(year, sDays);

      // create output with one half year per page (2 pages, landscape)
      CHalfYearPerPageFormatter(std::to_string(year) + "y.ps").Run(year, sDays);
   }
   catch (std::exception &e) {
      std::cerr << "ERROR: " << e.what() << "\n";
      return 1;
   }

   return 0;
}
