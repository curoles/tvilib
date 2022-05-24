#include "tachy/vilib.h"

#include <cstdint>
#include <cassert>
#include <vector>
#include "test_assert.h"

bool test1()
{
    static const std::size_t SIZE = 16*2+4;
    uint32_t array[SIZE];
    for (std::size_t i = 0; i < SIZE; ++i) {
        array[i] = i;
    }

    uint32_t sum = vil::array_sum(array, SIZE);

    ASSERT(sum == (SIZE-1)*(SIZE/2), "fail");

    sum = vil::array_sum(std::span<const uint32_t>{array, SIZE});

    ASSERT(sum == (SIZE-1)*(SIZE/2), "fail");

    return true;
}

bool test2()
{
    static const std::size_t SIZE = 8*2+4;
    double array[SIZE];
    for (std::size_t i = 0; i < SIZE; ++i) {
        array[i] = i;
    }

    double sum = vil::array_sum(array, SIZE);

    ASSERT(sum == (SIZE-1)*(SIZE/2), "fail");

    return true;
}

bool test3()
{
    static const std::size_t SIZE = 16*2+4;
    std::vector<float> array(SIZE);
    for (std::size_t i = 0; i < SIZE; ++i) {
        array[i] = i;
    }

    float sum = vil::array_sum<float>(array);

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
    assert(test3());

    return 0;
}
