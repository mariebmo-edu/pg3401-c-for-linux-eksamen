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
    int sockClientFd;
    struct sockaddr_in saClientAddr;
    char szServerResponse[BUFFER_SIZE];
    char szMessage[] = "GET / HTTP/1.1\r\n\r\n";

    if((sockClientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("ERROR opening socket");
        exit(1);
    }

    memset(&saClientAddr, 0, sizeof(saClientAddr));

    saClientAddr.sin_family = AF_INET;
    saClientAddr.sin_port = htons(PORT);
    saClientAddr.sin_addr.s_addr = inet_addr(ADDRESS);

    printf("Connecting to server %s:%d ...", ADDRESS, PORT);
    if(connect(sockClientFd, (struct sockaddr *) &saClientAddr, sizeof(saClientAddr)) < 0){
        perror("ERROR connecting to socket");
        exit(1);
    }

    printf("Connected to server %s:%d", ADDRESS, PORT);

    if(send(sockClientFd, szMessage, strlen(szMessage), 0) < 0){
        perror("ERROR sending message to socket");
        exit(1);
    }

    printf("Message sent to server");

    memset(szServerResponse, 0, BUFFER_SIZE);

    if(recv(sockClientFd, szServerResponse, BUFFER_SIZE, 0) < 0){
        perror("ERROR receiving message from socket");
        exit(1);
    }

    printf("Message received from server: %s", szServerResponse);

    close(sockClientFd);

    return 0;
}

/* End of file ------------------------------------------------------------------------------*/