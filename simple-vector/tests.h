#ifndef TESTS_H
#define TESTS_H

#include <cassert>

#include "simple_vector.h"

class X {
public:
    X()
        : X(5) {
    }
    X(size_t num)
        : x_(num) {
    }
    X(const X& other) = delete;
    X& operator=(const X& other) = delete;
    X(X&& other) {
        x_ = std::exchange(other.x_, 0);
    }
    X& operator=(X&& other) {
        x_ = std::exchange(other.x_, 0);
        return *this;
    }
    size_t GetX() const {
        return x_;
    }

private:
    size_t x_;
};

class SimpleVectorTester
{
public:
    SimpleVectorTester();

    void testTemporaryObjConstructor();
    void testTemporaryObjOperator();
    void testNamedMoveConstructor();
    void testNamedMoveOperator();
    void testNoncopiableMoveConstructor();
    void testNoncopiablePushBack();
    void testNoncopiableInsert();
    void testNoncopiableErase();

private:
    SimpleVector<int> generateVector(size_t size);
};

#endif // TESTS_H
