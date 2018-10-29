// Client side C/C++ program to demonstrate Socket programming
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 4321

void sendSTR(int socket, char *msg) { send(socket, msg, strlen(msg) + 1, 0); }

int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;  // Dominio, IPv4
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    /*char *msg = "entendido\n";
    send(sock, msg, strlen(msg) + 1, 0);*/

    //#up up left down left up right down down left *

    sendSTR(sock, "entendido\n");
    sendSTR(sock, "#965874580*\n");
    sendSTR(sock, "connecting people\n");
    sendSTR(sock, "me gusta el chocolate\n");
    sendSTR(sock, "ESGOLODROBO\n");
    sendSTR(sock, "amarillo\n");
    sendSTR(sock, "Heisenberg\n");
    sendSTR(sock, "indefinido\n");
    sendSTR(sock, "Colombia\n");

    sleep(5);

    return 0;
}