#ifndef __MSLOGGER_H__
#define __MSLOGGER_H__

void MsLogger(unsigned long ulErrorType, int iLine, char *szFile, const char *pszFormat, ...);

#define msdebug(...) if (glob_bTraceEnabled == 1){MsLogger(0, __LINE__, __FILE__, __VA_ARGS__);}
#define mserror(...) MsLogger(1, __LINE__, __FILE__, __VA_ARGS__)

#endif //__MSLOGGER_H__
