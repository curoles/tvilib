#include "tachy/vilib.h"

#include <cstdint>
#include <cassert>
#include <vector>
#include "test_assert.h"

bool test1()
{
    static const std::size_t SIZE = 16*2+3;
    uint32_t array[SIZE];
    for (std::size_t i = 0; i < SIZE; ++i) {
        array[i] = i;
    }

    uint32_t n = vil::array_find_min(array, SIZE);

    ASSERT(n == 0, "fail");

    return true;
}

bool test2()
{
    static const std::size_t SIZE = 8*2+4;
    double array[SIZE];
    for (std::size_t i = 0; i < SIZE; ++i) {
        array[i] = -SIZE + i;
    }

    double n = vil::array_find_min(array, SIZE);

    ASSERT(n == -SIZE, "fail");

    return true;
}

bool test3()
{
    static const std::size_t SIZE = 16*2+5;
    std::vector<int32_t> array(SIZE);
    for (std::size_t i = 0; i < SIZE; ++i) {
        array[i] = -SIZE + i;
    }

    int32_t n = vil::array_find_min<int32_t>(array);

    ASSERT(n == (int32_t)(-SIZE), "fail");

    return true;
}



int main()
{
    assert(test1());
    assert(test2());
    assert(test3());

    return 0;
}

