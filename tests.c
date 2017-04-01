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
/*
static int test_create_duplicate_topic();
static int test_add_publisher_twice();
static int test_add_subscriber_twice();
static int test_add_publisher_subscriber_unknown_topic();
static int test_retrieve_unknown_topic();
static int test_retrieve_different_topic();
*/

testcase_t testcases[] = {
        &test_create_topic,
        &test_block_publisher,
};

int
main()
{
    uint64_t ntestcases = 0;
    uint64_t totaltestcases = sizeof(testcases) / sizeof(testcase_t);

    for (ntestcases = 0; ntestcases < totaltestcases; ntestcases++) {
        (testcases[ntestcases])();
        printf("Executing tests: %s\n", testName);
    }
    return 0;
}

int
test_create_topic()
{
    char *topic_name = "test";
    status_t status = ok;
    testName = "Create new topic";
    status = create_topic(topic_name);
    ASSERT_EQUALS_INT(status, ok, "Successfully created topic");
    TEST_DONE();
    return status;
}

int
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
            sleep(2);
            status = retrieve_msg(topic_name, &rmsg);
            if (status == ok) {
                printf("Message Retrieved: %s\n", nmsg);
                free(rmsg);
            }
        }
        exit(0);
    } // child process

    for (nmsg = 0; nmsg < max_msgs; nmsg++) {
        printf("Publishing msg: %s\n", nmsg);
        block(topic_name, msg);
    }
    wait(&child_status);
}

int block(char *topic_name, char *message) {
    status_t status = ok;
    do {
        sleep(1);
        status = publish_message(topic_name, message);
    } while(status != err_msg_full);
}
