//
// Created by 1012
//
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/oppgave7.h"

#define LINE_LENGTH 1024
#define TAB_LENGTH 3

int main(int argc, char *argv[]) {

    // Sjekker om det er gitt et filnavn som argument
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char ch;
    char fileName[strlen(argv[1])], newFileName[(strlen(argv[1]) + 12)], line[LINE_LENGTH], updateCondition[LINE_LENGTH];
    regex_t regexForLoop, regexForTab, regexForCurlyBraceEnd;
    int iRegexResult, iIndentation, iForLoopsFound, iIsInCurrentMatch;

    strcpy(fileName, argv[1]);
    createBeautifiedFileName(fileName, newFileName);

    //Regex matcher linjer med for-løkker
    // 0: whole line,
    // 1: characters before "for("
    // 2: initialisation statement (int i = 0)
    // 3: condition statement (i < 10)
    // 4: increment statement (i++)
    // 5: characters after "){"
    iRegexResult = regcomp(&regexForLoop, "(.*)for\\s*\\(\\s*([^;]+);\\s*([^;]+);\\s*([^\\)]+)\\)\\s*\\{\\s*([^}]*)", REG_EXTENDED);
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


    iIsInCurrentMatch = 0;

    while(fgets(line, LINE_LENGTH, fileIn) != NULL) {

        regmatch_t regMatches[6];
        if(regexec(&regexForLoop, line, 6, regMatches, 0) == 0){
            forLoopToWhileLoop(line, regMatches, fileOut, &iIsInCurrentMatch, &iIndentation, updateCondition, &regexForCurlyBraceEnd);
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

    replaceTabsWithSpacesInFile(fileOut);

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
    while (fileName[i] != '.' && i < strlen(fileName)) {
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

void forLoopToWhileLoop(char *line, regmatch_t *regMatches, FILE *fileOut, int *iIsInCurrentMatch, int *iIndentation, char *updateCondition, regex_t *regexForCurlyBraceEnd) {

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
        *iIndentation = strlen(beforeLoop);
    }

    strncpy(updateCondition, update, updateLength+1);
    updateCondition[updateLength] = '\0';

    handleCodeBeforeLoop(beforeLoop, fileOut, iIndentation);

    char initOutput[strlen(init) + 2];
    sprintf(initOutput, "%s%s", init, ";\n");

    char conditionOutput[strlen(condition) + 9];
    sprintf(conditionOutput, "%s%s%s", "while(", condition, "){\n");

    addIndentation(fileOut, *iIndentation);
    fputs(initOutput, fileOut);
    addIndentation(fileOut, *iIndentation);
    fputs(conditionOutput, fileOut);

    iIsInCurrentMatch++;

    char afterLoopBuffer[strlen(afterLoop) + 1];
    char updateConditionBuffer[strlen(updateCondition) + 1];
    char outputBuffer[LINE_LENGTH];

    //Hvis det er en } på samme linje som forløkken, så er forløkken på en linje. Vi håndterer det her.
    if(regexec(regexForCurlyBraceEnd, line, 0, NULL, 0) == 0 && iIsInCurrentMatch > 0) {
        addIndentation(fileOut, *iIndentation += 4);
        sprintf(outputBuffer, "%s%s", afterLoop, "\n");
        fputs(outputBuffer, fileOut);
        addIndentation(fileOut, *iIndentation);
        sprintf(updateConditionBuffer, "%s%s", updateCondition, ";\n");
        fputs(updateConditionBuffer, fileOut);
        addIndentation(fileOut, *iIndentation -= 4);
        fputs("}\n", fileOut);
        iIsInCurrentMatch--;
    }
}

void handleCodeBeforeLoop(char *beforeLoop, FILE *fileOut, int *iIndentation) {

    regex_t regexForWhiteSpace;
    int iRegexResult = regcomp(&regexForWhiteSpace, "(\\s*)(.*)", REG_EXTENDED);
    if (iRegexResult) {
        printf("Could not compile tab regex");
        exit(1);
    }

    regmatch_t regMatches[3];
    if(regexec(&regexForWhiteSpace, beforeLoop, 3, regMatches, 0) == 0){
        size_t indentationLength = regMatches[1].rm_eo - regMatches[1].rm_so;
        size_t beforeLoopTextLength = regMatches[2].rm_eo - regMatches[2].rm_so;

        char szIndentation[indentationLength+1];
        char szBeforeLoop[beforeLoopTextLength+1];

        strncpy(szIndentation, beforeLoop + regMatches[1].rm_so, indentationLength);
        strncpy(szBeforeLoop, beforeLoop + regMatches[2].rm_so, beforeLoopTextLength);

        szIndentation[indentationLength] = '\0';
        szBeforeLoop[beforeLoopTextLength] = '\0';

        if(strlen(szIndentation) > 0){

            if(strlen(szIndentation) < 4){
                *iIndentation = 4;
            } else {
                *iIndentation = strlen(szIndentation);
            }
        }

        if(strlen(szBeforeLoop) > 0){
            addIndentation(fileOut, *iIndentation);
            fputs(szBeforeLoop, fileOut);
            fputs("\n", fileOut);
        }
    }

    regfree(&regexForWhiteSpace);
}

void replaceTabsWithSpacesInFile(FILE *file){
    char chCurrent;
    int i, counter, iHaveFoundTab = 0;

    chCurrent = fgetc(file);
    while(chCurrent != EOF){
        if(chCurrent == '\t'){
            int iNumOfSpaces = TAB_LENGTH - (counter % TAB_LENGTH);
            for(i = 0; i < iNumOfSpaces; i++){
                fputc(' ', file);
            }
            counter += iNumOfSpaces;
            iHaveFoundTab = 1;
        } else {
            fputc(chCurrent, file);
            counter++;
        }

        if(chCurrent == '\n'){
            counter = 0;
        }

        chCurrent = fgetc(file);
    }

    if(iHaveFoundTab){
        printf("Tabs have been replaced with spaces in the file.\n");
    } else {
        printf("No tabs found in the file.\n");
    }
    rewind(file);
}
