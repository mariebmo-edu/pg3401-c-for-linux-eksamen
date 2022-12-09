//
// Created by 1012 on 09.12.2022.
//

#ifndef __RESERVATION_H__
#define __RESERVATION_H__

struct reservation {
    struct reservation *pNext;
    struct reservation *pPrev;
    char szName[128];
    char szRoomNr[3];
    int iDate;
    int iDays;
    float fPricePerDay;
};

void initReservation(struct reservation **ppTail, struct reservation **ppHead);
void insertBefore(struct reservation **ppBeforeRes, char *pszName, char *pszRoomNr, int iDate, int iDays, float fPricePerDay);
int insertAfter(struct reservation **ppAfterRes, char *pszName, char *pszRoomNr, int iDate, int iDays, float fPricePerDay);
void deleteReservation(struct reservation **ppReservation);
void deleteCompletedReservations(struct reservation **ppHead);
void searchReservationByName(struct reservation **ppHead, char *pName);
void sumPriceOfReservations(struct reservation **ppHead);
void printReservations(struct reservation **ppHead);
void printReservation(struct reservation **ppReservation);

#endif //__RESERVATION_H__
