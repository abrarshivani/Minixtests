#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include <minix/pubsub_status.h>
#include "framework.h"
#include "framework_api.h"

char *testName;
typedef void (*testcase_t)();

int testCreateTopic();

testcase_t testcases[] = {
        &testCreateTopic,
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
testCreateTopic()
{
    char *topic_name = "test";
    status_t status = ok;
    testName = "Create new topic";
    status = create_topic(topic_name);
    ASSERT_EQUALS_INT(status, ok, "Successfully created topic");
    TEST_DONE();
    return status;
}

