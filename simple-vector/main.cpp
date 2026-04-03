#include "tests.h"


int main() {

    SimpleVectorTester simpleVectorTester;
    simpleVectorTester.testTemporaryObjConstructor();
    simpleVectorTester.testTemporaryObjOperator();
    simpleVectorTester.testNamedMoveConstructor();
    simpleVectorTester.testNamedMoveOperator();
    simpleVectorTester.testNoncopiableMoveConstructor();
    simpleVectorTester.testNoncopiablePushBack();
    simpleVectorTester.testNoncopiableInsert();
    simpleVectorTester.testNoncopiableErase();

    return 0;
}
