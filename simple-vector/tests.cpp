#include <iostream>
#include <numeric>

#include "tests.h"

using namespace std::literals;

SimpleVectorTester::SimpleVectorTester(){}

SimpleVector<int> SimpleVectorTester::generateVector(size_t size) {
    SimpleVector<int> v(size);
    std::iota(v.begin(), v.end(), 1);
    return v;
}

void SimpleVectorTester::testTemporaryObjConstructor() {
    const size_t size = 1000000;
    std::cout << "Test with temporary object, copy elision"s << std::endl;
    SimpleVector<int> moved_vector(generateVector(size));
    assert(moved_vector.size() == size);
    std::cout << "Done!"s << std::endl << std::endl;
}

void SimpleVectorTester::testTemporaryObjOperator() {
    const size_t size = 1000000;
    std::cout << "Test with temporary object, operator="s << std::endl;
    SimpleVector<int> moved_vector;
    assert(moved_vector.size() == 0);
    moved_vector = generateVector(size);
    assert(moved_vector.size() == size);
    std::cout << "Done!"s << std::endl << std::endl;
}

void SimpleVectorTester::testNamedMoveConstructor() {
    const size_t size = 1000000;
    std::cout << "Test with named object, move constructor"s << std::endl;
    SimpleVector<int> vector_to_move(generateVector(size));
    assert(vector_to_move.size() == size);

    SimpleVector<int> moved_vector(std::move(vector_to_move));
    assert(moved_vector.size() == size);
    assert(vector_to_move.size() == 0);
    std::cout << "Done!"s << std::endl << std::endl;
}

void SimpleVectorTester::testNamedMoveOperator() {
    const size_t size = 1000000;
    std::cout << "Test with named object, operator="s << std::endl;
    SimpleVector<int> vector_to_move(generateVector(size));
    assert(vector_to_move.size() == size);

    SimpleVector<int> moved_vector = std::move(vector_to_move);
    assert(moved_vector.size() == size);
    assert(vector_to_move.size() == 0);
    std::cout << "Done!"s << std::endl << std::endl;
}

void SimpleVectorTester::testNoncopiableMoveConstructor() {
    const size_t size = 5;
    std::cout << "Test noncopiable object, move constructor"s << std::endl;
    SimpleVector<X> vector_to_move;
    for (size_t i = 0; i < size; ++i) {
        vector_to_move.pushBack(X(i));
    }

    SimpleVector<X> moved_vector = std::move(vector_to_move);
    assert(moved_vector.size() == size);
    assert(vector_to_move.size() == 0);

    for (size_t i = 0; i < size; ++i) {
        assert(moved_vector[i].GetX() == i);
    }
    std::cout << "Done!"s << std::endl << std::endl;
}

void SimpleVectorTester::testNoncopiablePushBack() {
    const size_t size = 5;
    std::cout << "Test noncopiable push back"s << std::endl;
    SimpleVector<X> v;
    for (size_t i = 0; i < size; ++i) {
        v.pushBack(X(i));
    }

    assert(v.size() == size);

    for (size_t i = 0; i < size; ++i) {
        assert(v[i].GetX() == i);
    }
    std::cout << "Done!"s << std::endl << std::endl;
}

void SimpleVectorTester::testNoncopiableInsert() {
    const size_t size = 5;
    std::cout << "Test noncopiable insert"s << std::endl;
    SimpleVector<X> v;
    for (size_t i = 0; i < size; ++i) {
        v.pushBack(X(i));
    }

    // в начало
    v.insert(v.begin(), X(size + 1));
    assert(v.size() == size + 1);
    assert(v.begin()->GetX() == size + 1);
    // в конец
    v.insert(v.end(), X(size + 2));
    assert(v.size() == size + 2);
    assert((v.end() - 1)->GetX() == size + 2);
    // в середину
    v.insert(v.begin() + 3, X(size + 3));
    assert(v.size() == size + 3);
    assert((v.begin() + 3)->GetX() == size + 3);
    std::cout << "Done!"s << std::endl << std::endl;
}

void SimpleVectorTester::testNoncopiableErase() {
    const size_t size = 3;
    std::cout << "Test noncopiable erase"s << std::endl;
    SimpleVector<X> v;
    for (size_t i = 0; i < size; ++i) {
        v.pushBack(X(i));
    }

    auto it = v.erase(v.begin());
    assert(it->GetX() == 1);
    std::cout << "Done!"s << std::endl << std::endl;
}
