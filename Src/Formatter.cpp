//
//    $VER: Formatter.cpp 1.0 (28.12.2007)
//
//    PostScript output formatting strategies for PSMonths.
//
//==============================================================================
//
#include "Formatter.h"
#include "DateUtils.h"

#include <stdexcept>

#include <stdlib.h>
#include <stdarg.h>

//------------------------------------------------------------------------------
//  Constants
//
static const char * const spMonths[] = {
   "J\\212nner", "Februar", "M\\212rz", "April", "Mai", "Juni",
   "Juli", "August", "September", "Oktober", "November", "Dezember"
};

static const char * const spWeekdays[] = {
   "Mo", "Di", "Mi", "Do", "Fr", "Sa", "So"
};

static const char *spBoxIDs[] = {
   "PBox", "SBox"
};

static const char cEmptyStr[] = "";

static const char cHeaderStr[] =
	"%%!PS-Adobe-1.0\n"
	"%%%%Title: output.ps\n"
	"%%%%Creator: PSMonths\n"
	"%%%%CreationDate: (01/10/06 00:00:00)\n"
	"%%%%DocumentFonts: Helvetica Helvetica-Bold\n"
	"%%%%Pages: (atend)\n"
   "%%%%Orientation: %s\n" // Parameter: Portrait or Landscape
	"%%%%EndComments\n"
	"\n";

static const char cTrailerStr[] =
	"%%%%Trailer\n"
	"%%%%Pages: %d\n"; // Parameter: page count

static const char cMonthPerPageDefsStr[] =
	"/headerSize 24 def\n"
	"\n"
	"/textSize 14 def\n"
	"/hMargin   4 def\n"
	"/vMargin   4 def\n"
	"/height   22 def\n"
	"\n"
	"/lWidth  60 def\n"
	"/rWidth 460 def\n"
	"\n"
	"/hOrigin  30 def\n"
	"/vOrigin 780 def\n"
	"\n";

static const char cHalfYearPerPageDefsStr[] =
	"/textSize  8 def   %% used in PBox\n"
	"/hMargin   2 def   %% used in PBox\n"
	"/vMargin   4 def   %% used in PBox\n"
	"/height   16 def   %% used in PBox, SBox, BText\n"
	"\n"
	"/bTextSize 10 def   %% used in PText\n"
	"/bvMargin   2 def   %% used in PText\n"
	"\n"
	"/lWidth 28 def\n"
	"/rWidth 92 def\n"
	"\n"
	"/tWidth lWidth rWidth add def\n"
	"\n";

static const char cBoxDefsStr[] =
	"/PBox {\n"
	"   3 -1 roll dup 4 1 roll\n"
	"   3 -1 roll dup 4 1 roll\n"
	"   3 -1 roll dup 4 1 roll\n"
	"   gsave\n"
	"   newpath\n"
	"      moveto\n"
	"      0 0 height sub rlineto\n"
	"      0 rlineto\n"
	"      0 height rlineto\n"
	"      closepath\n"
	"      clip\n"
	"      .0 setgray\n"
	"   stroke\n"
	"   2 -1 roll dup 3 1 roll hMargin add\n"
	"   2 -1 roll dup 3 1 roll vMargin sub textSize sub\n"
	"   moveto\n"
	"   4 -1 roll\n"
	"   show\n"
	"   4 1 roll\n"
	"   add hMargin sub\n"
	"   2 -1 roll dup 3 1 roll\n"
	"   stringwidth pop sub\n"
	"   3 -1 roll\n"
	"   vMargin sub textSize sub\n"
	"   moveto\n"
	"   show\n"
	"   grestore\n"
	"} def\n"
	"\n"
	"/SBox {\n"
	"   3 -1 roll dup 4 1 roll\n"
	"   3 -1 roll dup 4 1 roll\n"
	"   3 -1 roll dup 4 1 roll\n"
	"   newpath\n"
	"      moveto\n"
	"      0 0 height sub rlineto\n"
	"      0 rlineto\n"
	"      0 height rlineto\n"
	"      closepath\n"
	"      .8 setgray\n"
	"      fill\n"
	"   stroke\n"
	"   PBox\n"
	"} def\n"
	"\n";

static const char cTextDefsStr[] =
	"/PText {\n"
	"   moveto\n"
	"   2 div\n"
	"   bvMargin neg bTextSize sub rlineto\n"
	"   dup stringwidth pop\n"
	"   2 div\n"
	"   neg\n"
	"   0 rlineto\n"
	"   show\n"
	"} def\n"
	"\n"
	"/BText {\n"
	"   3 -1 roll dup 4 1 roll\n"
	"   3 -1 roll dup 4 1 roll\n"
	"   3 -1 roll dup 4 1 roll\n"
	"   newpath\n"
	"      moveto\n"
	"      0 0 height sub rlineto\n"
	"      0 rlineto\n"
	"      0 height rlineto\n"
	"      closepath\n"
	"   stroke\n"
	"   PText\n"
	"} def\n"
	"\n";

static const char cFontDefsStr[] =
	"/ReEncodeSmallDict 12 dict def\n"
	"\n"
	"/ReEncodeSmall\n"
	"{\n"
	"ReEncodeSmallDict begin\n"
	"  /NewCodesAndNames exch def\n"
	"  /NewFontName exch def\n"
	"  /BaseFontName exch def\n"
	"\n"
	"  /BaseFontDict BaseFontName findfont def\n"
	"  /NewFont BaseFontDict maxlength dict def\n"
	"\n"
	"  BaseFontDict\n"
	"  {\n"
	"    exch dup /FID ne\n"
	"    {\n"
	"      dup /Encoding eq\n"
	"      { exch dup length array copy NewFont 3 1 roll put }\n"
	"      { exch NewFont 3 1 roll put }\n"
	"      ifelse\n"
	"    }\n"
	"    { pop pop }\n"
	"    ifelse\n"
	"  } forall\n"
	"\n"
	"  NewFont /FontName NewFontName put\n"
	"  NewCodesAndNames aload pop\n"
	"\n"
	"  NewCodesAndNames length 2 idiv\n"
	"  { NewFont /Encoding get 3 1 roll put\n"
	"  } repeat\n"
	"  NewFontName NewFont definefont pop\n"
	"  end\n"
	"} def\n"
	"\n"
	"/GermanMap\n"
	"[\n"
	"  8#212 /adieresis\n"
	"  8#200 /Adieresis\n"
	"  8#232 /odieresis\n"
	"  8#205 /Odieresis\n"
	"  8#237 /udieresis\n"
	"  8#206 /Udieresis\n"
	"  8#247 /germandbls\n"
	"] def\n"
	"\n"
	"/Helvetica-Bold /Helvetica-Bold-German GermanMap ReEncodeSmall\n"
	"/Helvetica      /Helvetica-German      GermanMap ReEncodeSmall\n"
	"\n"
	"/newpage {\n"
	"  gsave\n"
	"  .25 setlinewidth\n"
	"} def\n"
	"\n"
	"/endpage {\n"
	"  grestore\n"
	"  save\n"
	"  restore\n"
	"  showpage\n"
	"} def\n";

//------------------------------------------------------------------------------
//  Functions
//

//------------------------------------------------------------------------------
//  Constructor
//
CFormatter::CFormatter(const std::string &outpName) :
   mpOutp(fopen(outpName.c_str(), "w")) {

   if (mpOutp.get() == nullptr) {
      throw std::runtime_error(std::string("cannot open output file ") + outpName);
   }
}

//------------------------------------------------------------------------------
//  CFormatter::Printf
//
void CFormatter::Printf(const char *pFormat, ...) {

   va_list list;

   va_start(list, pFormat);
   vfprintf(mpOutp.get(), pFormat, list);
   va_end(list);
}

//------------------------------------------------------------------------------
//  CFormatter::Run
//
void CFormatter::Run(int year, const TDayMap &days) {

   TDayKey   key;
   TDayData  data;
   int       month, day, wDay, mDays, week;
	bool      highlighted;

   wDay = DayOfWeek(1, 1, year);
   week = 1;
   if (wDay >= eDowTuesday && wDay <= eDowThursday) ++week;

   OnYearBegin(year);

   for (month=1; month<=12; ++month) {

      key.mMonth = month;
      mDays = DaysInMonth(month, year);

      OnMonthBegin(year, month);

      for (day=1; day<=mDays; ++day) {

        	std::string descr;
			highlighted = wDay == eDowSunday;

         key.mDay = day;
         TDayMap::const_iterator it = days.find(key);
         if (it != days.end()) {
				descr = it->second.mDescr;
				if (it->second.mHighlighted) {
					highlighted = true;
				}
         }

         OnMonthDay(day, wDay, week, descr.c_str(), highlighted);

         if (wDay == eDowMonday) ++week;

         // more than 52 weeks AND 4th Jan on Fri/Sat/Sun: KW1 instead of KW53
         if (week == 53 && DayOfWeek(31, 12, year) < eDowThursday) week = 1;

         wDay = (wDay + 1) % 7;
      }

      OnMonthEnd(year, month);
   }

   OnYearEnd(year);
}

//------------------------------------------------------------------------------
//  CMonthPerPageFormatter
//
void CMonthPerPageFormatter::OnYearBegin(int year) {
   Printf(cHeaderStr, "Portrait");
   Printf("%s%s%s\n", cMonthPerPageDefsStr, cBoxDefsStr, cFontDefsStr);
}

void CMonthPerPageFormatter::OnYearEnd(int year) {
   Printf(cTrailerStr, 12);
}

void CMonthPerPageFormatter::OnMonthBegin(int year, int month) {
   (void)year;
   Printf(
		"%%%%Page: %d %d\n"
		"newpage\n"
		"/Helvetica-Bold-German findfont headerSize scalefont setfont\n"
		"hOrigin vOrigin moveto (%s) show\n"
		"/Helvetica-German findfont textSize scalefont setfont\n"
		"/hPos hOrigin        def\n"
		"/vPos vOrigin 40 sub def\n",
		month, month, spMonths[month - 1]);
}

void CMonthPerPageFormatter::OnMonthEnd(int year, int month) {
   (void)year;
   Printf(
		"endpage\n"
		"%%%%EndPage: %d %d\n\n",
		month, month);
}

void CMonthPerPageFormatter::OnMonthDay(int day, int wDay, int week, const char *pDescr, bool highlighted) {
   Printf(
      "(%d) (%s) lWidth hPos vPos %s "
      "(%s) (%s) rWidth hPos lWidth add vPos %s\n"
      "/vPos vPos height sub def\n",
      day, spWeekdays[wDay], spBoxIDs[highlighted],
      wDay == eDowMonday ? std::to_string(week).c_str() : cEmptyStr,
      pDescr,
      spBoxIDs[highlighted]);
}

//------------------------------------------------------------------------------
//  CHalfYearPerPageFormatter
//
void CHalfYearPerPageFormatter::OnYearBegin(int year) {
   Printf(cHeaderStr, "Landscape");
   Printf("%s%s%s%s\n", cHalfYearPerPageDefsStr, cBoxDefsStr, cTextDefsStr, cFontDefsStr);
   PrintHalfYearBegin(year, 1);
}

void CHalfYearPerPageFormatter::OnYearEnd(int year) {
   PrintHalfYearEnd(2);
   Printf(cTrailerStr, 2);
}

void CHalfYearPerPageFormatter::OnMonthBegin(int year, int month) {
   (void)year;
   Printf(
      "%%--- %d. Monat ---\n"
      "\n"
      "/hPos hStart def\n"
      "/vPos vStart def\n"
      "\n",
      month);
}

void CHalfYearPerPageFormatter::OnMonthEnd(int year, int month) {
   Printf(
      "\n"
      "/hStart hStart tWidth add def\n"
      "\n");
   if (month == 6) {
      PrintHalfYearEnd(1);
      PrintHalfYearBegin(year, 2);
   }
}

void CHalfYearPerPageFormatter::OnMonthDay(int day, int wDay, int week, const char *pDescr, bool highlighted) {
   Printf(
      "(%d) (%s) lWidth hPos vPos %s\n"
      "(%s) (%s) rWidth lWidth hPos add vPos %s\n"
      "/vPos vPos height sub def\n",
      day, spWeekdays[wDay], spBoxIDs[highlighted],
      wDay == eDowMonday ? std::to_string(week).c_str() : cEmptyStr, pDescr, spBoxIDs[highlighted]);
}

void CHalfYearPerPageFormatter::PrintHalfYearBegin(int year, int half) {
   int i = (half - 1) * 6;
   Printf(
      "%%%%Page: %d %d\n"
      "newpage\n"
      "\n"
      "40 60 translate\n"
      "90 rotate\n"
      "\n"
      "%%--- Tabellenbeschriftung mit Jahr ---\n"
      "\n"
      "/Helvetica-Bold-German findfont 24 scalefont setfont\n"
      "\n"
      "(%d) tWidth 6 mul 0 0 PText\n"
      "\n"
      "%%--- Spaltenbeschriftung mit Monat ---\n"
      "\n"
      "/Helvetica-Bold-German findfont bTextSize scalefont setfont\n"
      "\n"
      "(%s) tWidth 0            height neg BText\n"
      "(%s) tWidth tWidth       height neg BText\n"
      "(%s) tWidth tWidth 2 mul height neg BText\n"
      "(%s) tWidth tWidth 3 mul height neg BText\n"
      "(%s) tWidth tWidth 4 mul height neg BText\n"
      "(%s) tWidth tWidth 5 mul height neg BText\n"
      "\n"
      "%%--- Monatsausgabe vorbereiten\n"
      "\n"
      "/Helvetica-German findfont textSize scalefont setfont\n"
      "\n"
      "/hStart 0 def\n"
      "/vStart height 2 mul neg def\n"
      "\n",
      half, half, year,
      spMonths[i], spMonths[i+1], spMonths[i+2],
      spMonths[i+3], spMonths[i+4], spMonths[i+5]);
}

void CHalfYearPerPageFormatter::PrintHalfYearEnd(int half) {
   Printf(
      "endpage\n"
      "%%%%EndPage: %d %d\n"
      "\n",
      half, half);
}
