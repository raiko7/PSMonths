# Overview
*PSMonths* is a PostScript month calendar generator. It comes with pre-defined holidays for Austria and allows you to add per-day events using a text-based configuration. It creates two PostScript output files:
* a year view consisting of two pages, one page per half-year
* a month view consisting of 12 pages, one page per month

In order to convert generated PostScript files to PDF, you have two options:
1. Install and use [GhostScript](https://www.ghostscript.com/)
2. Use a free online service, for example [Online2PDF](https://online2pdf.com/convert-ps-to-pdf) or [ps2pdf](http://www.ps2pdf.com/)

In order to adapt pre-defined holidays to another country you need to change the source code, as described later.

# Getting started
*PSMonths* must be run from the command line as follows:
```
PSMonths [-] <configuration_file>
```
If the dash ('-') is given, user-defined events are exluded, only pre-defined holidays are included.

The configuration file is a simple text file. Example:
```
$2018
#################
# family events #
#################
1:9:Opas Geburtstag
4:7:Omas Geburtstag
9:9:Hochzeitstag
```
Lines starting with '#' are treated as command lines. The line starting with '$' defines the year. All the other lines define events, where each event follows the form <*month*>:<*day*>:<*description*>. If the description ends with an asterisk ('*'), the event's day is printed as a holiday.

For the example above *PSMonths* creates the following files:
* [2018y](https://github.com/raiko7/PSMonths/blob/master/Example/2018y.pdf)
* [2018m](https://github.com/raiko7/PSMonths/blob/master/Example/2018m.pdf)

# Customization
In order to adapt *PSMonths* to another language and country you first have to change month names and day names:
```cpp
// Formatter.cpp
static const char * const spMonths[] = {
   "J\\212nner", "Februar", "M\\212rz", "April", "Mai", "Juni",
   "Juli", "August", "September", "Oktober", "November", "Dezember"
};
static const char * const spWeekdays[] = {
   "Mo", "Di", "Mi", "Do", "Fr", "Sa", "So"
};
```

If the language uses diacritical signs, their translation to PostScript must be specified as follows:
```cpp
// PSMonths.cpp
static const char sEscChars[] =
   "\xe4\xc4\xf6\xd6\xfc\xdc\xdf()\\";
static const char * const spUmlauts[] = {
	"\\212", "\\200", "\\232", "\\205", "\\237", "\\206", "\\247"
};
```
Finally, pre-defined holidays of a country are defined in the function `InsertHolidays`:
```cpp
// PSMonths.cpp
static void InsertHolidays(int year) {
   ...
}
```

# Requirements
In order to run the build using the provided project files you need at least Visual Studio 2017 Commodity Edition.
