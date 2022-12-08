//
// Created by 1012 on 08.12.2022.
//
#include <stdio.h>

#include "include/oppgave3.h"

void printMenu();
int addReservation();
char getFirstCharOfUserInput();

int main(int argc, char *argv[]){

    char chChoice;
    int iExit = 0;

    while(iExit == 0){
        printMenu();
        chChoice = getFirstCharOfUserInput();

        switch (chChoice) {
            case '1':
                iExit = addReservation();
                break;
            case '2':
                printf("2. Delete last reservation\n");
                break;
            case '3':
                printf("3. Delete all completed reservations\n");
                break;
            case '4':
                printf("4. Search by name\n");
                break;
            case '5':
                printf("5. Sum up total price for all reservations\n");
                break;
            case '6':
                printf("6. Print all reservations\n");
                break;
            case '7':
                printf("7. Exit program\n");
                break;
            case '0':
                printf("0. Exit\n");
                iExit = 1;
                break;
            default:
                printf("%c. Invalid input\n", chChoice);
                break;
        }
    }

}

struct reservation {
    struct reservation *pNext;
    struct reservation *pPrev;
    char szName[128];
    char szRoomNr[3];
    int iDate;
    int iDays;
    float fPricePerDay;
};

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
    char chFirstChar, chRestOfLine;
    int iValidInput = 0;
    int iStackIsEmpty = 0;

    chFirstChar = getchar();

    while(iStackIsEmpty == 0){
        chRestOfLine = getchar();
        if(chRestOfLine == '\n' || chRestOfLine == EOF){
            iStackIsEmpty = 1;
        }
    }
    return chFirstChar;
}

