//
// Created by 1012
//
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/oppgave7.h"

#define BUFFER_SIZE 1024
#define TAB_LENGTH 3
#define INDENTATION_LENGTH 4

int main(int argc, char *argv[]) {

    // Sjekker om det er gitt et filnavn som argument
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char ch;
    char updateCondition[BUFFER_SIZE]; // Brukes for å mellomlagre oppdateringsdelen av for-løkken.
    regex_t regexForLoop, regexForCurlyBraceEnd;
    int iRegexResult, iIndentation = 0, iForLoopsFound = 0, iForLoopNumber = 0, iTabsFound = 0;

    // Åpner filen som skal leses. Gir midlertidig navn til filen som skal skrives.
    char fileName[strlen(argv[1])];
    strcpy(fileName, argv[1]);


    //Regex matcher linjer med for-løkker. Kan inneholde så mange mellomrom man ønsker mellom dele i forløkka.
    // 0: whole line,
    // 1: characters before "for("
    // 2: initialisation statement (int i = 0)
    // 3: condition statement (i < 10)
    // 4: increment statement (i++)
    // 5: characters after "){"
    // 6: characters after "} "
    iRegexResult = regcomp(&regexForLoop, "(.*)for\\s*\\(\\s*([^;]+);\\s*([^;]+);\\s*([^\\)]+)\\)\\s*\\{\\s*([^}]*)\\}*\\s*(.*)", REG_EXTENDED);
    if (iRegexResult) {
        printf("Could not compile forloop regex");
        return 1;
    }

    //Regex matcher linjer med }
    iRegexResult = regcomp(&regexForCurlyBraceEnd, "\\s*\\}\\s*", REG_EXTENDED);
    if (iRegexResult) {
        printf("Could not compile curly braces regex");
        return 1;
    }

    //Åpner filen som skal leses, og filen som skal skrives til
    FILE *fileIn = fopen(fileName, "r");


    if (fileIn == NULL) {
        printf("Could not open file %s\n", fileName);
        return 1;
    }

    char newFileName[(strlen(fileName) + 12)], buffer[BUFFER_SIZE];
    memset(newFileName, 0, sizeof(newFileName));

    // Legger til _beautified på slutten av filnavnet som kommer inn.
    createBeautifiedFileName(fileName, newFileName);

    FILE *fileOut = fopen(newFileName, "w+");

    if (fileOut == NULL) {
        printf("Could not open file %s\n", newFileName);
        return 1;
    }

    // Is In CurrentMatch holder styr på om hvilken forløkke vi er inne i ved nested-løkker.
    iForLoopNumber = 0;

    // Går gjennom fila i buffer-chunks.
    while(fgets(buffer, BUFFER_SIZE, fileIn) != NULL) {

        regmatch_t regMatches[7];
        // Sjekker om linjen inneholder en for-løkke.
        if(regexec(&regexForLoop, buffer, 7, regMatches, 0) == 0){
            // Hvis den inneholder en for-løkke, formatteres den om til en while-løkke.
            forLoopToWhileLoop(buffer, regMatches, fileOut, &iForLoopNumber, &iIndentation, updateCondition, &regexForCurlyBraceEnd, &iForLoopsFound);
        } else {
            // Hvis den ikke inneholder en for-løkke, sjekker vi først om den inneholder en }, og er inne i en forløkke.
            // Det er dermed slutten på en forløkke, og vi ønsker å legge til updateCondition (f.eks i++), og legge til en }.
            if(regexec(&regexForCurlyBraceEnd, buffer, 0, NULL, 0) == 0 && iForLoopNumber > 0){
                addIndentation(fileOut, iIndentation+INDENTATION_LENGTH);
                fputs(updateCondition, fileOut);
                fputs(";\n", fileOut);
                fputs(buffer, fileOut);
                iForLoopNumber--;
                iIndentation -= INDENTATION_LENGTH;
            } else {
                // Hvis den ikke inneholder en for-løkke, og ikke er slutten på en for-løkke, skrives den bare over til den nye fila.
                fwrite(buffer, sizeof(char), strlen(buffer), fileOut);
            }
        }
    }

    // Til slutt går vi gjennom hele fila og endrer tabs til mellomrom.
    replaceTabsWithSpacesInFile(fileOut, &iTabsFound);

    regfree(&regexForLoop);
    regfree(&regexForCurlyBraceEnd);
    fclose(fileIn);
    fclose(fileOut);

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("File %s has been beautified and saved as %s\n", fileName, newFileName);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Number of for-loops converted: %d\n", iForLoopsFound);
    printf("Number of tabs replaced: %d\n", iTabsFound);
    printf("--------------------------------------------------------------------------------------------------------------------------------\n");

    return 0;
}

//Henter inn filnavnet, legger inn _beautified, og legger på extension.
void createBeautifiedFileName(char *fileName, char *newFileName) {

    int i = 0;
    int iDoneWithFileName = 0; // Brukes for å holde styr på om vi er ferdig med filnavnet, og skal legge til _beautified.

    while (i < strlen(fileName)) {
        // Sjekker om vi har kommet til filendelsen. Legger i såfall til _beautified.
        if(fileName[i] == '.') {
            iDoneWithFileName = 1;
            strcat(newFileName, "_beautified");
        }

        // Hvis vi ikke er ferdig med filnavnet, legger vi til bokstaven, ellers legger vi til filendelsen.
        if(!iDoneWithFileName){
            newFileName[i] = fileName[i];
        } else {
            newFileName[i+11] = fileName[i];
        }
        i++;
    }
}

// Legger til mellomrom i fila for å indentere.
void addIndentation(FILE *fileOut, int iIndentation) {
    for(int i = 0; i < iIndentation; i++){
        fputc(' ', fileOut);
    }
}

// Erstatter forløkker med while-løkker.
void forLoopToWhileLoop(char *buffer, regmatch_t *regMatches, FILE *fileOut, int *iForLoopNumber, int *iIndentation, char *updateCondition, regex_t *regexForCurlyBraceEnd, int *iForLoopsFound) {

    size_t beforeLoopLength = regMatches[1].rm_eo - regMatches[1].rm_so; // Lengden på det som kommer før for-løkken.
    size_t initLength = regMatches[2].rm_eo - regMatches[2].rm_so; // Lengden på initialiseringen av for-løkken. (f.eks int i = 0)
    size_t conditionLength = regMatches[3].rm_eo - regMatches[3].rm_so; // Lengden på betingelsen i for-løkken. (f.eks i < 10)
    size_t updateLength = regMatches[4].rm_eo - regMatches[4].rm_so; // Lengden på oppdateringen av for-løkken. (f.eks i++)
    size_t withinForLoopLength = regMatches[5].rm_eo - regMatches[5].rm_so; // Lengden på det som er inni for-løkken. (f.eks { ... })
    size_t afterLoopLength = regMatches[6].rm_eo - regMatches[6].rm_so; // Lengden på det som kommer etter for-løkken.

    // Initialiserer variabler som skal holde på de forskjellige delene av for-løkken.
    char beforeLoop[beforeLoopLength+1];
    char init[initLength+1];
    char condition[conditionLength+1];
    char update[updateLength+1];
    char withinForLoop[withinForLoopLength+1];
    char afterLoop[afterLoopLength+1];

    // Kopierer de forskjellige delene av for-løkken til de respektive variablene.
    strncpy(beforeLoop, buffer + regMatches[1].rm_so, beforeLoopLength);
    strncpy(init, buffer + regMatches[2].rm_so, initLength);
    strncpy(condition, buffer + regMatches[3].rm_so, conditionLength);
    strncpy(update, buffer + regMatches[4].rm_so, updateLength);
    strncpy(withinForLoop, buffer + regMatches[5].rm_so, withinForLoopLength);
    strncpy(afterLoop, buffer + regMatches[6].rm_so, afterLoopLength);

    // Legger til \0 på slutten av strengene for å nullterminere dem.
    beforeLoop[beforeLoopLength] = '\0';
    init[initLength] = '\0';
    condition[conditionLength] = '\0';
    update[updateLength] = '\0';
    withinForLoop[withinForLoopLength] = '\0';
    afterLoop[afterLoopLength] = '\0';


    // Kopierer oppdateringsdelen av for-løkken til en variabel som skal brukes til å legge til etter løkken.
    strncpy(updateCondition, update, updateLength+1);
    updateCondition[updateLength] = '\0';

    // Håndterer indentasjonen og skriver ut det som kommer før for-løkken.
    handleCodeBeforeLoop(beforeLoop, fileOut, iIndentation);

    // Trekker ut variabelnavnet fra initialiseringen av for-løkken.
    char initOutput[strlen(init) + 2];
    sprintf(initOutput, "%s%s", init, ";\n");

    // Legger en while-løkke rundt betingelsen i for-løkken.
    char conditionOutput[strlen(condition) + 9];
    sprintf(conditionOutput, "%s%s%s", "while(", condition, "){\n");

    // Legger til indentasjonen og skriver ut initialiseringen og betingelsen.
    addIndentation(fileOut, *iIndentation);
    fputs(initOutput, fileOut);
    addIndentation(fileOut, *iIndentation);
    fputs(conditionOutput, fileOut);

    // Oppdaterer løkkenummer
    iForLoopNumber++;

    // Initialiserer variabler som skal holde på de forskjellige delene av det som er inne i, etter, og oppdateringen av for-løkken.
    char withinForLoopBuffer[strlen(withinForLoop) + 1];
    char updateConditionBuffer[strlen(updateCondition) + 1];
    char afterLoopBuffer[strlen(afterLoop) + 1];

    //Hvis det er en } på samme linje som forløkken, så er forløkken på en linje. Vi håndterer det her.
    if(regexec(regexForCurlyBraceEnd, buffer, 0, NULL, 0) == 0 && iForLoopNumber > 0) {

        // Legger INDENTATION_LENGTH i indentation, da vi skal ha innrykk inne i forløkken.
        addIndentation(fileOut, *iIndentation += INDENTATION_LENGTH);

        // Legger til koden inni forløkken.
        sprintf(withinForLoopBuffer, "%s%s", withinForLoop, "\n");
        fputs(withinForLoopBuffer, fileOut);

        // Legger til indentasjonen til forløkken.
        addIndentation(fileOut, *iIndentation);

        // Legger til oppdateringskoden. (f.eks i++)
        sprintf(updateConditionBuffer, "%s%s", updateCondition, ";\n");
        fputs(updateConditionBuffer, fileOut);

        // Legger til indentasjonen til forløkken.
        addIndentation(fileOut, *iIndentation -= INDENTATION_LENGTH);

        // Legger til } som avslutter forløkken.
        fputs("}\n", fileOut);

        // Ser om det er noe kode utenfor forløkken, og legger den til.
        if(strlen(afterLoop) > 0 && afterLoop[0] != '\r' && afterLoop[0] != '\n') {
            addIndentation(fileOut, *iIndentation);
            fputs(afterLoop, fileOut);
            fputc('\n', fileOut);

        }

        // Siden løkken er ferdig, trekker vi fra 1 fra løkkenummeret.
        iForLoopNumber--;
    }

    *iForLoopsFound += 1;
}

// Håndterer det som kommer før for-løkken.
void handleCodeBeforeLoop(char *beforeLoop, FILE *fileOut, int *iIndentation) {

    // Separerer whitespace fra tekst.
    regex_t regexForWhiteSpace;
    int iRegexResult = regcomp(&regexForWhiteSpace, "(\\s*)(.*)", REG_EXTENDED);
    if (iRegexResult) {
        printf("Could not compile tab regex");
        exit(1);
    }

    regmatch_t regMatches[3];

    // Hvis det er noe som kommer før for-løkken, så håndterer vi det her.
    if(regexec(&regexForWhiteSpace, beforeLoop, 3, regMatches, 0) == 0){

        size_t indentationLength = regMatches[1].rm_eo - regMatches[1].rm_so; // Henter ut lengden på whitespace.
        size_t beforeLoopTextLength = regMatches[2].rm_eo - regMatches[2].rm_so; // Henter ut lengden på teksten.

        // Initialiserer variabler som skal holde på whitespace og teksten.
        char szIndentation[indentationLength+1];
        char szBeforeLoop[beforeLoopTextLength+1];

        // Kopierer whitespace og teksten til variablene.
        strncpy(szIndentation, beforeLoop + regMatches[1].rm_so, indentationLength);
        strncpy(szBeforeLoop, beforeLoop + regMatches[2].rm_so, beforeLoopTextLength);

        // Legger til \0 på slutten av strengene for å nullterminere dem.
        szIndentation[indentationLength] = '\0';
        szBeforeLoop[beforeLoopTextLength] = '\0';

        // Hvis det er whitespace, så legger vi til indentasjonen.
        if(strlen(szIndentation) > 0){

            if(strlen(szIndentation) < INDENTATION_LENGTH){
                *iIndentation = INDENTATION_LENGTH;
            } else {
                *iIndentation = strlen(szIndentation);
            }
        }

        // Hvis det er tekst, så legger vi til indentasjonen og teksten.
        if(strlen(szBeforeLoop) > 0){
            addIndentation(fileOut, *iIndentation);
            fputs(szBeforeLoop, fileOut);
            fputs("\n", fileOut);
        }
    }

    // Frigjør minnet som ble brukt av regexen.
    regfree(&regexForWhiteSpace);
}

// Håndterer tabs, og gjør om de til spaces.
void replaceTabsWithSpacesInFile(FILE *file, int *iTabsFound){
    char chCurrent;
    int i, counter;

    // henter ut første tegn i filen.
    chCurrent = fgetc(file);

    // Så lenge det er tegn igjen i filen, så kjører vi.
    while(chCurrent != EOF){

        // Hvis det er en tab, så erstatter vi den med spaces.
        if(chCurrent == '\t'){
            // Vi legger til 3 spaces for hver tab. Vi må også kunne håndtere at det er flere tabs etter hverandre.
            int iNumOfSpaces = TAB_LENGTH - (counter % TAB_LENGTH);
            for(i = 0; i < iNumOfSpaces; i++){
                fputc(' ', file);
            }
            counter += iNumOfSpaces;
            *iTabsFound++;
        } else {
            fputc(chCurrent, file);
            counter++;
        }

        // Hvis det er en linjeskift, så nullstiller vi telleren.
        if(chCurrent == '\n'){
            counter = 0;
        }

        chCurrent = fgetc(file);
    }

    // Går tilbake til starten av filen i tilfelle vi skal bruke den igjen.
    rewind(file);
}
