#include "list.h"
#ifndef _HTTP_H_

#define _HTTP_H_

#if __cplusplus
extern "C" {
#endif

struct http_request {
    char *method;
    char *uri;
    char *version;
    struct List header;
    char *body;
};

struct http_response {
    char *version;
    int status_code;
    char *reason_phrase;
    struct List header;
    char *body;
};

int parse_request(char *request_str, struct http_request *request);

void print_request(struct http_request *request);

int build_resopnse(struct http_request *request, struct http_response *response);

void print_response(struct http_response *res);

int compose_response(struct http_response *res, char *response);

int parse_header(char *header, struct List *header_list_container);

void print_header(struct List *header_list);

#if __cplusplus
}
#endif

#endif