#ifndef QTEST_UNITTEST_H
#define QTEST_UNITTEST_H

#include "qtest/testsuite.h"
#include "testcase_p.h"

struct qunittest_s {
    char * label;
    int length;
    qtestcase_t * first;
    qunittest_t * next;
};

qunittest_t * create_qunittest(char * label);

char * qunittest_label(qunittest_t * unittest);

void add_qtestcase(qtestcase_t * testcase, qunittest_t * unittest);

void fprint_qunittest(FILE* stream, qunittest_t * unittest);

int qunittest_testcase_failures(qunittest_t * unittest);

#endif
