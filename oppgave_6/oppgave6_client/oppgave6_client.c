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

int main(int argc, char *argv[]){
    int sockClientFd;
    struct sockaddr_in saClientAddr;
    char szServerResponse[BUFFER_SIZE];
    char szFileName[128] = "index.html";
    char szMessage[256];
    unsigned long ulBytesReceived;

    if(argv[1] != NULL){
        strcpy(szFileName, argv[1]);
    }

    sprintf(szMessage, "GET /%s HTTP/1.1\r\n", szFileName);

    printf("Starting client %s:%d ...\n", ADDRESS, PORT);

    if((sockClientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("ERROR opening socket");
        exit(1);
    }

    memset(&saClientAddr, 0, sizeof(saClientAddr));

    saClientAddr.sin_family = AF_INET;
    saClientAddr.sin_port = htons(PORT);
    saClientAddr.sin_addr.s_addr = inet_addr(ADDRESS);

    printf("Connecting to server %s:%d ...\n", ADDRESS, PORT);
    if(connect(sockClientFd, (struct sockaddr *) &saClientAddr, sizeof(saClientAddr)) < 0){
        perror("ERROR connecting to socket\n");
        exit(1);
    }

    printf("Connected to server %s:%d", ADDRESS, PORT);

    if(send(sockClientFd, szMessage, strlen(szMessage), 0) < 0){
        perror("ERROR sending message to socket\n");
        exit(1);
    }

    printf("Message sent to server\n");

    memset(szServerResponse, 0, BUFFER_SIZE);

    if(recv(sockClientFd, szServerResponse, BUFFER_SIZE, 0) < 0){
        perror("ERROR receiving message from socket\n");
        exit(1);
    }

    printf("Message received from server:\n%s\n", szServerResponse);
    sscanf(szServerResponse, "Content-Length: %lu\n", &ulBytesReceived);
    printf("Bytes received: %lu\n", ulBytesReceived);


    FILE *fp = fopen(szFileName, "w+");

    if(fp == NULL){
        perror("ERROR opening file\n");
        exit(1);
    }

    unsigned long ulBytesRemaining = ulBytesReceived;
    char fileContent[BUFFER_SIZE];
    int iBytesRead;

    while (ulBytesRemaining > 0 && (iBytesRead = read(sockClientFd, fileContent, BUFFER_SIZE)) > 0) {
        printf("Bytes read: %d\n", iBytesRead);
        fwrite(fileContent, 1, iBytesRead, fp);
        ulBytesRemaining -= iBytesRead;
    }

    fclose(fp);
    printf("Message received from server: %s\n", szServerResponse);

    close(sockClientFd);

    return 0;
}