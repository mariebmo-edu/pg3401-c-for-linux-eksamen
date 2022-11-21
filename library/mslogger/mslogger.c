#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "include/mslogger.h"

void MsLogger(unsigned long ulErrorType, int iLine, char *szFile, const char *pszFormat, ...) {

    /*Error or debug*/
    va_list vaArgumentPointer;
    char szOutputString[256] = {0};
    const char *pszType = (ulErrorType==1)?"Error":"Debug";

    /*File handling*/
    static FILE *fLogFile = NULL;
    char szFileName = char[256] = {0};
    time_t tTimeAndDate = 0;

    if(fLogFile == NULL){
        tTimeAndDate = time(null);
        snprintf(szFileName, 256-1, "debug_log_%s.txt", localtime(&tTimeAndDate));
    }

    fLogFile = fopen(szFileName, "w");

    /*amount of times called*/
    static unsigned int iCallCounter = 0;

    va_start(vaArgumentPointer, pszFormat);
    vfprintf(szOutputString, 256-1, pszFormat, vaArgumentPointer);
    va_end(vaArgumentPointer);

    fprintf(fp, "%04i: error type is %lu\n", iCallCounter++, pszType, szOutputString);

    return;
}