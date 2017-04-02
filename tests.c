#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>

#include <minix/pubsub_status.h>
#include "framework.h"
#include "framework_api.h"

char *testName;
typedef int (*testcase_t)();

static int run_tests(int testcase);
static int test_create_topic();
static int test_block_publisher();
static int test_create_duplicate_topic();
static int test_add_publisher_twice();
static int test_add_subscriber_twice();

int block(char *topic_name, char *message);

char *testcase_names[] = {
        "Create topic",
        "Publisher blocks",
        "Create topic with existing name",
        "Add publisher twice to same topic",
        "Add subscriber twice to same topic",
};

testcase_t testcases[] = {
        &test_create_topic,
        &test_block_publisher,
        &test_create_duplicate_topic,
        &test_add_publisher_twice,
        &test_add_subscriber_twice,
};

int
main()
{
    int ntestcases = 0;
    int totaltestcases = sizeof(testcases) / sizeof(testcase_t);

    printf("Note: You need to run these tests" \
            " before running other program which uses this topic system calls.\n");
    printf("Running tests ...\n");
    for (ntestcases = 0; ntestcases < totaltestcases; ntestcases++) {
        run_tests(ntestcases);
    }
    return 0;
}

static int run_tests(int ntestcase) {
    testName = testcase_names[ntestcase];
    printf("\nExecuting tests: %s\n", testcase_names[ntestcase]);
    (testcases[ntestcase])();
    printf("Executed tests: %s\n", testcase_names[ntestcase]);
    return 0;
}

static int
test_create_topic()
{
    char *topic_name = "test";
    status_t status = ok;
    status = create_topic(topic_name);
    ASSERT_EQUALS_INT(status, ok, "Create topic test");
    return status;
}

static int
test_block_publisher()
{
    int max_msgs = 10;
    int nmsg = 0;
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
    return child_status;
}

int block(char *topic_name, char *message) {
    status_t status = ok;
    do {
        sleep(1);
        status = publish_message(topic_name, message);
    } while(status == err_msg_full);
    return status;
}

static int
test_create_duplicate_topic()
{
    char *topic_name = "duptest";
    status_t status = ok;
    status = create_topic(topic_name);
    ASSERT_EQUALS_INT(status, ok, "Create topic duptest");
    status = create_topic(topic_name);
    ASSERT_EQUALS_INT(status, ok, "Create topic again duptest");
    return status;
}


static int
test_add_publisher_twice()
{
    char *topic_name = "ptwice";
    status_t status = ok;
    status = create_topic(topic_name);
    ASSERT_EQUALS_INT(status, ok, "Create topic ptwice");
    status = add_publisher(topic_name);
    ASSERT_EQUALS_INT(status, ok, "Add publisher");
    status = add_publisher(topic_name);
    ASSERT_EQUALS_INT(status, err_already_publisher, "Add publisher again");
    return status;

}


static int
test_add_subscriber_twice()
{
    char *topic_name = "stwice";
    status_t status = ok;
    status = create_topic(topic_name);
    ASSERT_EQUALS_INT(status, ok, "Create topic stwice");
    status = add_subscriber(topic_name);
    ASSERT_EQUALS_INT(status, ok, "Add subscriber");
    status = add_subscriber(topic_name);
    ASSERT_EQUALS_INT(status, err_already_publisher, "Add subscriber again");
    return status;
}