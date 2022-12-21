//
// Created by 1012
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
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

    printf("Starting server %s:%d ...\n", ADDRESS, PORT);

    char webpage[] = "HTTP/1.1 200 OK\r\n Content-Type: text/html; charset=UTF-8\r\n\r\n <!DOCTYPE html> <html> <head> <title>HTTP Server</title> </head> <body> <h1>HTTP Server</h1> <p> File not found </p> </body> </html>\r\n\r\n";
    char szBuffer[BUFFER_SIZE];
    char szFileName[128];
    char szResponseHeader[256];

    // Lager en server-socket
    int sockServerFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockServerFd < 0) {
        printf("ERROR opening socket\n");
        exit(1);
    }

    // Allokerer minne til en sockaddr_in struktur, og setter verdier.
    struct sockaddr_in saServerAddr = {0};
    saServerAddr.sin_family = AF_INET;
    saServerAddr.sin_port = htons(PORT);
    saServerAddr.sin_addr.s_addr = inet_addr(ADDRESS);

    // Binder socketen til adressen
    if (bind(sockServerFd, (struct sockaddr *) &saServerAddr, sizeof(saServerAddr)) < 0) {
        printf("ERROR binding to socket\n");
        close(sockServerFd);
        exit(1);
    }

    // Lytter på socketen
    if(listen(sockServerFd, 5) < 0) {
        printf("ERROR listening to socket\n");
        close(sockServerFd);
        exit(1);
    }

    // Så lenge serveren kjører og lytter på socketen, så skal den akseptere nye klient-connections
    while(1) {
        // Aksepterer en klient-connection
        int clientFd = accept(sockServerFd, (struct sockaddr *) NULL, NULL);

        if (clientFd < 0) {
            printf("ERROR accepting connection\n");
            continue;
        }

        printf("Connection accepted\n");

        // Åpner en klient-fork. Dette er en ny prosess som håndterer klienten.
        if(!fork()) {
            printf("Child process created\n");
            close(sockServerFd);
            memset(szBuffer, 0, BUFFER_SIZE);

            // Leser fra klienten
            if(read(clientFd, szBuffer, BUFFER_SIZE) < 0) {
                printf("ERROR reading from socket\n");
                close(clientFd);
                exit(1);
            }

            // Henter ut filnavnet fra bufferen
            sscanf(szBuffer, "GET /%s HTTP/1.1", szFileName);

            printf("%s", szBuffer);
            printf("Requested file: %s\n", szFileName);

            // Hvis serveren ikke finner fila, så returnerer vi "webpage", som er en html-side. Hvis serveren finner fila, så returnerer vi filen.
            if(access(szFileName, F_OK) < 0){
                printf("File not found\n");
                if(write(clientFd, webpage, sizeof(webpage) - 1) < 0) {
                    printf("ERROR writing to socket\n");
                    close(clientFd);
                    exit(1);
                }
            } else {
                printf("File found\n");

                // Åpner filen og genererer filinformasjon.
                int fd = open(szFileName, O_RDONLY);
                struct stat stFileInfo;
                fstat(fd, &stFileInfo);

                // Genererer headeren som skal sendes til klienten
                sprintf(szResponseHeader, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\n", stFileInfo.st_size);

                // Hvis serveren ikke klarer å skrive til headeren, lukker vi socketen og avslutter.
                if(write(clientFd, szResponseHeader, strlen(szResponseHeader)) < 0) {
                    printf("ERROR sending header to client\n");
                    close(clientFd);
                    exit(1);
                }

                // Hvis serveren klarer å sende headeren, så sender vi filen til klienten.
                printf("Sending file to client, Content-Length: %lu\n", stFileInfo.st_size);
                sendfile(clientFd, fd, NULL, stFileInfo.st_size);
            }

            // Lukker socketen og avslutter.
            close(clientFd);
            printf("Client -  Connection closed\n");
            exit(0);
        }
        // Lukker klient-socketen og fortsetter å lytte på server-socketen.
        close(clientFd);
        printf("Client -  Connection closed\n");
    }
}
