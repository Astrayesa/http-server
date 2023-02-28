#include "list.h"
#include <gtest/gtest.h>

TEST(ListTest, CreateNode)
{
    struct Node* new_node = create_node((char*)"Host", (char*)"localhost:8080");
    EXPECT_STREQ(new_node->key, "Host");
    EXPECT_STREQ(new_node->value, "localhost:8080");
    EXPECT_EQ(new_node->next, nullptr);
    free(new_node);
}

TEST(ListTest, Append)
{
    struct List *list_ptr = (struct List*) calloc(1, sizeof(struct List));
    struct Node* host_node = create_node((char*)"Host", (char*)"localhost:8080");
    append(list_ptr, host_node);
    EXPECT_EQ(list_ptr->head, host_node);
    EXPECT_EQ(list_ptr->tail, host_node);

    struct Node* ua_node = create_node((char*)"User-Agent", (char*)"testcase");
    append(list_ptr, ua_node);
    EXPECT_EQ(list_ptr->head, host_node);
    EXPECT_EQ(list_ptr->tail, ua_node);
    clear_list(list_ptr);
    free(list_ptr);
}

TEST(ListTest, GetNode)
{
    struct List *list_ptr = (struct List*) calloc(1, sizeof(struct List));
    struct Node* host_node = create_node((char*)"Host", (char*)"localhost:8080");
    append(list_ptr, host_node);
    struct Node* ua_node = create_node((char*)"User-Agent", (char*)"testcase");
    append(list_ptr, ua_node);

    struct Node* get_host_node = get_node(list_ptr, (char*)"Host");
    EXPECT_EQ(get_host_node, host_node);
    EXPECT_STREQ(get_host_node->key, "Host");
    EXPECT_STREQ(get_host_node->value, "localhost:8080");

    struct Node* get_ua_node = get_node(list_ptr, (char*)"User-Agent");
    EXPECT_EQ(get_ua_node, ua_node);
    EXPECT_STREQ(get_ua_node->key, "User-Agent");
    EXPECT_STREQ(get_ua_node->value, "testcase");
    clear_list(list_ptr);
    free(list_ptr);
}