#include <cstdint>
#include <cassert>
#include "tachy/vilib.h"
#include "test_assert.h"

bool test1()
{
    static const unsigned int SIZE = 1560;
    uint32_t array[SIZE];
    for (unsigned int i = 0; i < SIZE; ++i) {
        array[i] = i;
    }

    uint32_t sum = vil::array_sum(array, SIZE);

    ASSERT(sum == (SIZE-1)*(SIZE/2), "fail");

    return true;
}

bool test2()
{
    static const unsigned int SIZE = 1560;
    double array[SIZE];
    for (unsigned int i = 0; i < SIZE; ++i) {
        array[i] = i;
    }

    double sum = vil::array_sum(array, SIZE);

    ASSERT(sum == (SIZE-1)*(SIZE/2), "fail");

    return true;
}

//TODO test for uneven size
//TODO test for signed type
//TODO test for extended flavor of hsum

int main()
{
    assert(test1());
    assert(test2());

    return 0;
}
