#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 4321

#define clear() printf("\033[H\033[J")

#define NUMQUESTIONS 5

char answers[NUMQUESTIONS][50] = {"adelante\n", "Lost\n", "BRaNCa\n",
                                  "Eco de los Andes\n", "Macri\n"};
char questions[NUMQUESTIONS][2048] = {
    "Jelou jelou!\nÚltimo trabajito. A ver si lo hacemos como la "
    "gente.\nEscriba \"adelante\" para continuar.",
    "4 8 15 16 23 42", "G2P13G7P2G2P15G4P2", "7792799000011",
    "       ,     ,       \n"
    "       |\\.\"./|       \n"
    "      / _   _ \\       \n"
    "     / {|} {|} \\  _   \n"
    "     \\==  Y  ==/ ( \\  \n"
    "      ;-._^_.-;   ) ) \n"
    "     /   \\_/   \\ / /  \n"
    "     |   (_)   |/ /   \n"
    "    /|  |   |  |\\/    \n"
    "   | |  |   |  | |    \n"
    "    \\|  |___|  |/     \n"
    "     '\"\"'   '\"\"'      "};

char suggestion[NUMQUESTIONS][1024] = {
    "¿Que es peor? ¿Branca con Pepsi o 1882 con Coca?",
    "¿Cuantos kilobytes se necesitan para almacenar la edad de Mirtha Legrand?",
    "¿Cuántos TPs no testeados hacen falta para enloquecer a Rowda?",
    "¿Hay algo más decadente que la puerta del Laboratorio 9?",
    "¿Con cuanto entusiasmo le puso un 10 al grupo 1?"};

void drawLevel(int level) {
    clear();
    printf("------------- NIVEL %d -------------\n", level);
    printf("%s\n\n", questions[level]);
    printf("PREGUNTA PARA INVESTIGAR:\n%s\n\n", suggestion[level]);
}

int createSocket() {
    int socket_fd;
    struct sockaddr_in address;
    int opt = 1;

    // Crear FD del socket
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("No se pudo crear socket");
        exit(EXIT_FAILURE);
    }

    // Vincular socket al puerto
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("No se puede establecer el puerto");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror(
            "No se puede vincular el socket a la dirección y puerto "
            "especificados");
        exit(EXIT_FAILURE);
    }
    if (listen(socket_fd, 3) < 0) {
        perror("No se puede poner el socket a la escucha");
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

void sendSTR(int socket, char *msg) { send(socket, msg, strlen(msg) + 1, 0); }

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = createSocket();

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
        perror("Error aceptando la conexión entrante");
        exit(EXIT_FAILURE);
    }

    int currQuestion = 0;
    int drawn = 0;

    char answer[50];
    int i = 0;
    int redraw = 1;

    while (currQuestion < NUMQUESTIONS) {
        if (redraw) drawLevel(currQuestion);
        redraw = 0;

        // valread = read(new_socket, buffer, 1024);
        valread = read(new_socket, &answer[i], 1);
        if (valread == 0) {
            printf("El cliente se desconectó. Saliendo...\n");
            close(server_fd);
            exit(1);
        }

        if (answer[i] == '\n') {
            redraw = 1;
            answer[i + 1] = 0;
            if (strcmp(answer, answers[currQuestion]) == 0) {
                currQuestion++;
            } else {
                printf("Respuesta incorrecta.\n");
                sleep(1);
            }
            i = 0;
        } else if (answer[i] == 0)
            i = 0;
        else
            i++;
    }

    clear();

    printf(
        "( •_•)\n( •_•)>⌐■-■\n(⌐■_■)\n\n"
        "Yeah.\n");

    close(new_socket);
    close(server_fd);

    return 0;
}