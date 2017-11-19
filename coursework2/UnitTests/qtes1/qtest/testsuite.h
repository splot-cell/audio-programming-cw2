#ifndef QTEST_TESTSUITE_H
#define QTEST_TESTSUITE_H

#include <stdbool.h>

/*
    semantic version (semver.org): 1.0.1-snapshot

    qtest is a small test framework for C. It provides a test suite for
    holding a collection of unit tests that in turn hold a collection of
    test cases.
*/

/*
    Return results for test cases, unit tests and test suites
*/
typedef enum {
    FAILED,
    PASSED
} qtestresult_t;

/*
    A structure representing a collection of test cases
*/
typedef struct qunittest_s qunittest_t;

/*
    A structure representing a collection of unit tests
*/
typedef struct qtestsuite_s qtestsuite_t;

/*
    Create a test suite with a description/label
*/
qtestsuite_t * create_qtestsuite(char * label);

/*
    Create a unit test and add it to a test suite
*/
qunittest_t * add_qunittest(char * label, qtestsuite_t * testsuite);

/*
    Add a test case to a unit test and set its result based on a boolean condition
*/
qtestresult_t qtest_assert_true(bool condition, char * label, qunittest_t * unittest);

/*
    Print test suite results
*/
void fprint_qtestsuite(FILE* stream, qtestsuite_t * testsuite);

/*
    Get the number of test cases in a test suite
*/
int qtestsuite_unittest_count(qtestsuite_t * testsuite);

/*
    Get the number of failed unit tests in a test suite
*/
int qtestsuite_unittest_failures(qtestsuite_t * testsuite);

/*
    Get the number of test cases in a test suite
*/
int qtestsuite_testcase_count(qtestsuite_t * testsuite);

/*
    Get the number of failed test cases in a test suite
*/
int qtestsuite_testcase_failures(qtestsuite_t * testsuite);

#endif
