#include "VectorTest.h"

#include <core/collections/Vector.h>
#include <assert.h>
#include <float.h>

void hd::test::vectors::TestVector() {
    const unsigned VECTOR_START_SIZE = 4;
    hd::Vector<int> v1(VECTOR_START_SIZE);

    hdAssert(v1.IsEmpty());
    hdAssert(v1.Pop() == v1.End());
    hdAssert(v1.RemoveAtIndex(30) == false);

    v1.Add(10);
    v1.Add(20);
    v1.Add(30);
    v1.Add(40);

    hdAssert(v1.Size() == 4);
    hdAssert(v1.IsFull());

    int pop = v1.Pop();
    hdAssert(pop == 40);
    hdAssert(v1.Size() == 3);

    int addResult = v1.Add(50);
    hdAssert(addResult == 3);
    hdAssert(v1.At(addResult) == 50);
    hdAssert(v1.Size() == 4);

    v1.Clear();
    hdAssert(v1.Size() == 0);
    hdAssert(v1.IsFull() == false);

    v1.Add(10);
    v1.Add(20);
    v1.Add(30);
    v1.Add(40);
    v1.Add(50);

    hdAssert(v1.Size() == 5);
    hdAssert(v1.IsFull() == false);

    v1.Clear();
    hdAssert(v1.Size() == 0);
    hdAssert(v1.IsFull() == false);

    v1.Add(10);
    v1.Add(20);
    v1.Add(30);
    hdAssert(v1.Size() == 3);

    hd::Vector<int> v2(VECTOR_START_SIZE);
    v2.Add(10);
    v2.Add(20);

    v2.Enqueue(40);
    hdAssert(v2.At(0) == 40);
    hdAssert(v2.Size() == 3);

    v2.Enqueue(40);
    hdAssert(v2.At(0) == 40);
    hdAssert(v2.At(1) == 40);
    hdAssert(v2.At(2) == 10);
    hdAssert(v2.At(3) == 20);

    pop = v2.Pop();
    hdAssert(pop == 20);
    hdAssert(v2.Size() == 3);

    hd::Vector<int> v3(0);
    v3.Enqueue(10);
    hdAssert(v3.Size() == 1);
    v3.Enqueue(20);
    hdAssert(v3.Size() == 2);
    v3.Enqueue(30);
    hdAssert(v3.Size() == 3);
    v3.Enqueue(40);
    hdAssert(v3.Size() == 4);
    v3.Enqueue(50);
    hdAssert(v3.Size() == 5);

    hdAssert(v3.At(0) == 50);
    hdAssert(v3.At(1) == 40);
    hdAssert(v3.At(2) == 30);
    hdAssert(v3.At(3) == 20);
    hdAssert(v3.At(4) == 10);
}
