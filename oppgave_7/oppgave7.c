//
// Created by 1012
//
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/oppgave7.h"

#define LINE_LENGTH 1024

int main(int argc, char *argv[]) {

    // Sjekker om det er gitt et filnavn som argument
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char fileName[strlen(argv[1])];
    char newFileName[(strlen(argv[1]) + 12)];
    regex_t regexForLoop, regexForTab, regexForCurlyBraceEnd;
    int iRegexResult, iIndentation;

    strcpy(fileName, argv[1]);
    createBeautifiedFileName(fileName, newFileName);

    //Regex matcher linjer med for-løkker
    // 0: whole line,
    // 1: characters before "for("
    // 2: initialisation statement (int i = 0)
    // 3: condition statement (i < 10)
    // 4: increment statement (i++)
    // 5: characters after "){"
    iRegexResult = regcomp(&regexForLoop, "(\\s*)for\\s*\\(\\s*([^;]*);\\s*([^;]*);\\s*([^\\)]*)\\)\\s*(\\{\\s*)", REG_EXTENDED);
    if (iRegexResult) {
        printf("Could not compile forloop regex");
        return 1;
    }

    //Regex matcher linjer med tab
    iRegexResult = regcomp(&regexForTab, "\\t", REG_EXTENDED);
    if (iRegexResult) {
        printf("Could not compile tab regex");
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
    FILE *fileOut = fopen(newFileName, "w+");

    if (fileIn == NULL) {
        printf("Could not open file %s\n", fileName);
        return 1;
    }

    if (fileOut == NULL) {
        printf("Could not open file %s\n", newFileName);
        return 1;
    }

    char line[LINE_LENGTH];
    char ch;
    char updateCondition[LINE_LENGTH];
    int iIsInCurrentMatch = 0;

    while(fgets(line, LINE_LENGTH, fileIn) != NULL) {

        regmatch_t regMatches[6];

        if(regexec(&regexForLoop, line, 6, regMatches, 0) == 0){

            size_t beforeLoopLength = regMatches[1].rm_eo - regMatches[1].rm_so;
            size_t initLength = regMatches[2].rm_eo - regMatches[2].rm_so;
            size_t conditionLength = regMatches[3].rm_eo - regMatches[3].rm_so;
            size_t updateLength = regMatches[4].rm_eo - regMatches[4].rm_so;
            size_t afterLoopLength = regMatches[5].rm_eo - regMatches[5].rm_so;

            char beforeLoop[beforeLoopLength+1];
            char init[initLength+1];
            char condition[conditionLength+1];
            char update[updateLength+1];
            char afterLoop[afterLoopLength+1];

            strncpy(beforeLoop, line + regMatches[1].rm_so, beforeLoopLength);
            strncpy(init, line + regMatches[2].rm_so, initLength);
            strncpy(condition, line + regMatches[3].rm_so, conditionLength);
            strncpy(update, line + regMatches[4].rm_so, updateLength);
            strncpy(afterLoop, line + regMatches[5].rm_so, afterLoopLength);

            beforeLoop[beforeLoopLength] = '\0';
            init[initLength] = '\0';
            condition[conditionLength] = '\0';
            update[updateLength] = '\0';
            afterLoop[afterLoopLength] = '\0';

            if(beforeLoop != NULL && strlen(beforeLoop) > 0){
                iIndentation = strlen(beforeLoop);
            }

            strncpy(updateCondition, update, updateLength+1);
            updateCondition[updateLength] = '\0';

            char initOutput[strlen(init) + 2];
            sprintf(initOutput, "%s%s", init, ";\n");

            char conditionOutput[strlen(condition) + 9];
            sprintf(conditionOutput, "%s%s%s", "while(", condition, "){\n");

            addIndentation(fileOut, iIndentation);
            fputs(initOutput, fileOut);
            addIndentation(fileOut, iIndentation);
            fputs(conditionOutput, fileOut);

            iIsInCurrentMatch++;
        } else {
            if(regexec(&regexForCurlyBraceEnd, line, 0, NULL, 0) == 0 && iIsInCurrentMatch > 0){
                addIndentation(fileOut, iIndentation+4);
                fputs(updateCondition, fileOut);
                fputs(";\n", fileOut);
                fputs(line, fileOut);
                iIsInCurrentMatch--;
                iIndentation -= 4;
            } else {
                fwrite(line, sizeof(char), strlen(line), fileOut);
            }
        }
    }

    regfree(&regexForLoop);
    regfree(&regexForTab);
    regfree(&regexForCurlyBraceEnd);
    fclose(fileIn);
    fclose(fileOut);

    printf("Beautified file %s created\n", newFileName);
    return 0;
}
void createBeautifiedFileName(char *fileName, char *newFileName) {
    int i = 0;
    while (fileName[i] != '.') {
        newFileName[i] = fileName[i];
        i++;
    }
    strcat(newFileName, "_beautified.c");
}

void addIndentation(FILE *fileOut, int iIndentation) {
    for(int i = 0; i < iIndentation; i++){
        fputc(' ', fileOut);
    }
}


