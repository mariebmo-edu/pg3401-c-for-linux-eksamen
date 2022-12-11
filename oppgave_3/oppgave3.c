//
// Created by 1012 on 08.12.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "include/reservation.h"
#include "include/menu.h"
#include "include/oppgave3.h"

int main(int argc, char *argv[]){

    struct reservation *pHead = NULL;
    struct reservation *pTail = NULL;

    //Init test data
    initReservation(&pHead, &pTail, "Trine", "1", 20231212, 5, 9.99);
    insertAfter(&pTail, "Marie", "2", 20231224, 6, 12.99);
    insertAfter(&pTail, "Kari", "3", 20221212, 7, 5.50);
    insertAfter(&pTail, "Ole", "4", 20220512, 8, 20.3);
    insertAfter(&pTail, "Per", "5", 20230112, 9, 59.90);

    char chChoice;
    int iDecoratorLength = 50;
    char chDecorator = '~';
    int iExit = 0;

    while(iExit == 0){
        printMenu(iDecoratorLength, chDecorator);
        chChoice = getFirstCharOfUserInput();

        switch (chChoice) {
            case '1':
                printHeader("1. Add reservation", iDecoratorLength, chDecorator);
                addReservationMenu(&pHead, &pTail);
                break;
            case '2':
                printHeader("2. Delete last reservation", iDecoratorLength, chDecorator);
                deleteReservation(&pTail);
                break;
            case '3':
                printHeader("3. Delete all completed reservations", iDecoratorLength, chDecorator);
                deleteCompletedReservations(&pHead);
                break;
            case '4':
                printHeader("4. Search by name", iDecoratorLength, chDecorator);
                searchByNameMenu(&pHead);
                break;
            case '5':
                printHeader("5. Sum price of all reservations", iDecoratorLength, chDecorator);
                sumPriceOfReservations(&pHead);
                break;
            case '6':
                printHeader("6. Print all reservations", iDecoratorLength, chDecorator);
                printReservations(&pHead);
                break;
            case '0':
                printHeader("0. Exit", iDecoratorLength, chDecorator);
                iExit = 1;

                break;
            default:
                printf("%c. Invalid input\n", chChoice);
                break;
        } //End switch
    } // End while

    clearAllocation(&pHead, &pTail);
} // End main

char getFirstCharOfUserInput(){
    char *szInput = NULL;
    size_t iInputLength = 0;

    if(getline(&szInput, &iInputLength, stdin) == -1){
        printf("Error reading input");
        exit(1);
    }

    char chFirstChar = szInput[0];
    free(szInput);

    return chFirstChar;
}

int validateInput(int returnCode){

    clearInputStack();

    if(returnCode == 1){
        printf("Failed to read input\n");
        return 1;
    }
    return 0;
}

int validateDateInput(unsigned int iDateInput){
    struct tm time = getTmFromYYYYMMDD(iDateInput);

    //Ser om det er vits i å sjekke om datoen er gyldig
    if(time.tm_year < 0
            || time.tm_year > 9999
            || time.tm_mon < 0
            || time.tm_mon > 11
            || time.tm_mday < 0
            || time.tm_mday > 30){
        printf("Invalid date input\n");
        return 1;
    }

    //Februar har 28 dager
    if(time.tm_mon == 1) {
        if (time.tm_mday > 27) {
            printf("Invalid date input\n");
            return 1;
        }
    }

    //Januar, mars, mai, juli, august, oktober og desember har 31 dager
    if(time.tm_mon == 0 || time.tm_mon == 2 || time.tm_mon == 4 || time.tm_mon == 6 || time.tm_mon == 7 || time.tm_mon == 9 || time.tm_mon == 11) {
        if (time.tm_mday > 30) {
            printf("Invalid date input\n");
            return 1;
        }
    }

    //April, juni, september og november har 30 dager
    if(time.tm_mon == 3 || time.tm_mon == 5 || time.tm_mon == 8 || time.tm_mon == 10){
        if(time.tm_mday > 29){
            printf("Invalid date input\n");
            return 1;
        }
    } else {
        return 0;
    }
}

void addReservationMenu(struct reservation **ppHead, struct reservation **ppTail){
    char szName[128];
    char szRoomNr[3];
    unsigned int iDate;
    int iDays;
    float fPricePerDay;

    printf("Enter name: ");
    if(validateInput(scanf("%s", szName) != 1)){
        return;
    }

    printf("Enter room number: ");
    if(validateInput(scanf("%s", szRoomNr) != 1)){
        return;
    }

    printf("Enter date (YYYYMMDD): ");
    if(validateInput(scanf("%d", &iDate) != 1) || validateDateInput(iDate) == 1){
        return;
    }

    printf("Enter number of days: ");
    if(validateInput(scanf("%d", &iDays) != 1)){
        return;
    }

    printf("Enter price per day: ");
    if(validateInput(scanf("%f", &fPricePerDay) != 1)){
        return;
    }

    if(*ppHead == NULL && *ppTail == NULL){
        initReservation(ppHead, ppTail, szName, szRoomNr, iDate, iDays, fPricePerDay);
    } else {
        if(insertAfter(ppTail, szName, szRoomNr, iDate, iDays, fPricePerDay)){
            printSeparator(50, '-');
            printf("Reservation added.\n");
            printReservation(ppTail);
        }
    }
} // End addReservationMenu

void searchByNameMenu(struct reservation **ppHead){
    char szName[128];

    printf("Enter name: ");
    if(validateInput(scanf("%s", szName) != 1)){
        return;
    }

    searchReservationByName(ppHead, szName);
}

void clearInputStack(){
    int iStackEmpty = 0;
    char chRestOfLine;

    while(iStackEmpty == 0){
        chRestOfLine = getchar();
        if(chRestOfLine == '\n' || chRestOfLine == EOF){
            iStackEmpty = 1;
        }
    }
}

