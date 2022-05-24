#include "tachy/vilib.h"

#include "test_assert.h"

bool test1()
{
    static const std::size_t SIZE = 8*2 + 3;

    using AT = std::array<uint64_t, SIZE>;
    AT array1, array2;

    uint64_t inner_product = 0;

    for (std::size_t i = 0; i < SIZE; ++i) {
        array1[i] = i;
        array2[i] = i;
        inner_product += array1[i] * array2[i];
    }

    uint64_t n = vil::arrays_msum<uint64_t>((const AT)array1, (const AT)array2);

    ASSERT(n == inner_product,
        "fail %ld vs %ld", n, inner_product);

    return true;
}

bool test2()
{
    static const std::size_t SIZE = 16*2 + 3;

    using AT = std::array<float, SIZE>;
    AT array1, array2;

    float inner_product = 0;

    for (std::size_t i = 0; i < SIZE; ++i) {
        array1[i] = i;
        array2[i] = i;
        inner_product += array1[i] * array2[i];
    }

    float n = vil::arrays_inner_product<float>((const AT)array1, (const AT)array2);

    ASSERT(n == inner_product,
        "fail %f vs %f", n, inner_product);

    n = vil::array_norm<float>(array1);

    ASSERT(almost_equal(n, (float)sqrt(inner_product), 1),
        "fail %f vs %f", n, sqrt(inner_product));

    return true;
}

int main()
{
    assert(test1());
    assert(test2());

    return 0;
}
