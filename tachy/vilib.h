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
namespace insn {

//TODO extended flavor when sizeof(TDst)=2*sizeof(T)
//
template<typename VT, typename VTDst=VT>
VTDst hsum(VT v)
{
    using T = tvx::get_base<VT>::type;

    if constexpr(std::is_floating_point<T>::value) {
        return tvx::hsumso(v);
    }
    else if constexpr(std::is_signed<T>::value) {
        //FIXME return tvx::hsums(v); //TODO add hsums intrinsic
    }
    else {
        return tvx::hsum(v);
    }
}

}
namespace insn {

template<typename VT>
VT hmax(VT v)
{
    using T = tvx::get_base<VT>::type;

    if constexpr(std::is_floating_point<T>::value) {
        return tvx::hmax(v); //TODO hmaxn?
    }
    else if constexpr(std::is_signed<T>::value) {
        return tvx::hmaxs(v);
    }
    else {
        return tvx::hmax(v);
    }
}

}
/// Sum of all elements.
///
/// It is recommended to use
/// sizeof(TRes) = 2 * sizeof(T) for integral types.
///
template<typename T, typename TRes=T>
TRes array_sum(const T* array, std::size_t nr_elem)
{
    array_info<T> a(nr_elem);
    using VT = array_info<T>::VT;

    TRes sum {0};

    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT v = *(VT*)&array[i*a.VNR_ELEM];
        sum += vil::insn::hsum(v)[0];
    }

    //FIXME TODO once hsum intrinsic has mask, rewrite with hsum+mask
    for (std::size_t i = a.nr_chunks*a.VNR_ELEM; i < a.nr_elem; ++i) {
        sum += array[i];
    }

    return sum;
}

template<typename T, typename TRes=T>
TRes array_sum(std::span<T> array)
{
    return vil::array_sum(array.data(), array.size());
}


/// Reduction AND of all elements.
///
///
template<typename T>
T array_and(const T* array, std::size_t nr_elem)
{
    array_info<T> a(nr_elem);
    using VT = array_info<T>::VT;

    T res {~0ul};

    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT sv = *(VT*)&array[i*a.VNR_ELEM];
        res &= tvx::hand(sv)[0];
    }

    //FIXME TODO once hand intrinsic has mask, rewrite with mask
    for (std::size_t i = a.nr_chunks*a.VNR_ELEM; i < a.nr_elem; ++i) {
        res &= array[i];
    }

    return res;
}

/// Reduction AND of a contiguous sequence of objects.
///
/// span can be C array or STL container.
///
template<typename T>
T array_and(std::span<T> array)
{
    return vil::array_and(array.data(), array.size());
}

/// AND elements of 2 arrays.
///
///
template<typename T>
void arrays_and(T* dst_array, const T* array1, const T* array2, std::size_t nr_elem)
{
    array_info<T> a(nr_elem);
    using VT = array_info<T>::VT;

    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT v1 = *(VT*)&array1[i*a.VNR_ELEM];
        VT v2 = *(VT*)&array2[i*a.VNR_ELEM];
        VT dv = tvx::vand(v1, v2);
        //FIXME tvx::vst((VT*)&dst_array[i*a.VNR_ELEM], /*P0*/tvx::_pmask(63/*VSZ1*/), dv);
        __builtin_memcpy(&dst_array[i*a.VNR_ELEM], &dv, sizeof(dv));
    }

    //FIXME TODO once hand intrinsic has mask, rewrite with mask
    for (std::size_t i = a.nr_chunks*a.VNR_ELEM; i < a.nr_elem; ++i) {
        dst_array[i] = array1[i] & array2[i];
    }
}

template<typename T>
void arrays_and(std::span<T> dst_array, std::span<T> array1, std::span<T> array2)
{
    assert(array2.size() >= array1.size());
    assert(dst_array.size() >= array1.size());

    vil::arrays_and(dst_array.data(), array1.data(), array2.data(), array1.size());
}

/// Find element with maximum value.
///
///
template<typename T, typename TRes=T>
TRes array_find_max(const T* array, std::size_t nr_elem)
{
    array_info<T> a(nr_elem);
    using VT = array_info<T>::VT;

    TRes maxel {array[0]};

    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT v = *(VT*)&array[i*a.VNR_ELEM];
        maxel = std::max(maxel, vil::insn::hmax(v)[0]);
    }

    //FIXME TODO once hsum intrinsic has mask, rewrite with hsum+mask
    for (std::size_t i = a.nr_chunks*a.VNR_ELEM; i < a.nr_elem; ++i) {
        maxel = std::max(maxel, array[i]);
    }

    return maxel;
}

template<typename T, typename TRes=T>
TRes array_find_max(std::span<T> array)
{
    return vil::array_find_max(array.data(), array.size());
}

/// add elements of 2 arrays.
///
///
template<typename T>
void arrays_add(T* dst_array, const T* array1, const T* array2, std::size_t nr_elem)
{
    array_info<T> a(nr_elem);
    using VT = array_info<T>::VT;

    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT v1 = *(VT*)&array1[i*a.VNR_ELEM];
        VT v2 = *(VT*)&array2[i*a.VNR_ELEM];
        VT dv = tvx::vadd(v1, v2);
        //FIXME tvx::vst((VT*)&dst_array[i*a.VNR_ELEM], /*P0*/tvx::_pmask(63/*VSZ1*/), dv);
        __builtin_memcpy(&dst_array[i*a.VNR_ELEM], &dv, sizeof(dv));
    }

    //FIXME TODO once hand intrinsic has mask, rewrite with mask
    for (std::size_t i = a.nr_chunks*a.VNR_ELEM; i < a.nr_elem; ++i) {
        dst_array[i] = array1[i] + array2[i];
    }
}

template<typename T>
void arrays_add(std::span<T> dst_array, std::span<T> array1, std::span<T> array2)
{
    assert(array2.size() >= array1.size());
    assert(dst_array.size() >= array1.size());

    vil::arrays_add(dst_array.data(), array1.data(), array2.data(), array1.size());
}
/// mul elements of 2 arrays.
///
///
template<typename T>
void arrays_mul(T* dst_array, const T* array1, const T* array2, std::size_t nr_elem)
{
    array_info<T> a(nr_elem);
    using VT = array_info<T>::VT;

    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT v1 = *(VT*)&array1[i*a.VNR_ELEM];
        VT v2 = *(VT*)&array2[i*a.VNR_ELEM];
        VT dv = tvx::vmul(v1, v2);
        //FIXME tvx::vst((VT*)&dst_array[i*a.VNR_ELEM], /*P0*/tvx::_pmask(63/*VSZ1*/), dv);
        __builtin_memcpy(&dst_array[i*a.VNR_ELEM], &dv, sizeof(dv));
    }

    //FIXME TODO once hand intrinsic has mask, rewrite with mask
    for (std::size_t i = a.nr_chunks*a.VNR_ELEM; i < a.nr_elem; ++i) {
        dst_array[i] = array1[i] * array2[i];
    }
}

template<typename T>
void arrays_mul(std::span<T> dst_array, std::span<T> array1, std::span<T> array2)
{
    assert(array2.size() >= array1.size());
    assert(dst_array.size() >= array1.size());

    vil::arrays_mul(dst_array.data(), array1.data(), array2.data(), array1.size());
}
/// add_abs elements of 2 arrays.
///
///
template<typename T>
void arrays_add_abs(T* dst_array, const T* array1, const T* array2, std::size_t nr_elem)
{
    array_info<T> a(nr_elem);
    using VT = array_info<T>::VT;

    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT v1 = *(VT*)&array1[i*a.VNR_ELEM];
        VT v2 = *(VT*)&array2[i*a.VNR_ELEM];
        VT dv = tvx::vadda(v1, v2);
        //FIXME tvx::vst((VT*)&dst_array[i*a.VNR_ELEM], /*P0*/tvx::_pmask(63/*VSZ1*/), dv);
        __builtin_memcpy(&dst_array[i*a.VNR_ELEM], &dv, sizeof(dv));
    }

    //FIXME TODO once hand intrinsic has mask, rewrite with mask
    for (std::size_t i = a.nr_chunks*a.VNR_ELEM; i < a.nr_elem; ++i) {
        dst_array[i] = std::abs(array1[i]) + std::abs(array2[i]);
    }
}

template<typename T>
void arrays_add_abs(std::span<T> dst_array, std::span<T> array1, std::span<T> array2)
{
    assert(array2.size() >= array1.size());
    assert(dst_array.size() >= array1.size());

    vil::arrays_add_abs(dst_array.data(), array1.data(), array2.data(), array1.size());
}

} // end of namespace vil
