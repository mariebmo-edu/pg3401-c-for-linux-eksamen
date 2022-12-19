//
// Created by 1012
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/oppgave2.h"

int main(int argc, char *argv[]){

    FILE *fileIn, *fileOut;
    char chCurrChar;

    // fileIn skal lese fra filen "input.txt". Hvis filen ikke finnes, avsluttes programmet.
    fileIn = fopen("input.txt", "r");

    if(fileIn == NULL){
        printf("Failed to open file for reading\n");
        exit(1);
    }

    // fileOut skal skrive til filen "output.txt". Hvis filen ikke finnes, opprettes den.
    fileOut = fopen("output.txt", "w");

    if(fileOut == NULL){
        printf("Failed to open file for writing\n");
        exit(1);
    }

    // Leser en char om gangen fra filen og skriver den til output.txt så lenge det er flere chars igjen. Vi skriver %x-verdien til tegnet, som er ASCII-verdien til en char.
    // Jeg har også valgt å skrive den ut, slik at det er lettere å se hva som skjer.
    chCurrChar = fgetc(fileIn);
    while(chCurrChar != EOF){
        fprintf(fileOut, "%x", chCurrChar);
        printf("%c : %x\n", chCurrChar, chCurrChar);
        chCurrChar = fgetc(fileIn);
    }

    // Når vi er ferdige med å lese og skrive fra filene, lukker vi dem og avslutter programmet.
    fclose(fileIn);
    fclose(fileOut);

    printf("Conversion complete.\n");
    return 0;
}