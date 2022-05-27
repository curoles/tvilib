#include "tachy/vilib.h"

#include <cassert>
#include "test_assert.h"

static bool test1()
{
    static const std::size_t SIZE = 8*3 + 3;

    std::array<uint64_t, SIZE> array;

    for (std::size_t i = 0; i < SIZE; ++i) {
        array[i] = i;
    }

    std::size_t index = vil::array_find_first<uint64_t>(23, array).value_or(0);

    ASSERT(index == 23ul,
        "fail %lu vs %lu", index, 23ul);

    return true;
}

int main()
{
    assert(test1());

    return 0;
}
