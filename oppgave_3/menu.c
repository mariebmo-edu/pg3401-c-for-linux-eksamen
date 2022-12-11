//
// Created by marie on 09.12.2022.
//

#include <stdio.h>
#include <string.h>

#include "include/menu.h"

void printHeader(char szHeader[], int length, char decorator){

    char decoratorLine[length+1];
    int spaceLength = (length/2 - strlen(szHeader)/2);
    char headerLine[spaceLength + 1];

    memset(decoratorLine, decorator, length);
    memset(headerLine, ' ', spaceLength);

    decoratorLine[length] = 0;
    headerLine[spaceLength] = 0;

    printf("%s\n", headerLine);

    printf("%s\n", decoratorLine);
    printf("%s%s\n", headerLine, szHeader);
    printf("%s\n", decoratorLine);
}

void printMenu(int length, char decorator){
    printHeader("Menu", length, decorator);
    printf("1. Add reservation\n");
    printf("2. Delete last reservation\n");
    printf("3. Delete all completed reservations\n");
    printf("4. Search by name\n");
    printf("5. Sum price of all reservations\n");
    printf("6. Print all reservations\n");
    printf("0. Exit program\n");
    printf("Enter choice: ");
}

void printSeparator(int length, char decorator){
    char decoratorLine[length+1];
    memset(decoratorLine, decorator, length);
    decoratorLine[length] = '\0';

    printf("%s\n", decoratorLine);
}