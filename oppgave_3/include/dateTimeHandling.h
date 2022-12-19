//
// Created by 1012
//
#ifndef __TIMEHANDELER_H__
#define __TIMEHANDELER_H__

struct tm getTmFromYYYYMMDD(unsigned int iDate);
void printDate(struct tm *pTmDate);
void printDateFormatted(struct tm *pTmDate);
int isLeapYear(int year);

#endif //__TIMEHANDELER_H__
