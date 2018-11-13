#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 4321

int sendSTR(int socket, char *msg) {
    return send(socket, msg, strlen(msg) + 1, 0);
}

int NUMQUESTIONS[2] = {5, 9};
char answers[9][50] = {
    "entendido\n",         "#965874580*\n",
    "connecting people\n", "me gusta el chocolate\n",
    "ESGOLODROBO\n",       "amarillo\n",
    "Heisenberg\n",        "indefinido\n",
    "Colombia\n"};

char answersn[5][50] = {"adelante\n", "Lost\n", "BRaNCa\n",
                                  "Eco de los Andes\n", "Macri\n"};

int main(int argc, char const *argv[]) {
    int shouldWait = 0;  // SI CORRES CONTRA EL DE ARIEL PONER EN 1.

    if (argc > 1 && strcmp(argv[1], "ariel") == 0) shouldWait = 1;

    struct sockaddr_in address;
    int sock = 0, valread = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creando el socket.\n");
        exit(EXIT_FAILURE);
    }

    char * answerToQuestion;

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;  // Dominio, IPv4
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nDirección inválida.\n");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("No se puede conectar al servidor.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUMQUESTIONS[shouldWait]; i++) {
        if(shouldWait) {
            sendSTR(sock, answers[i]);
            sleep(1);
        } else
            sendSTR(sock, answersn[i]);
    }

    char *tmpBuff;
    do {
        valread = read(sock, tmpBuff, 1);
        sleep(1);
    } while (valread != -1 && valread != 0);

    printf("El servidor cerró la conexión.\n");

    return 0;
}