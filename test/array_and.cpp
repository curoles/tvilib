#include "tachy/vilib.h"

#include <cassert>
#include "test_assert.h"

static bool test1()
{
    static const std::size_t SIZE = 8*5 + 5;

    std::array<uint64_t, SIZE> array;

    for (std::size_t i = 0; i < SIZE; ++i) {
        array[i] = ~((1ul << (i+1)) - 1);
    }

    uint64_t res = vil::array_and<uint64_t>(array);

    ASSERT(res == ~((1ul << SIZE) - 1),
        "fail %lx vs %lx", res, ~((1ul << SIZE) - 1));

    return true;
}

int main()
{
    assert(test1());

    return 0;
}
