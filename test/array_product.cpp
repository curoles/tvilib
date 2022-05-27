#include "tachy/vilib.h"

#include <cassert>
#include "test_assert.h"

static bool test1()
{
    static const std::size_t SIZE = 8*3 + 3;

    std::array<double, SIZE> array;
    double prod{1};

    for (std::size_t i = 0; i < SIZE; ++i) {
        array[i] = i + 1;
        prod *= array[i];
    }

    double res = vil::array_product<double>(array);

    ASSERT(res == prod,
        "fail %f vs %f", res, prod);

    return true;
}

int main()
{
    assert(test1());

    return 0;
}
