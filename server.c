#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 256

int main(){
    // variable declaration
    int socket_fd, client_fd;
    const int opt = 1;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    client_addr_size  = (socklen_t) sizeof(client_addr);
    // create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        fprintf(stderr, "Socket creation failed. Errno: %s\n", strerror(errno));
        return -1;
    }
    // make socket can reuse socket
    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1){
        fprintf(stderr, "Socket reuse failed. Errno: %s\n", strerror(errno));
        return -1;
    }

    // set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // bind socket to address
    if(bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        fprintf(stderr, "Failed to bind socket to address. Errno: %s\n", strerror(errno));
        return -1;
    }

    // listen to socket
    if(listen(socket_fd, 5) == -1){
        fprintf(stderr, "Failed listen to socket. Errno: %s\n", strerror(errno));
        return -1;
    }
    
    // accept connection
    client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_size);
    if(client_fd == -1){
        fprintf(stderr, "Failed accept connection. Errno: %s\n", strerror(errno));
        return -1;
    }

    // read all data from client
    char *buff;
    char *msg = NULL;
    int pos = 0;
    while (1)
    {
        buff = (char *)malloc(BUFFER_SIZE);
        int recv_size = recv(client_fd, buffer, BUFFER_SIZE, 0);
        msg = realloc(msg, sizeof(msg) * BUFFER_SIZE);
        memmove(msg + pos, buffer, recv_size);
        pos += recv_size;
        free(buff);
        if (recv_size < BUFFER_SIZE || recv_size == -1)
        {
            msg[pos] = '\0';
            break;
        }
    }
    printf("Message from client: \n%s\n", msg);
    free(msg);

    // cleanup
    close(client_fd);
    close(socket_fd);

    return 0;
}