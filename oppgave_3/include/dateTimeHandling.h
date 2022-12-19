//
// Created by marie on 19.12.2022.
//

#ifndef __TIMEHANDELER_H__
#define __TIMEHANDELER_H__

struct tm getTmFromYYYYMMDD(unsigned int iDate);
void printDate(struct tm *pTmDate);
void printDateFormatted(struct tm *pTmDate);

#endif //__TIMEHANDELER_H__
