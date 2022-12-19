//
// Created by 1012
//
#ifndef __RESERVATION_H__
#define __RESERVATION_H__

struct reservation {
    struct reservation *pNext;
    struct reservation *pPrev;
    char szName[128];
    char szRoomNr[3];
    unsigned int iDate;
    int iDays;
    float fPricePerDay;
};

void clearAllocation(struct reservation **ppHead, struct reservation **ppTail);
int initReservation(struct reservation **ppHead, struct reservation **ppTail, char *pszName, char *pszRoomNr, int iDate, int iDays, float fPricePerDay);
int insertBefore(struct reservation **ppBeforeRes, char *pszName, char *pszRoomNr, int iDate, int iDays, float fPricePerDay);
int insertAfter(struct reservation **ppAfterRes, char *pszName, char *pszRoomNr, int iDate, int iDays, float fPricePerDay);
void deleteReservation(struct reservation **ppReservation);
void deleteCompletedReservations(struct reservation **ppHead);
void searchReservationByName(struct reservation **ppHead, char *pName);
void sumPriceOfReservations(struct reservation **ppHead);
void printReservations(struct reservation **ppHead);
void printReservation(struct reservation **ppReservation);
struct tm getTmFromYYYYMMDD(unsigned int iDate);
void printDate(struct tm *pTmDate);

#endif //__RESERVATION_H__
