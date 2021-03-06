#include "tachy/vilib.h"

#include "test_assert.h"

static bool test1()
{
    static const std::size_t SIZE = 8*2 + 3;

    std::array<uint64_t, SIZE> array1, array2, array3;

    for (std::size_t i = 0; i < SIZE; ++i) {
        array1[i] = i;
        array2[i] = i;
    }

    vil::arrays_mul<uint64_t>(array3, array1, array2);

    for (std::size_t i = 0; i < SIZE; ++i) {
        ASSERT(array3[i] == (i * i),
            "fail pos:%lu %lx vs %lx", i, array3[i], (i*i));
    }

    return true;
}

static bool test2()
{
    static const std::size_t SIZE = 8*2 + 5;

    std::array<double, SIZE> array1, array2, array3;

    for (std::size_t i = 0; i < SIZE; ++i) {
        array1[i] = i;
        array2[i] = i;
    }

    vil::arrays_mul<double>(array3, array1, array2);

    for (std::size_t i = 0; i < SIZE; ++i) {
        ASSERT(array3[i] == (i * i),
            "fail pos:%lu %f vs %lx", i, array3[i], (i*i));
    }

    return true;
}

int main()
{
    assert(test1());
    assert(test2());

    return 0;
}
