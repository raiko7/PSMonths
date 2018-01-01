#if !defined(PS_MONTHS_H_)
#define PS_MONTHS_H_
//
//    $VER: PSMonths.h 1.1 (01.10.06, 27.12.07)
//
//    PostScript Month Calendar Generator, pre-defined holidays for Austria
//
//    (C) Copyright 2006-2017 by Rainer Koppler
//
//==============================================================================
//
#include <string>
#include <map>

//------------------------------------------------------------------------------
//  Types
//
struct TDayKey {
   int  mMonth;
   int  mDay;
};

struct TDayData {
   std::string  mDescr;
   bool         mHighlighted;
};

using TDayMap = std::map<TDayKey, TDayData>;

//------------------------------------------------------------------------------
//  Inline Functions
//
inline bool operator<(const TDayKey &obj1, const TDayKey &obj2) {

   return std::tie(obj1.mMonth, obj1.mDay) < std::tie(obj2.mMonth, obj2.mDay);
}


#endif //PS_MONTHS_H_
