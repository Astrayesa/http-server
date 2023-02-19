#ifndef _HTTP_H_

#define _HTTP_H_

#if __cplusplus
extern "C" {
#endif

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

int parse_request(char *request, struct http_request *req);

void print_request(struct http_request *req);

int build_resopnse(struct http_response *res, struct http_request *req);

void print_response(struct http_response *res);

int compose_response(struct http_response *res, char *response);


#if __cplusplus
}
#endif

#endif