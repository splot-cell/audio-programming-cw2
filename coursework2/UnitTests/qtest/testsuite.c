#include <stdio.h>
#include <stdlib.h>

#include "qtest/testsuite.h"
#include "testsuite_p.h"
#include "unittest_p.h"

qtestsuite_t * create_qtestsuite(char * label) {
    qtestsuite_t * testsuite = malloc(sizeof(qtestsuite_t));

    if (testsuite == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for qtestsuite_t!\n");
        exit(-1);
    }

    testsuite->label = label;
    testsuite->length = 0;
    testsuite->first = NULL;
    return testsuite;
}

char * qtestsuite_label(qtestsuite_t * testsuite) {
    return testsuite->label;
}

qunittest_t * add_qunittest(char * label, qtestsuite_t * testsuite) {
    qunittest_t * unittest = create_qunittest(label);

    if (testsuite->length == 0) {
        testsuite->first = unittest;
        testsuite->length = 1;
        return unittest;
    }

    qunittest_t * last_unittest = testsuite->first;

    for (int i = 1; i < testsuite->length; i++)
        last_unittest = last_unittest->next;

    last_unittest->next = unittest;
    testsuite->length++;

    return unittest;
}

void fprint_qtestsuite(FILE* stream, qtestsuite_t * testsuite) {
    fprintf(stream, "- %s\n    ------\n", qtestsuite_label(testsuite));

    if (testsuite->length == 0) {
        fprintf(stream, "Error: No unit tests in test suite!\n");
        return;
    }

    qunittest_t * unittest = testsuite->first;

    for (int i = 0; i < testsuite->length; i++) {
        fprint_qunittest(stream, unittest);
        fprintf(stream, "\n");
        unittest = unittest->next;
    }

    int unittests = qtestsuite_unittest_count(testsuite);
    int unittest_failures = qtestsuite_unittest_failures(testsuite);
    int testcases = qtestsuite_testcase_count(testsuite);
    int testcase_failures = qtestsuite_testcase_failures(testsuite);

    fprintf(stream, "- Results: %s\n", testsuite->label);

    if (unittest_failures == 0)
        fprintf(stream, 
            "  All unit tests passed!\n"
            "  Test cases: %d\n"
            "  Unit tests: %d\n",
            testcases, unittests);
    else
        fprintf(stream, 
            "  Test cases failed: %d of %d\n"
            "  Unit tests failed: %d of %d\n",
            testcase_failures, testcases,
            unittest_failures, unittests);
}

int qtestsuite_unittest_count(qtestsuite_t * testsuite) {
    return testsuite->length;
}

int qtestsuite_testcase_count(qtestsuite_t * testsuite) {
    int count = 0;

    qunittest_t * unittest = testsuite->first;

    for (int i = 0; i < testsuite->length; i++) {
        count += unittest->length;
        unittest = unittest->next;
    }

    return count;
}

int qtestsuite_unittest_failures(qtestsuite_t * testsuite) {
    int count = 0;

    qunittest_t * unittest = testsuite->first;

    for (int i = 0; i < testsuite->length; i++) {
        if (qunittest_testcase_failures(unittest) > 0)
            count++;
        unittest = unittest->next;
    }

    return count;
}

int qtestsuite_testcase_failures(qtestsuite_t * testsuite) {
    int count = 0;

    qunittest_t * unittest = testsuite->first;

    for (int i = 0; i < testsuite->length; i++) {
        count += qunittest_testcase_failures(unittest);
        unittest = unittest->next;
    }

    return count;
}
