#if !defined(FORMATTER_H_)
#define FORMATTER_H_
//
//    $VER: Formatter.h 1.0 (28.12.2007)
//
//    PostScript output formatting strategies for PSMonths.
//
//==============================================================================
//
#include "PSMonths.h"

#include <memory>
#include <stdio.h>

//------------------------------------------------------------------------------
namespace std {
   template <> struct default_delete<FILE> {
      void operator()(FILE *fp) { if (fp) fclose(fp); }
   };
}

//------------------------------------------------------------------------------
//  CFormatter
//
class CFormatter {
public:
   CFormatter(const std::string &outpName);
   virtual ~CFormatter(void) = default;

   // Generic day traversal algorithm (intermediate state as locals)
   void Run(int year, const TDayMap &days);

protected:
   void Printf(const char *pFormat, ...);

private:
   // template methods for particular points in the algorithm
   virtual void OnYearBegin (int year) = 0;
   virtual void OnYearEnd   (int year) = 0;

   virtual void OnMonthBegin(int year, int month) = 0;
   virtual void OnMonthEnd  (int year, int month) = 0;

   virtual void OnMonthDay  (int day, int way, int week, const char *pDescr, bool highlighted) = 0;

   std::unique_ptr<FILE> mpOutp;
};

//------------------------------------------------------------------------------
//  CMonthPerPageFormatter
//
class CMonthPerPageFormatter : public CFormatter {
public:
   CMonthPerPageFormatter(const std::string &outpName) : CFormatter(outpName) { }

private:
   void OnYearBegin (int year) override;
   void OnYearEnd   (int year) override;

   void OnMonthBegin(int year, int month) override;
   void OnMonthEnd  (int year, int month) override;

   void OnMonthDay  (int day, int way, int week, const char *pDescr, bool highlighted) override;
};

//------------------------------------------------------------------------------
//  CHalfYearPerPageFormatter
//
class CHalfYearPerPageFormatter : public CFormatter {
public:
   CHalfYearPerPageFormatter(const std::string &outpName) : CFormatter(outpName) { }

private:
   void OnYearBegin (int year) override;
   void OnYearEnd   (int year) override;

   void OnMonthBegin(int year, int month) override;
   void OnMonthEnd  (int year, int month) override;

   void OnMonthDay  (int day, int way, int week, const char *pDescr, bool highlighted) override;

   void PrintHalfYearBegin(int year, int half);
   void PrintHalfYearEnd(int half);
};

#endif //FORMATTER_H_
