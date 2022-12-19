//
// Created by marie on 19.12.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/dateTimeHandling.h"

// Tar inn en int i YYYYMMDD format (ex. 20201219) og resutnerer en tm-struct.
struct tm getTmFromYYYYMMDD(unsigned int iDate){
    struct tm date;
    memset(&date, 0, sizeof(struct tm));

    //TM-struktøren bruker år fra 1900, så vi må trekke fra 1900 for å få riktig år.
    date.tm_year = iDate / 10000 - 1900;

    // TM-struktøren bruker måneder fra 0-11, så vi må trekke fra 1 for å få riktig måned.
    date.tm_mon = iDate % 10000 / 100 - 1;

    // TM-struktøren bruker dager fra 1-31, så vi trenger ikke trekke fra 1.
    date.tm_mday = iDate % 100;

    return date;
}

// Tar inn en tm-struct og printer ut datoen i formatet "2021-12-1"
void printDate(struct tm *pTmDate){
    printf("%d-%d-%d", pTmDate->tm_year + 1900, pTmDate->tm_mon + 1, pTmDate->tm_mday);
}

// Tar inn en tm-struct og printer ut datoen i formatet "1. januar 2021".
void printDateFormatted(struct tm *pTmDate){
    char buffer[80];
    strftime(buffer, 80, "%d. %B %Y", pTmDate);
    printf("%s", buffer);
}
