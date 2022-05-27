#pragma once

#include <type_traits>
#include <optional>
#include <span>
#include <cmath>
#include <cassert>
#include <cstdio>
#include <vxintrin.hpp>

//FIXME
namespace tvx{
template <> struct make<uint64_t, 2> {typedef tvx::U64x2  type;};
template <> struct make<uint64_t, 4> {typedef tvx::U64x4  type;};
template <> struct make<uint64_t, 8> {typedef tvx::U64x8  type;};
template <> struct make< int64_t, 2> {typedef tvx::S64x2  type;};
template <> struct make< int64_t, 4> {typedef tvx::S64x4  type;};
template <> struct make< int64_t, 8> {typedef tvx::S64x8  type;};

static inline void
__attribute__ ((__gnu_inline__, __always_inline__, __artificial__))
vst(F64x8* a, __mask_t b, F64x8 c) {
    __builtin_tachy_vst8_512(a, b, (tvx::U64x8)c);
}

static inline F64x8
__attribute__ ((__gnu_inline__, __always_inline__, __artificial__))
vld(__mask_t a, F64x8* b) {
    return (F64x8) __builtin_tachy_vld8_512(a, b);
}

using pmask_t = __mask_t;
}

namespace vil {


template<typename T, std::size_t VLEN = 512>
struct array_info
{
    static constexpr std::size_t VSZ = VLEN / 8;

    static constexpr std::size_t NR_ELEM = VSZ / sizeof(T);

    using VT = tvx::make<T,NR_ELEM>::type;

    array_info(std::size_t nr_elem):nr_elem(nr_elem){}

    const std::size_t nr_elem;

    const std::size_t nr_bytes = nr_elem * sizeof(T);

    const std::size_t nr_chunks = nr_bytes / VSZ;

    const std::size_t nr_tail_bytes = nr_bytes - (nr_chunks * VSZ);

    const std::size_t nr_tail_elem = nr_tail_bytes / sizeof(T);

};
