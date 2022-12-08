//
// Created by 1012 on 08.12.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/oppgave2.h"

int main(int argc, char *argv[]){

    FILE *fileIn, *fileOut;
    char cCurrChar;

    fileIn = fopen("input.txt", "r");

    if(fileIn == NULL){
        printf("Failed to open file for reading\n");
        return 0;
    }

    fileOut = fopen("output.txt", "w");

    if(fileOut == NULL){
        printf("Failed to open file for writing\n");
        return 0;
    }

    cCurrChar = fgetc(fileIn);
    while(cCurrChar != EOF){
        fprintf(fileOut, "%x", cCurrChar);
        printf("%c : %x\n", cCurrChar, cCurrChar);
        cCurrChar = fgetc(fileIn);
    }

    fclose(fileIn);
    fclose(fileOut);

    printf("Conversion complete.\n");
}