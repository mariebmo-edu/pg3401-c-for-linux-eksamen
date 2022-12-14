//
// Created by marie on 12.12.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h> // Defines the sockaddr_in on Debian
#include <unistd.h> // Defines close function on Debian

#include "include/oppgave6_client.h"

#define PORT 1234 // Port number to use
#define ADDRESS "127.0.0.1" // Address to use
#define BUFFER_SIZE 1024 // Size of buffer to use

int main(int iArgc, char *apszArgv[]){
    int sockFd;
    struct sockaddr_in saAddr;

    if((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("ERROR opening socket");
        exit(1);
    }

    memset(&saAddr, 0, sizeof(saAddr));

    saAddr.sin_family = AF_INET;
    saAddr.sin_port = htons(PORT);
    saAddr.sin_addr.s_addr = inet_addr(ADDRESS);

    if(connect(sockFd, (struct sockaddr *) &saAddr, sizeof(saAddr)) < 0){
        perror("ERROR connecting to socket");
        exit(1);
    }

    ReceiveHttpGetReply(sockFd);

    close(sockFd);

    return 0;
}

void ReceiveHttpGetReply(int sockFd) {
    char szBuffer[BUFFER_SIZE];
    int n;

    memset(szBuffer, 0, BUFFER_SIZE);
    n = 0;

    while((szBuffer[n++] = getchar()) != EOF);

    if(write(sockFd, szBuffer, strlen(szBuffer)) < 0){
        perror("ERROR writing to socket");
        exit(1);
    }

    memset(szBuffer, 0, BUFFER_SIZE);
    n = read(sockFd, szBuffer, BUFFER_SIZE);
    printf("From server: %s\n", szBuffer);
    if((strncmp(szBuffer, "exit", 4)) == 0){
        printf("Client Exit...\n");
    }
}

/* End of file ------------------------------------------------------------------------------*/