#pragma once

#include <type_traits>
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

}

namespace vil {

constexpr std::size_t VSZ1 = 512 / 8;

//constexpr __mask_t P0 = (__mask_t)((uint8_t)0x0 - (/*__mask_t*/tvx::U8x16){});
//constexpr __mask_t PMASK0 = tvx::_pmask(VSZ1); (__mask_t)((uint8_t)0xff - (tvx::U8x16){});

template<typename T>
struct array_info {
    array_info(std::size_t nr_elem):nr_elem(nr_elem){}

    const std::size_t nr_elem;

    static constexpr std::size_t VNR_ELEM = VSZ1 / sizeof(T);

    const std::size_t nr_bytes = nr_elem * sizeof(T);

    const std::size_t nr_chunks = nr_bytes / VSZ1;

    using VT = tvx::make<T,VNR_ELEM>::type;
};
