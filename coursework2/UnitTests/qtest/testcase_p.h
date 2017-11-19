#ifndef QTEST_TESTCASE_H
#define QTEST_TESTCASE_H

#include "qtest/testsuite.h"

/*
*  A structure representing a test case
*/
typedef struct qtestcase_s qtestcase_t;

struct qtestcase_s {
    char * label;
    qtestresult_t result;
    qtestcase_t * next;
};

qtestcase_t * create_qtestcase(char * label);

char * qtestcase_label(qtestcase_t * testcase);

qtestresult_t qtestcase_result(qtestcase_t * testcase);

qtestcase_t * qtestcase_assert_true(bool condition, char * label);

void fprint_qtestcase(FILE* stream, qtestcase_t * testcase);

#endif
