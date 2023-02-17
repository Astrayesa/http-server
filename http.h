#include <string.h>
#include <stdio.h>

struct http_request {
    char *method;
    char *uri;
    char *version;
    char *header;
    char *body;
};

struct http_response {
    char *version;
    int status_code;
    char *reason_phrase;
    char *header;
    char *body;
};

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
    *loc = '\0';
    req->header = request;

    // get request body
    request = loc + 4;
    req->body = request;
    return 1;
}

void print_request(struct http_request *req){
    printf("Method: %s\n", req->method);
    printf("URI: %s\n", req->uri);
    printf("Version: %s\n", req->version);
    printf("Header:\n%s\n", req->header);
    printf("Body: %s\n", req->body);
}

int build_resopnse(struct http_response *res, struct http_request *req){
    res->version = req->version;
    res->status_code = 200;
    res->reason_phrase = "OK";
    res->body = calloc(sizeof(char), strlen(req->uri) + 100);
    sprintf( res->body, "You requested to path: %s\n", req->uri);

    int body_length = strlen(res->body);
    res->header = calloc(1, 100);
    if(sprintf(res->header, "Content-Length: %d", body_length) < 0){
        fprintf(stderr, "Error creating header");
        return -1;
    }

    return 1;
}

void print_response(struct http_response *res){
    printf("Version: %s\n", res->version);
    printf("Status-Code: %d\n", res->status_code);
    printf("Reason-Phrase: %s\n", res->reason_phrase);
    printf("Header:\n%s\n", res->header);
    printf("Body: %s\n", res->body);
}

int compose_response(struct http_response *res, char *response){
    sprintf(response, "%s %d %s\r\n%s\r\n\r\n%s", res->version, res->status_code, res->reason_phrase, res->header, res->body);
    return 1;
}