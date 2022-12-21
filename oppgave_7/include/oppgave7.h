//
// Created by 1012
//
#ifndef __OPPGAVE7_H__
#define __OPPGAVE7_H__

void createBeautifiedFileName(char *fileName, char *newFileName);
void addIndentation(FILE *fileOut, int iIndentation);
void forLoopToWhileLoop(char *line, regmatch_t *regMatches, FILE *fileOut, int *iIsInCurrentMatch, int *iIndentation, char *updateCondition, regex_t *regexForCurlyBraceEnd, int *iForLoopsFound);
void handleCodeBeforeLoop(char *beforeLoop, FILE *fileOut, int *iIndentation);
void replaceTabsWithSpacesInFile(FILE *file, int *iTabsFound);

#endif //__OPPGAVE7_H__
