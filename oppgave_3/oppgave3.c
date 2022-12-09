//
// Created by 1012 on 08.12.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/reservation.h"
#include "include/menu.h"
#include "include/oppgave3.h"

int main(int argc, char *argv[]){

    struct reservation *pHead = NULL;
    struct reservation *pTail = NULL;
    initReservation(&pTail, &pHead);

    //Init test data
    insertAfter(&pTail, "Trine", "1", 01012023, 5, 9.99);
    insertAfter(&pTail, "Marie", "2", 02022022, 6, 12.99);
    insertAfter(&pTail, "Kari", "3", 03032023, 7, 5.50);
    insertAfter(&pTail, "Ole", "4", 09121992, 8, 20.3);
    insertAfter(&pTail, "Per", "5", 01022033, 9, 59.90);

    char chChoice;
    int iDecoratorLength = 50;
    char cDecorator = '~';
    int iExit = 0;

    while(iExit == 0){
        printMenu(iDecoratorLength, cDecorator);
        chChoice = getFirstCharOfUserInput();

        switch (chChoice) {
            case '1':
                printHeader("1. Add reservation", iDecoratorLength, cDecorator);
                addReservationMenu(&pTail);
                break;
            case '2':
                printHeader("2. Delete last reservation", iDecoratorLength, cDecorator);
                deleteReservation(&pTail);
                break;
            case '3':
                printHeader("3. Delete all completed reservations", iDecoratorLength, cDecorator);
                deleteCompletedReservations(&pHead);
                break;
            case '4':
                printHeader("4. Search by name", iDecoratorLength, cDecorator);
                searchByNameMenu(&pHead);
                break;
            case '5':
                printHeader("5. Sum price of all reservations", iDecoratorLength, cDecorator);
                sumPriceOfReservations(&pHead);
                break;
            case '6':
                printHeader("6. Print all reservations", iDecoratorLength, cDecorator);
                printReservations(&pHead);
                break;
            case '0':
                printHeader("0. Exit", iDecoratorLength, cDecorator);
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
    char chFirstChar;
    int iValidInput = 0;

    chFirstChar = getchar();

    clearInputStack();

    return chFirstChar;
}

int failedToReadInput(int returnCode){

    clearInputStack();

    if(returnCode == 0){
        printf("Failed to read input\n");
        return 1;
    }
    return 0;
}

void addReservationMenu(struct reservation **ppTail){
    char szName[128];
    char szRoomNr[3];
    int iDate;
    int iDays;
    float fPricePerDay;

    printf("Enter name: ");
    if(failedToReadInput(scanf("%s", szName))){
        return;
    }

    printf("Enter room number: ");
    if(failedToReadInput(scanf("%s", szRoomNr))){
        return;
    }

    printf("Enter date (YYYYMMDD): ");
    if(failedToReadInput(scanf("%d", &iDate))){
        return;
    }

    printf("Enter number of days: ");
    if(failedToReadInput(scanf("%d", &iDays))){
        return;
    }

    printf("Enter price per day: ");
    if(failedToReadInput(scanf("%f", &fPricePerDay))){
        return;
    }

    if(insertAfter(ppTail, szName, szRoomNr, iDate, iDays, fPricePerDay)){
        printSeparator(50, '-');
        printf("Reservation added.\n");
        printReservation(ppTail);
    }
} // End addReservationMenu

void searchByNameMenu(struct reservation **ppHead){
    char szName[128];

    printf("Enter name: ");
    if(failedToReadInput(scanf("%s", szName))){
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

