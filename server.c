#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 4321

#define clear() printf("\033[H\033[J")

#define NUMQUESTIONS 9

char answers[NUMQUESTIONS][50] = {
    "entendido\n",         "#965874580*\n",
    "connecting people\n", "me gusta el chocolate\n",
    "ESGOLODROBO\n",       "amarillo\n",
    "Heisenberg\n",        "indefinido\n",
    "Colombia\n"};
char questions[NUMQUESTIONS][2048] = {
    "Bienvenidos al TP4 y felicitaciones, ya resolvieron el primer "
    "acertijo.\nEn este TP deberán finalizar el juego que ya comenzaron "
    "respondiendo correctamente los desafios de cada nivel. Además tendrán que "
    "investigar otras preguntas para responder durante la defensa.\nEl desafio "
    "final consiste en crear un servidor que se comporte igual que yo.\n\nPara "
    "verificar que sus respuestas tienen el formato correcto respondan a este "
    "desafio con \"entendido\\n\"",
    "# up up left down left up right down down left *",
    "-.-. --- -. -. . -.-. - .. -. --. / .--. . --- .--. .-.. .",
    "Mike Echo Golf Uniform Sierra Tango Alpha Echo Lima Charlie Hotel Oscar "
    "Charlie Oscar Lima Alpha Tango Echo",
    "TJ ANBYDNBCJ J NBCN JLNACQRX NB \"NBOXTXMAXKX\"",
    "#FFFF00",
    "I AM THE ONE WHO KNOCKS",
    "sizeof(int)",
    "dEAtP3SAlnA"};

char suggestion[NUMQUESTIONS][1024] = {
    "Cómo descubrieron el protocolo, la direccion y el puerto para conectarse?",
    "Que diferencias hay entre TCP y UDP y en que casos conviene usar cada "
    "uno?",
    "El puerto que usaron para conectarse al server es el mismo que usan para "
    "mandar las respuestas? por qué?",
    "Qué útil abstracción es utilizada para comunicarse con sockets? se puede "
    "utilizar read(2) y write(2) para operar?",
    "Cómo garantiza TCP que los paquetes llegan en orden y no se pierden?",
    "Un servidor suele crear un nuevo proceso o thread para atender las "
    "conexiones entrantes. Qué conviene más?",
    "Cómo se puede implementar un servidor que atienda muchas conexiones sin "
    "usar procesos ni threads?",
    "Qué aplicaciones se pueden utilizar para ver el tráfico por la red. "
    "Muestre un ejemplo.",
    "sockets es un mecanismo de IPC. Qué es más eficiente entre sockets y "
    "pipes?"};

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
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(socket_fd, 3) < 0) {
        perror("listen");
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
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    server_fd = createSocket();

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
        perror("accept");
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
        "Felicitaciones, finalizaron el juego. Ahora deberán implementar el "
        "servidor que se comporte como el servidor provisto.\n");

    close(server_fd);

    return 0;
}