//
// Created by marie on 12.12.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "include/oppgave6_server.h"

#define PORT 1234 // Port number to use
#define ROOT_DIR "." // Current directory is the root
#define ADDRESS "127.0.0.1" // Address to use
#define BUFFER_SIZE 1024 // Size of buffer to use

int main(int argc, char *argv[]) {

    char webpage[] = "HTTP/1.1 200 OK\r\n Content-Type: text/html; charset=UTF-8\r\n\r\n <!DOCTYPE html> <html> <head> <title>HTTP Server</title> </head> <body> <h1>HTTP Server</h1> <p> This is a HTTP server </p> </body> </html>\r\n\r\n";
    char szBuffer[BUFFER_SIZE];

    printf("Starting server %s:%d ...", ADDRESS, PORT);

    int sockServerFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockServerFd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    struct sockaddr_in saServerAddr = {0};
    saServerAddr.sin_family = AF_INET;
    saServerAddr.sin_port = htons(PORT);
    saServerAddr.sin_addr.s_addr = inet_addr(ADDRESS);

    if (bind(sockServerFd, (struct sockaddr *) &saServerAddr, sizeof(saServerAddr)) < 0) {
        perror("ERROR binding to socket");
        close(sockServerFd);
        exit(1);
    }

    if(listen(sockServerFd, 5) < 0) {
        perror("ERROR listening to socket");
        close(sockServerFd);
        exit(1);
    }

    while(1) {
        int clientFd = accept(sockServerFd, (struct sockaddr *) NULL, NULL);

        if (clientFd < 0) {
            perror("ERROR accepting connection");
            continue;
        }

        printf("Connection accepted");
        if(!fork()) {
            printf("Child process created");
            close(sockServerFd);
            memset(szBuffer, 0, BUFFER_SIZE);

            if(read(clientFd, szBuffer, BUFFER_SIZE) < 0) {
                perror("ERROR reading from socket");
                close(clientFd);
                exit(1);
            }

            printf("%s", szBuffer);

            if(write(clientFd, webpage, sizeof(webpage) - 1) < 0) {
                perror("ERROR writing to socket");
                close(clientFd);
                exit(1);
            }

            close(clientFd);
            printf("Connection closed");
            exit(0);
        }
        close(clientFd);
    }
}
