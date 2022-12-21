//
// Created by 1012
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/reservation.h"
#include "include/menu.h"
#include "include/oppgave3.h"
#include "include/dateTimeHandling.h"

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

    // En while-løkke som håndterer menyvalg fra brukeren. Kjører helt til brukeren velger å avslutte programmet.
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

    // Når brukeren har valgt å avslutte programmet, frigjøres minnet som er brukt til å lagre reservasjonene.
    clearAllocation(&pHead, &pTail);
} // End main

// Metode for å kun lese inn første tegn fra brukerinput.
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

// Metode for å validere at brukerinput er et valid årstall.
int validateYearInput(int year) {
    if (year < 1000) {
        printf("Year must be 4 digits\n");
        return 1;
    }

    if (year > 9999) {
        printf("Year must be less than 9999\n");
        return 1;
    }

    return 0;
}

// Metode for å validere at brukerinput er en valid måned.
int validateMonthInput(int month) {
    if (month < 1 || month > 12) {
        printf("Month must be between 1-12\n");
        return 1;
    }

    return 0;
}

// Metode for å validere at brukerinput er en valid dag. Tar inn måned for å validere at dag er gyldig for måneden. OBS!! tar ikke hensyn til skuddår.
int validateDayInput(int iDay, int iMonth, int iYear) {

    if (iDay < 1) {
        printf("Must be a valid day\n");
        return 1;
    }

    // Validerer at dag er gyldig for februar.
    if(iMonth == 2 && iDay > 28 || (iMonth == 2 && iDay == 29 && isLeapYear(iYear) == 0)){
        printf("February can only have 28 days\n");
        printf("Month: %d, Day: %d\n", iMonth, iDay);
        return 1;
    }

    // Validerer at dag er gyldig for måneder med 30 dager.
    if((iMonth == 4  || iMonth == 6 || iMonth == 9 || iMonth == 11) && iDay > 30){
        printf("Day must be 30 or less in April, June, September and November\n");
        return 1;
    }

    // Validerer at dag er gyldig for måneder med 31 dager.
    if(iDay > 31){
        printf("Day must be 31 or less\n");
        return 1;
    }

    return 0;
}

// Sjekker at iDateInput er et gyldig datoformat
int validateDateInput(unsigned int iDateInput){
    struct tm time = getTmFromYYYYMMDD(iDateInput);

    int iYear = time.tm_year + 1900;
    int iMonth = time.tm_mon + 1;
    int iDay = time.tm_mday;

    if(validateYearInput(iYear) == 1){
        return 1;
    }

    if(validateMonthInput(iMonth) == 1){
        return 1;
    }

    if(validateDayInput(iDay, iMonth, iYear) == 1){
        return 1;
    }

    return 0;
}

// Meny for å legge til dato for en reservasjon, for å gjøre det mer brukervennlig enn å skrive 20201212 for 12. desember 2020.
// Metoden returnerer en dato som en int på formatet YYYYMMDD.
unsigned int addDateMenu(int *iDateInput){
    char szDate[9];
    char szYear[5];
    char szMonth[3];
    char szDay[3];
    char *pChar;
    int iValidInput = 0;

    // En while-løkke som kjører helt til brukeren har skrevet inn en gyldig årstall.
    printf("Date of arrival\n");
    printf("Enter a year (YYYY): ");
    while(!iValidInput){
        if(input(szYear, sizeof(szYear)) == 1){
            printf("Year is too long. Enter a year (YYYY): ");
        } else {
            if(validateYearInput(atoi(szYear)) == 0){
                iValidInput = 1;
                szDate[0] = szYear[0];
                szDate[1] = szYear[1];
                szDate[2] = szYear[2];
                szDate[3] = szYear[3];
            }
        }
    }

    // En while-løkke som kjører helt til brukeren har skrevet inn en gyldig måned.
    iValidInput = 0;
    printf("Enter a month (MM): \n");
    printf("1. January\n");
    printf("2. February\n");
    printf("3. March\n");
    printf("4. April\n");
    printf("5. May\n");
    printf("6. June\n");
    printf("7. July\n");
    printf("8. August\n");
    printf("9. September\n");
    printf("10. October\n");
    printf("11. November\n");
    printf("12. December\n");

    while(!iValidInput){
        if(input(szMonth, sizeof(szMonth)) == 1){
            printf("Month is too long. Enter a month (MM): ");
        } else if (validateMonthInput(atoi(szMonth)) == 0){
            if(atoi(szMonth) < 10){
                szDate[4] = '0';
                szDate[5] = szMonth[0];
            } else {
                szDate[4] = szMonth[0];
                szDate[5] = szMonth[1];
            }
            iValidInput = 1;
        }
    }

    // En while-løkke som kjører helt til brukeren har skrevet inn en gyldig dag.
    iValidInput = 0;
    printf("Enter a day (DD): ");
    while(!iValidInput){
        if(input(szDay, sizeof(szDay)) == 1){
            printf("Day is too long. Enter a day (DD): ");
        } else {
            if(validateDayInput(atoi(szDay), atoi(szMonth), atoi(szYear)) == 0){
                if(atoi(szDay) < 10){
                    szDate[6] = '0';
                    szDate[7] = szDay[0];
                } else {
                    szDate[6] = szDay[0];
                    szDate[7] = szDay[1];
                }
                iValidInput = 1;
            } else {
                printf("Invalid day input. Enter a day (DD): ");
            }
        }
    }

    szDate[8] = '\0';

    // Gjør om dato fra char til int, og sjekker om det er en gyldig dato. Hvis ikke returnerer metoden 0, som gjør at den blir kjørt på nytt.
    *iDateInput = (unsigned int) strtoul(szDate, &pChar, 10);

    if(validateDateInput(*iDateInput) == 0){
        return 0;
    } else {
        printf("Invalid date input. Please try again\n");
        return 1;
    }
}

// Meny for å legge til en reservasjon.
void addReservationMenu(struct reservation **ppHead, struct reservation **ppTail){
    char szName[128];
    char szRoomNr[8];
    char szBuffer[8];

    unsigned int iDate;
    int iDays;
    float fPricePerDay;
    int iValidInput;

    // En while-løkke som kjører helt til brukeren har skrevet inn en gyldig navn.
    iValidInput = 0;
    printf("Enter name: ");
    while(!iValidInput){
        if(input(szName, sizeof(szName)) == 1){
            printf("Name is too long. Enter name: ");
        } else {
            iValidInput = 1;
        }
    }

    // En while-løkke som kjører helt til brukeren har skrevet inn en gyldig romnummer.
    iValidInput = 0;
    printf("Enter room number: ");
    while (!iValidInput) {
        if(input(szRoomNr, sizeof(szRoomNr)) == 1){
            printf("Room number is too long. Enter room number: ");
        } else {
            iValidInput = 1;
        }
    }

    // En while-løkke som kjører helt til brukeren har skrevet inn en gyldig dato.
    while(addDateMenu(&iDate) != 0);

    // En while-løkke som kjører helt til brukeren har skrevet inn en gyldig antall dager.
    iValidInput = 0;
    printf("Enter number of days: ");
    while (!iValidInput){
        if(input(szBuffer, sizeof(szBuffer)) == 1){
            printf("Number of days is too long. Enter number of days: ");
        } else {
            if(atoi(szBuffer) > 0){
                iDays = atoi(szBuffer);
                iValidInput = 1;
            } else {
                printf("Invalid number of days input. Enter number of days: ");
            }
        }
    }

    // En while-løkke som kjører helt til brukeren har skrevet inn en gyldig pris per dag.
    iValidInput = 0;
    printf("Enter price per day: ");
    while(!iValidInput){
        if(input(szBuffer, sizeof(szBuffer)) == 1){
            printf("Price per day is too long. Enter price per day: ");
        } else {
            if(atof(szBuffer) > 0){
                fPricePerDay = atof(szBuffer);
                iValidInput = 1;
            } else {
                printf("Invalid price per day input. Enter price per day: ");
            }
        }
    }

    // Legger til reservasjonen i listen. Hvis listen er tom, opprettes den første noden. Hvis ikke, legges den til bakerst i listen.
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

// Meny for å søke etter en reservasjon basert på navn.
void searchByNameMenu(struct reservation **ppHead){
    char szName[129];

    printf("Enter name: ");
    if(input(szName, sizeof(szName)) == 1){
        return;
    }

    searchReservationByName(ppHead, szName);
}

// Metode for å håndtere og validere input fra brukeren.
int input(char *szInput, int iLength){

    int c;
    char *input = fgets(szInput, iLength, stdin);

    int i = 0;
    int iInWhile = 1;

    // Ser om det er "new line" eller "carriage return" i inputen. Hvis det er det, så setter den det til '\0'.
    while(iInWhile && i < iLength){

        if(szInput[i] == '\n' || szInput[i] == '\r'){
            szInput[i] = '\0';
            iInWhile = 0;
        }
        i++;
    }

    // Sjekker om inputen er for lang, og fjerner resten av inputen.
    i = strlen(szInput);
    iInWhile = 1;
    while(iInWhile) {
        c = getchar();
        i++;
        if (c == '\n' || c == EOF) {
            iInWhile = 0;
        }
    }

    // Hvis inputen er NULL eller for lang, returnerer metoden 1, som betyr at den skal kjøres på nytt.
    if(input == NULL){
        return 1;
    } else if (i > iLength){
        return 1;
    } else {
        return 0;
    }
}