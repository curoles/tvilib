#include "tachy/vilib.h"

#include "test_assert.h"

bool test1()
{
    static const std::size_t SIZE = 8*2 + 3;

    std::array<uint64_t, SIZE> array1, array2, array3;

    for (std::size_t i = 0; i < SIZE; ++i) {
        array1[i] = ~((1ul << (i+1)) - 1);
        array2[i] = array1[i];
    }

    vil::arrays_and<uint64_t>(array3, array1, array2);

    for (std::size_t i = 0; i < SIZE; ++i) {
        ASSERT(array3[i] == ~((1ul << (i+1)) - 1),
            "fail pos:%lu %lx vs %lx", i, array3[i], ~((1ul << (i+1)) - 1));
    }

    return true;
}

int main()
{
    assert(test1());

    return 0;
}
