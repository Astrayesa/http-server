#include "http.h"
#include "list.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_request(char *request, struct http_request *req){
    // request line parsing
    char* loc = strstr(request, "\r\n");
    if (loc == NULL)
    {
        return -1;
    }
    *loc = '\0';

    req->method = strtok(request, " ");
    if (req->method == NULL)
    {
        return -1;
    }

    req->uri = strtok(NULL, " ");
    if (req->uri == NULL)
    {
        return -1;
    }

    req->version = strtok(NULL, " ");
    if (req->version == NULL)
    {
        return -1;
    }
    request = loc + 2;
    // end request line parsing

    // parse header
    loc = strstr(request, "\r\n\r\n");
    if (loc == NULL)
    {
        return -1;
    }
    loc += 2;
    *loc = '\0';
    parse_header(request, &req->header);

    // get request body
    request = loc + 2;
    req->body = request;
    return 0;
}

void print_request(struct http_request *req){
    printf("Method: %s\n", req->method);
    printf("URI: %s\n", req->uri);
    printf("Version: %s\n", req->version);

    printf("Header:\n");
    print_header(&req->header);
    printf("\n");

    printf("Body: %s\n", req->body);
}

void not_supported_response(struct http_request *req, struct http_response *res){
    res->version = "HTTP/1.1";
    res->status_code = 501;
    res->reason_phrase = "Not Implemented";
    res->body = "Not Implemented";
    init_list(&res->header);
    append(&res->header, create_node("Host", get_node(&req->header, "Host")->value));
}

int build_resopnse(struct http_request *req, struct http_response *res){
    // only accept GET method
    if (strcmp(req->method, "GET") != 0)
    {
        not_supported_response(req, res);
        return 0;
    }
    res->version = req->version;
    res->status_code = 200;
    res->reason_phrase = "OK";
    res->body = (char*) calloc(sizeof(char), strlen(req->uri) + 100);
    sprintf(res->body, "You requested to path: %s\n", req->uri);

    int body_length = strlen(res->body);
    char* str_body_length = (char*) calloc((int) ceil(log10(body_length) + 1), sizeof(char));
    sprintf(str_body_length, "%d", body_length);

    init_list(&res->header);
    append(&res->header, create_node("Host", get_node(&req->header, "Host")->value));
    append(&res->header, create_node("Content-Type", "text/plain"));
    append(&res->header, create_node("Content-Length", str_body_length));
    return 0;
}

void print_response(struct http_response *res){
    printf("Version: %s\n", res->version);
    printf("Status-Code: %d\n", res->status_code);
    printf("Reason-Phrase: %s\n", res->reason_phrase);
    printf("Header:\n");
    print_header(&res->header);
    printf("\n");
    printf("Body: %s\n", res->body);
}

int compose_response(struct http_response *res, char *response){
    sprintf(response, "%s %d %s\r\n", res->version, res->status_code, res->reason_phrase);
    struct Node *current = res->header.head;
    while (current != NULL)
    {
        sprintf(response, "%s%s: %s\r\n", response, current->key, current->value);
        current = current->next;
    }
    // sprintf(response, "%s\r\n%s", response, res->body);
    strcat(response, "\r\n");
    strcat(response, res->body);
    return 1;
}

void print_header(struct List *header_list){
    struct Node *current = header_list->head;
    while(current != NULL){
        printf("%s: %s", current->key, current->value);
        current = current->next;
    }
}

int parse_header(char *header, struct List *header_list_container){
    init_list(header_list_container);
    char *loc = header;
    while(*loc != '\0'){
        char *key = loc;
        loc = strstr(loc, ":");
        if (loc == NULL)
        {
            return -1;
        }
        *loc = '\0';
        loc += 2;

        char *value = loc;
        loc = strstr(loc, "\r\n");
        if (loc == NULL)
        {
            return -1;
        }
        *loc = '\0';
        loc += 2;

        append(header_list_container, create_node(key, value));
    }
    return 0;
}

char* get_header(struct List *header_list, char *key){
    struct Node *node = get_node(header_list, key);
    if (node == NULL)
    {
        return NULL;
    }
    return node->value;
}
