#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

void show_error(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

int create_server_socket(void)
{
    int fd;
    int option = 1;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1)
        show_error("socket");

    /*
     * Allows the port to be reused quickly after
     * restarting the server.
     */
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
                   &option, sizeof(option)) == -1)
    {
        close(fd);
        show_error("setsockopt");
    }

    return fd;
}

int main(void)
{
    int server_socket;
    int connection_socket;

    struct sockaddr_in address;
    char message[BUFFER_SIZE];

    server_socket = create_server_socket();

    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_port = htons(SERVER_PORT);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(server_socket,
             (struct sockaddr *)&address,
             sizeof(address)) == -1)
    {
        close(server_socket);
        show_error("bind");
    }

    if (listen(server_socket, 5) == -1)
    {
        close(server_socket);
        show_error("listen");
    }

    printf("TCP server started on 127.0.0.1:%d\n", SERVER_PORT);
    printf("Waiting for a client...\n");

    connection_socket = accept(server_socket, NULL, NULL);

    if (connection_socket == -1)
    {
        close(server_socket);
        show_error("accept");
    }

    printf("Client connected.\n");
    printf("Start chatting:\n\n");

    while (1)
    {
        fd_set monitored;

        FD_ZERO(&monitored);

        FD_SET(STDIN_FILENO, &monitored);
        FD_SET(connection_socket, &monitored);

        if (select(connection_socket + 1,
                   &monitored,
                   NULL,
                   NULL,
                   NULL) == -1)
        {
            perror("select");
            break;
        }

        /*
         * Check whether the server user typed something.
         */
        if (FD_ISSET(STDIN_FILENO, &monitored))
        {
            if (fgets(message, BUFFER_SIZE, stdin) == NULL)
                break;

            if (send(connection_socket,
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
         * Check whether the client sent something.
         */
        if (FD_ISSET(connection_socket, &monitored))
        {
            int received;

            received = recv(connection_socket,
                            message,
                            BUFFER_SIZE - 1,
                            0);

            if (received <= 0)
            {
                printf("\nClient disconnected.\n");
                break;
            }

            message[received] = '\0';

            printf("\nClient: %s", message);
            printf("You: ");
            fflush(stdout);
        }
    }

    close(connection_socket);
    close(server_socket);

    return 0;
}
