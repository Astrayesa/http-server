#ifndef _LIST_H_

#define _LIST_H_

#if __cplusplus
extern "C"
{
#endif

struct List
{
    struct Node *head, *tail;
};

struct Node
{
    char *key;
    char *value;
    struct Node *next;
};

void init_list(struct List *list);
struct Node* create_node(char* key, char* value);
void append(struct List *list, struct Node *node);
struct Node* get_node(struct List *header_list, char *key);
int clear_list(struct List *list);

#if __cplusplus
}
#endif

#endif