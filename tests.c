#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include <minix/pubsub_status.h>
#include "framework.h"
#include "framework_api.h"

char *testName;
typedef int (*testcase_t)();
int block(char *topic_name, char *message);

static int test_create_topic();
static int test_block_publisher();
static int test_create_duplicate_topic();
static int test_add_publisher_twice();
static int test_add_subscriber_twice();
static int test_add_publisher_subscriber_unknown_topic();
static int test_retrieve_unknown_topic();
static int test_retrieve_different_topic();

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
        "No handle found",
};

testcase_t testcases[] = {
        &test_create_topic,
        &test_block_publisher,
        &test_create_duplicate_topic,
//        &test_add_publisher_twice,
//        &test_add_subscriber_twice,
//        &test_add_publisher_subscriber_unknown_topic,
//        &test_retrieve_unknown_topic,
//        &test_retrieve_different_topic,
};

int
main()
{
    uint64_t ntestcases = 0;
    uint64_t totaltestcases = sizeof(testcases) / sizeof(testcase_t);

    printf("Note: You need to run these tests \
            before running other program which uses this topic system calls");
    for (ntestcases = 0; ntestcases < totaltestcases; ntestcases++) {
        (testcases[ntestcases])();
        printf("Executing tests: %s\n", testName);
    }
    return 0;
}

static int
test_create_topic()
{
    char *topic_name = "test";
    status_t status = ok;
    testName = "Create new topic";
    status = create_topic(topic_name);
    if (status == ok) {
        printf("topic %s created successfully", topic_name);
    } else {
        printf("WARNING: You need to reboot to run tests", topic_name);
    }
    TEST_DONE();
    return status;
}

static int
test_block_publisher()
{
    uint64_t max_msgs = 10;
    uint64_t nmsg = 0;
    int child_status = 0;
    status_t status = ok;
    char *topic_name = "block";
    char *msg = "msg";
    char *rmsg = NULL;
    testName = "Publisher blocks";

    create_topic(topic_name);
    add_subscriber(topic_name);
    add_publisher(topic_name);

    if (fork() == 0) {
        add_subscriber(topic_name);
        for (nmsg = 0; nmsg < max_msgs; nmsg++) {
            sleep(7);
            status = retrieve_message(topic_name, &rmsg);
            if (status == ok) {
                printf("Message Retrieved: %d\n", nmsg);
                free(rmsg);
            }
        }
        exit(0);
    } // child process

    for (nmsg = 0; nmsg < max_msgs; nmsg++) {
        printf("Publishing msg: %d\n", nmsg);
        block(topic_name, msg);
    }
    wait(&child_status);
}

int block(char *topic_name, char *message) {
    status_t status = ok;
    do {
        sleep(1);
        status = publish_message(topic_name, message);
    } while(status == err_msg_full);
}

static int
test_create_duplicate_topic()
{
    char *topic_name = "duptest";
    status_t status = ok;
    testName = "Create topic with existing name";
    status = create_topic(topic_name);
    if (status == err_topic_exist) {
        printf("WARNING: You need to reboot to run tests", topic_name);
    }
    status = create_topic(topic_name);
    ASSERT_EQUALS_INT(status, err_topic_exist, error_msgs[err_topic_exist]);
    TEST_DONE();
    return status;
}

/*
static int
test_add_publisher_twice()
{

}

static int
test_add_subscriber_twice()
{

}

static int
test_add_publisher_subscriber_unknown_topic()
{

}

static int
test_retrieve_unknown_topic()
{

}

static int
test_retrieve_different_topic()
{

}
*/