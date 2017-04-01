#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include <minix/pubsub_status.h>
#include "framework.h"
#include "framework_api.h"

#define GET_TOPIC_NAME(VAR) \
    char VAR[MAX_TOPIC_NAME_SIZE]; \
    printf("Enter topic name: "); \
    scanf(" %[^\n]%*c", VAR);

#define GET_MESSAGE(VAR) \
    char VAR[MESSAGE_SIZE]; \
    printf("Enter message: "); \
    scanf(" %[^\n]%*c", VAR);

#define PRINT_STATUS(status) \
    printf("Status: %s\n", error_msgs[status]);

char *error_msgs[] = {
        "SUCCESS",
        "error",
        "Already Publisher",
        "Already Subscriber",
        "Not a publisher",
        "No subscriber",
        "No matching topic found",
        "Max topic per process reached",
        "err_msg_full",
        "No new message available",
        "Max topic reached",
        "Topic exists",
        "Error empty message",
        "Max characters in topic name",
        "No empty topic name",
        "No handle found for the given topic.\n"
                "First creating topic (if doesn't exist) & then become publisher and/or subscriber of the topic"
                " and handle will be created automatically.",
        "Deadlock",
};

typedef int (*testcase_t)();

static int test_create_topic();
static int test_add_publisher();
static int test_add_subscriber();
static int test_publish_message();
static int test_retrieve_message();
static int test_list_topics();


testcase_t testcases[] = {
        &test_create_topic,
        &test_add_publisher,
        &test_add_subscriber,
        &test_publish_message,
        &test_retrieve_message,
        &test_list_topics,
};

char *menu[] = {
        "Create topic",
        "Add Publisher",
        "Add Subscriber",
        "Publish Message",
        "Retrieve Message",
        "List topics",
        "Exit",
};


int MENU_SIZE = sizeof(menu) / sizeof(char *);

int
main (int argc, char **argv)
{
    int user_response = -1;
    int menu_item = 0;
    while (1) {
        printf("\nInteractive tests \n");
        for (menu_item = 0; menu_item < MENU_SIZE; ++menu_item) {
            printf("%d. %s\n", menu_item + 1, menu[menu_item]);
        }
        printf("Enter your choice: ");
        scanf("%d", &user_response);
        if (user_response < 1 || user_response > MENU_SIZE) {
            printf("Incorrect input. Valid inputs are numbers from range (%d - %d) inclusive\n", 1, MENU_SIZE);
        } else if (user_response == MENU_SIZE) {
            printf("Exiting...\n");
            printf("Recommended: Reboot os to clean state\n");
            exit(0);
        } else {
            (testcases[user_response - 1])();
        }
    }
    return 0;
}

static int
test_create_topic()
{
    status_t status = ok;
    GET_TOPIC_NAME(topic_name);
    status = create_topic(topic_name);
    PRINT_STATUS(status);
    return status;
}


static int
test_add_publisher()
{
    status_t status = ok;
    GET_TOPIC_NAME(topic_name);
    status = add_publisher(topic_name);
    PRINT_STATUS(status);
    return status;
}

static int
test_add_subscriber()
{
    status_t status = ok;
    GET_TOPIC_NAME(topic_name);
    status = add_subscriber(topic_name);
    PRINT_STATUS(status);
    return status;
}

static int
test_publish_message()
{
    status_t status = ok;
    GET_TOPIC_NAME(topic_name);
    GET_MESSAGE(message);
    status = publish_message(topic_name, message);
    PRINT_STATUS(status);
    return status;
}

static int
test_retrieve_message()
{
    status_t status = ok;
    char *message = NULL;
    GET_TOPIC_NAME(topic_name);
    status = retrieve_message(topic_name, &message);
    PRINT_STATUS(status);
    if (status == ok) {
        printf("Message: %s\n", message);
        free(message);
    }
    return status;
}

static int
test_list_topics()
{
    status_t status = ok;
    char *topics = NULL;
    status = list_topics(&topics);
    PRINT_STATUS(status);
    if (status == ok) {
        printf("Topics: %s\n", topics);
        free(topics);
    }
    return status;
}

