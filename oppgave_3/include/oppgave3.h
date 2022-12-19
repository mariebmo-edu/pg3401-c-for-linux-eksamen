//
// Created by 1012
//
#ifndef __OPPGAVE3_H__
#define __OPPGAVE3_H__

int handleMenu();
char getFirstCharOfUserInput();
void addReservationMenu(struct reservation **ppHead, struct reservation **ppTail);
void searchByNameMenu(struct reservation **ppHead);
void clearInputStack();
void handleNewLine(char *szInput, int iLength);
int input(char *szInput, int iLength);

#endif //__OPPGAVE3_H__
