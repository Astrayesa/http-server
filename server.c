#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "http.h"

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]){
    
    // variable declaration
    int opt_cli;
    int TEST_MODE = 0;

    int socket_fd, client_fd;
    const int sock_opt = 1;
    char buffer[BUFFER_SIZE];

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size = (socklen_t) sizeof(client_addr);

    struct http_request *req;
    struct http_response *res;

    char *request_msg;
    char *response_msg;

    int pos;
    // handle argument
    while ((opt_cli = getopt(argc, argv, "t")) != -1)
    {
        switch (opt_cli)
        {
        case 't':
            TEST_MODE = 1;
            break;
        
        default:
            printf("Usage: %s [-t]\n", argv[0]);
            return -1;
            break;
        }
    }

    // create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        fprintf(stderr, "Socket creation failed. Errno: %s\n", strerror(errno));
        return -1;
    }

    // make socket can reuse socket
    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &sock_opt, sizeof(int)) == -1){
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

    do {
        req = (struct http_request *)calloc(1, sizeof(struct http_request));
        res = (struct http_response *)calloc(1, sizeof(struct http_response));

        // accept connection
        client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_size);
        if (client_fd == -1)
        {
            fprintf(stderr, "Failed accept connection. Errno: %s\n", strerror(errno));
            return -1;
        }

        // read all data from client
        request_msg = NULL;
        pos = 0;

        while (1)
        {
            int recv_size = recv(client_fd, buffer, BUFFER_SIZE, 0);
            if (recv_size == -1)
            {
                fprintf(stderr, "Error recving data. Errno: %s\n", strerror(errno));
                request_msg = "";
                break;
            }
            
            request_msg = (char *)realloc(request_msg, pos + BUFFER_SIZE);
            memmove(request_msg + pos, buffer, recv_size);
            pos += recv_size;
            if (recv_size < BUFFER_SIZE || recv_size == -1)
            {
                request_msg[pos] = '\0';
                break;
            }
        }

        // parse request
        if (parse_request(request_msg, req) == -1)
        {
            fprintf(stderr, "Failed to parse request. Request message: %s\n", request_msg);
            // harusnya return 400
            continue;
        }

        if (build_resopnse(req, res) == -1)
        {
            fprintf(stderr, "Failed to create response.");
            return -1;
        }
        // print_response(&res);

        // send response
        response_msg = (char *)calloc(BUFFER_SIZE, sizeof(char)); //don't know how much memory we need
        compose_response(res, response_msg);
        send(client_fd, response_msg, strlen(response_msg), 0);

        // free memory
        struct Node *node = get_node(&res->header, "Content-Length");
        if (node != NULL)
        {
            free(get_node(&res->header, "Content-Length")->value);
        }

        // free memory
        // free reuqest 
        clear_list(&req->header);
        free(req);
        free(request_msg);

        // free response
        free(res->body);
        clear_list(&res->header);
        free(res);
        free(response_msg);

        // cleanup
        close(client_fd);
    } while (TEST_MODE != 1);

    close(socket_fd);

    return 0;
}