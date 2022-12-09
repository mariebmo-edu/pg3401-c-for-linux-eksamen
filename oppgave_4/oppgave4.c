//
// Created by marie on 09.12.2022.
//
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> //Fikser feil med bool
#include <ctype.h> //Fikser feil med isdigit & isalpha
#include <stdio.h>
#include "oppgave4.h"

typedef struct _MYHTTP {
    int iHttpCode;
    int iContentLength;
    bool bIsSuccess;
    char szServer[32]; //Servernavnet er lengre enn 16 char
    char szContentType[16];
} MYHTTP, *PMYHTTP;

MYHTTP *ProcessHttpHeader(char *pszHttp);

int main(int argc, char *argv[]) {

    //Fant en header på nettet som jeg brukte for å teste
    char szHttpHeader[] = "HTTP/1.1 200 OK\nServer: nginx/1.14.0 (Ubuntu)\nContent-Type: text/html\nContent-Length: 1234\n\n";
    MYHTTP *pMyHttp =  ProcessHttpHeader(szHttpHeader);

    printf("HttpCode: %d\n", pMyHttp->iHttpCode);
    printf("ContentLength: %d\n", pMyHttp->iContentLength);
    printf("IsSuccess: %d\n", pMyHttp->bIsSuccess);
    printf("Server: %s\n", pMyHttp->szServer);
    printf("ContentType: %s\n", pMyHttp->szContentType);

    free(pMyHttp);
    pMyHttp = NULL;

    return 0;
}

MYHTTP *ProcessHttpHeader(char *pszHttp) {

    char* pszPtr;
    MYHTTP* pHttp = (MYHTTP*)malloc(sizeof(MYHTTP)); //Må være sizeof på MYHTTP, ikke pekeren til MYHTTP. Fikser buffer overflow

    if (pHttp == NULL) {
        printf("Error: Could not allocate memory for new reservation.\n");
        return NULL;
    }

    memset(pHttp, 0, sizeof(MYHTTP));

    // HTTP-kode
    pHttp->iHttpCode = atoi(pszHttp + strlen("HTTP/1.x "));
    if (pHttp->iHttpCode == 200) {
        pHttp->bIsSuccess = true;
    }

    // Server: xx
    pszPtr = strstr(pszHttp, "Server");
    if (pszPtr) {
        pszPtr += 6;

        while (!isalpha(pszPtr[0])){
            pszPtr++;
        }

        strchr(pszPtr, '\n')[0] = 0;
        strcpy(pHttp->szServer, pszPtr);
        pszPtr[strlen(pHttp->szServer)] = '\n';
    }

    pszPtr = strstr(pszHttp, "Content-Type");
    if (pszPtr) {
        pszPtr += 12;

        while (!isalpha(pszPtr[0])){
            pszPtr++;
        }

        strchr(pszPtr, '\n')[0] = 0;
        strncpy(pHttp->szContentType, pszPtr, 15); // Skal være strncpy, ikke strcpy.
        pszPtr[strlen(pHttp->szContentType)] = '\n';
    }

    // Content-Length: xx
    pszPtr = strstr(pszHttp, "Content-Length");
    if (pszPtr) {
        pszPtr += 14;
        while (!isdigit(pszPtr[0])){
            pszPtr++;
        }

        pHttp->iContentLength = atoi(pszPtr); //dette er en int, ikke en char, så vi fjerner '0' fra atoi
    }

    return pHttp;
}