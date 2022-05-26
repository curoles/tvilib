#include "tachy/vilib.h"

#include <cstdint>
#include <cassert>
#include "test_assert.h"

static bool test1()
{
    static const std::size_t SIZE = 8*2+4;
    double array[SIZE];
    double check{0};
    for (std::size_t i = 0; i < SIZE; i += 2) {
        array[i] = i;
        array[i+1] = i + 1;
        check += array[i] * array[i+1];
    }

    double sum = vil::array_mul_pair_sum(array, SIZE);

    ASSERT(sum == check, "fail");

    return true;
}

int main()
{
    assert(test1());

    return 0;
}
