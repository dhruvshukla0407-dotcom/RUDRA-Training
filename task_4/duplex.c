#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

void report_error(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

int main(void)
{
    int client_socket;
    struct sockaddr_in server;
    char message[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket == -1)
        report_error("socket");

    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET,
                  "127.0.0.1",
                  &server.sin_addr) != 1)
    {
        close(client_socket);
        report_error("inet_pton");
    }

    if (connect(client_socket,
                (struct sockaddr *)&server,
                sizeof(server)) == -1)
    {
        close(client_socket);
        report_error("connect");
    }

    printf("Connected to TCP server.\n");
    printf("Start chatting:\n\n");

    while (1)
    {
        fd_set monitored;

        FD_ZERO(&monitored);

        FD_SET(STDIN_FILENO, &monitored);
        FD_SET(client_socket, &monitored);

        if (select(client_socket + 1,
                   &monitored,
                   NULL,
                   NULL,
                   NULL) == -1)
        {
            perror("select");
            break;
        }

        /*
         * Keyboard input is ready.
         */
        if (FD_ISSET(STDIN_FILENO, &monitored))
        {
            if (fgets(message, BUFFER_SIZE, stdin) == NULL)
                break;

            if (send(client_socket,
                     message,
                     strlen(message),
                     0) == -1)
            {
                perror("send");
                break;
            }

            if (strncmp(message, "exit", 4) == 0)
                break;
        }

        /*
         * Data has arrived from the server.
         */
        if (FD_ISSET(client_socket, &monitored))
        {
            int received;

            received = recv(client_socket,
                            message,
                            BUFFER_SIZE - 1,
                            0);

            if (received <= 0)
            {
                printf("\nServer disconnected.\n");
                break;
            }

            message[received] = '\0';

            printf("\nServer: %s", message);
            printf("You: ");
            fflush(stdout);
        }
    }

    close(client_socket);

    return 0;
}
