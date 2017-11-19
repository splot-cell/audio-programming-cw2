#ifndef QTEST_TESTSUITE_P_H
#define QTEST_TESTSUITE_P_H

#include "qtest/testsuite.h"

struct qtestsuite_s {
    char * label;
    int length;
    qunittest_t * first;
};

char * qtestsuite_label(qtestsuite_t * testsuite);

#endif
