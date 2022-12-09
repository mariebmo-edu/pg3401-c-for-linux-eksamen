//
// Created by 1012 on 08.12.2022.
//
#include <stdio.h>
#include <stdlib.h>

#include "include/oppgave3.h"
#include "include/reservation.h"

void printMenu();
int handleMenu();
char getFirstCharOfUserInput();
void addReservationMenu(struct reservation **ppTail);
void searchByNameMenu(struct reservation **ppHead);
void clearInputStack();

int main(int argc, char *argv[]){

    struct reservation *pHead = NULL;
    struct reservation *pTail = NULL;
    initReservation(&pTail, &pHead);

    //Init test data
    insertAfter(&pTail, "Trine", "1", 20220101, 5, 9.99);
    insertAfter(&pTail, "Marie", "2", 20220102, 6, 12.99);
    insertAfter(&pTail, "Kari", "3", 20220103, 7, 5.50);
    insertAfter(&pTail, "Ole", "4", 20220104, 8, 20.3);
    insertAfter(&pTail, "Per", "5", 20220105, 9, 59.90);

    char chChoice;
    int iExit = 0;

    while(iExit == 0){
        printMenu();
        chChoice = getFirstCharOfUserInput();

        switch (chChoice) {
            case '1':
                printf("1. Add reservation\n");
                addReservationMenu(&pTail);
                break;
            case '2':
                printf("2. Delete last reservation\n");
                deleteReservation(&pTail);
                break;
            case '3':
                printf("3. Delete all completed reservations\n");
                deleteCompletedReservations(&pHead);
                break;
            case '4':
                printf("4. Search by name\n");
                searchByNameMenu(&pHead);
                break;
            case '5':
                printf("5. Sum up total price for all reservations\n");
                sumPriceOfReservations(&pHead);
                break;
            case '6':
                printf("6. Print all reservations\n");
                printReservations(&pHead);
                break;
            case '0':
                printf("0. Exit\n");
                iExit = 1;
                break;
            default:
                printf("%c. Invalid input\n", chChoice);
                break;
        }
        printf("\n");
    }
}

void printMenu(){
    printf("1. Add reservation\n");
    printf("2. Delete last reservation\n");
    printf("3. Delete all completed reservations\n");
    printf("4. Search by name\n");
    printf("5. Sum up total price for all reservations\n");
    printf("6. Print all reservations\n");
    printf("7. Exit program\n");
    printf("Enter choice: ");
}

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
        printf("Reservation added.\n");
        printReservation(ppTail);
    }
}

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

