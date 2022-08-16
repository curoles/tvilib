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

static inline pmask_t
__attribute__ ((__gnu_inline__, __always_inline__, __artificial__))
_pmask(int a)
{
    return (__mask_t) __builtin_tachy_pmask(a);
}

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
VT hmsum(VT v1, VT v2)
{
    using T = tvx::get_base<VT>::type;

    if constexpr(std::is_floating_point<T>::value) {
        return tvx::hmsumso(v1, v2);
    }
    else if constexpr(std::is_signed<T>::value) {
        return tvx::hmsum(v1, v2);
    }
    else {
        return tvx::hmsum(v1, v2);
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
namespace insn {

template<typename VT>
VT hmin(VT v)
{
    using T = tvx::get_base<VT>::type;

    if constexpr(std::is_floating_point<T>::value) {
        return tvx::hmin(v); //TODO hminn?
    }
    else if constexpr(std::is_signed<T>::value) {
        return tvx::hmins(v);
    }
    else {
        return tvx::hmin(v);
    }
}

}
namespace insn {

template<typename VT>
VT hmpsum(VT v)
{
    using T = tvx::get_base<VT>::type;

    if constexpr(std::is_floating_point<T>::value) {
        return tvx::hmpsumso(v);
    }
    else {
        //return tvx::hmpsum(v);
    }
}

}
namespace insn {

template<typename VT>
VT hprod(VT v)
{
    using T = tvx::get_base<VT>::type;

    if constexpr(std::is_floating_point<T>::value) {
        return tvx::hprodso(v);
    }
    //else if constexpr(std::is_signed<T>::value) {
    //    return tvx::xxx(v);
    //}
    else {
        return tvx::hprod(v);
    }
}

}
namespace insn {

template<typename VT, std::size_t VLEN>
VT vfill(typename tvx::get_base<VT>::type v)
{
    using T = tvx::get_base<VT>::type;
    //FIXME static_assert(T and Tv are the same type)

    if constexpr(VLEN == 512) {
        if constexpr(sizeof(T) == sizeof(uint64_t))
            return tvx::__vx512_fill_l(v);
        else if constexpr(sizeof(T) == sizeof(uint32_t))
            return tvx::__vx512_fill_w(v);
        else if constexpr(sizeof(T) == sizeof(uint16_t))
            return tvx::__vx512_fill_h(v);
        else if constexpr(sizeof(T) == sizeof(uint8_t))
            return tvx::__vx512_fill_b(v);
    }
    else if constexpr(VLEN == 256) {
        if constexpr(sizeof(T) == sizeof(uint64_t))
            return tvx::__vx256_fill_l(v);
        else if constexpr(sizeof(T) == sizeof(uint32_t))
            return tvx::__vx256_fill_w(v);
        else if constexpr(sizeof(T) == sizeof(uint16_t))
            return tvx::__vx256_fill_h(v);
        else if constexpr(sizeof(T) == sizeof(uint8_t))
            return tvx::__vx256_fill_b(v);
    }

}

}
namespace insn {

template<typename VT>
VT vmadd(VT a, VT b, VT c)
{
    using T = tvx::get_base<VT>::type;

    if constexpr(std::is_floating_point<T>::value) {
        return tvx::vmadd(a, b, c);
    }
    else if constexpr(std::is_signed<T>::value) {
        return tvx::vmadds(a, b, c);
    }
    else {
        return tvx::vmadd(a, b, c);
    }
}

}
namespace insn {

//
// “r” is a GPR “x” is a FPR/VPR and “Yp” is a predicate reg
//
// what if I need to specify size of “r” or “x”? like b,h,w,l
// size will be automagically determined based on the size of the input/output object
//
static inline
tvx::pmask_t mov_r2p(uint64_t src)
{
    tvx::pmask_t dst;
    asm("pmov %[d], %[s]"
            : [d] "=Yp" (dst)
            : [s] "r" (src)
            :);
    return dst;
}

static inline
uint64_t mov_p2r(tvx::pmask_t src)
{
    uint64_t dst;
    asm("pmov %[d], %[s]"
            : [d] "=r" (dst)
            : [s] "Yp" (src)
            :);
    return dst;
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
        VT v = *(VT*)&array[i*a.NR_ELEM];
        sum += vil::insn::hsum(v)[0];
    }

    //FIXME TODO once hsum intrinsic has mask, rewrite with hsum+mask
    for (std::size_t i = a.nr_chunks*a.NR_ELEM; i < a.nr_elem; ++i) {
        sum += array[i];
    }

    return sum;
}

template<typename T, typename TRes=T>
TRes array_sum(std::span<const T> array)
{
    return vil::array_sum(array.data(), array.size());
}

/// Sum of a[i]*b[i] elements.
///
///
template<typename T, typename TRes=T>
TRes arrays_msum(const T* array1, const T* array2, std::size_t nr_elem)
{
    array_info<T> a(nr_elem);
    using VT = array_info<T>::VT;

    TRes sum {0};

    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT v1 = *(VT*)&array1[i*a.NR_ELEM];
        VT v2 = *(VT*)&array2[i*a.NR_ELEM];
        sum += vil::insn::hmsum(v1, v2)[0];
    }

    //FIXME TODO once hsum intrinsic has mask, rewrite with hsum+mask
    for (std::size_t i = a.nr_chunks*a.NR_ELEM; i < a.nr_elem; ++i) {
        sum += array1[i] * array2[i];
    }

    return sum;
}

template<typename T, typename TRes=T>
TRes arrays_msum(const std::span<const T> array1, const std::span<const T> array2)
{
    assert(array2.size() >= array1.size());

    return vil::arrays_msum(array1.data(), array2.data(), array1.size());
}

template<typename T, typename TRes=T>
TRes arrays_inner_product(std::span<const T> array1, std::span<const T> array2)
{
    return vil::arrays_msum<T,TRes>(array1, array2);
}

/// Vector L2 norm.
///
///  The most commonly encountered vector norm
///  (often simply called "the norm" of a vector, or sometimes the magnitude of a vector)
///  is the L2-norm, given by |x| = √ x₁² + x₂² + x₃² + ...
///
template<typename T, typename TRes=T>
TRes array_norm(std::span<const T> array)
{
    return sqrt(vil::arrays_inner_product<T,TRes>(array, array));
}
// AUTOGENERATED

/// Reduction and of all elements.
///
///
template<typename T, std::size_t VLEN = 512>
T array_and(const T* array, std::size_t nr_elem)
{
    using AI = array_info<T,VLEN>;
    AI a(nr_elem);
    using VT = AI::VT;

    T res { ~0ul  };

#if 0
    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT sv = *(VT*)&array[i*a.NR_ELEM];
        res &= tvx::hand(sv)[0];
    }
#else
    for (std::size_t i = 0; (i+1) < a.nr_chunks; i += 2) {
        VT sv1 = *(VT*)&array[i*a.NR_ELEM];
        VT sv2 = *(VT*)&array[(i+1)*a.NR_ELEM];
        res &= tvx::hand(sv1)[0] & tvx::hand(sv2)[0];
    }
    if (a.nr_chunks % 2) {
        VT sv = *(VT*)&array[(a.nr_chunks-1)*a.NR_ELEM];
        res &= tvx::hand(sv)[0];
    }
#endif

    //FIXME TODO once intrinsic has mask, rewrite with mask
    for (std::size_t i = a.nr_chunks*a.NR_ELEM; i < a.nr_elem; ++i) {
        res &= array[i];
    }
    /*if (a.nr_tail_elem) {
        tvx::pmask_t mask = tvx::_pmask(a.nr_tail_elem * sizeof(T));
        VT sv = tvx::vld(mask, (VT*)&array[a.nr_chunks*a.NR_ELEM]);
        res &= tvx::hand(sv)[0]; //FIXME use mask
    }*/

    return res;
}

/// Reduction and of a contiguous sequence of objects.
///
/// span can be C array or STL container.
///
template<typename T, std::size_t VLEN = 512>
T array_and(std::span<const T> array)
{
    return vil::array_and<T,VLEN>(array.data(), array.size());
}
// AUTOGENERATED

/// Reduction or of all elements.
///
///
template<typename T, std::size_t VLEN = 512>
T array_or(const T* array, std::size_t nr_elem)
{
    using AI = array_info<T,VLEN>;
    AI a(nr_elem);
    using VT = AI::VT;

    T res { 0  };

#if 0
    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT sv = *(VT*)&array[i*a.NR_ELEM];
        res |= tvx::hor(sv)[0];
    }
#else
    for (std::size_t i = 0; (i+1) < a.nr_chunks; i += 2) {
        VT sv1 = *(VT*)&array[i*a.NR_ELEM];
        VT sv2 = *(VT*)&array[(i+1)*a.NR_ELEM];
        res |= tvx::hor(sv1)[0] | tvx::hor(sv2)[0];
    }
    if (a.nr_chunks % 2) {
        VT sv = *(VT*)&array[(a.nr_chunks-1)*a.NR_ELEM];
        res |= tvx::hor(sv)[0];
    }
#endif

    //FIXME TODO once intrinsic has mask, rewrite with mask
    for (std::size_t i = a.nr_chunks*a.NR_ELEM; i < a.nr_elem; ++i) {
        res |= array[i];
    }
    /*if (a.nr_tail_elem) {
        tvx::pmask_t mask = tvx::_pmask(a.nr_tail_elem * sizeof(T));
        VT sv = tvx::vld(mask, (VT*)&array[a.nr_chunks*a.NR_ELEM]);
        res &= tvx::hand(sv)[0]; //FIXME use mask
    }*/

    return res;
}

/// Reduction or of a contiguous sequence of objects.
///
/// span can be C array or STL container.
///
template<typename T, std::size_t VLEN = 512>
T array_or(std::span<const T> array)
{
    return vil::array_or<T,VLEN>(array.data(), array.size());
}
// AUTOGENERATED

/// Reduction xor of all elements.
///
///
template<typename T, std::size_t VLEN = 512>
T array_xor(const T* array, std::size_t nr_elem)
{
    using AI = array_info<T,VLEN>;
    AI a(nr_elem);
    using VT = AI::VT;

    T res { 0  };

#if 0
    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT sv = *(VT*)&array[i*a.NR_ELEM];
        res ^= tvx::hxor(sv)[0];
    }
#else
    for (std::size_t i = 0; (i+1) < a.nr_chunks; i += 2) {
        VT sv1 = *(VT*)&array[i*a.NR_ELEM];
        VT sv2 = *(VT*)&array[(i+1)*a.NR_ELEM];
        res ^= tvx::hxor(sv1)[0] ^ tvx::hxor(sv2)[0];
    }
    if (a.nr_chunks % 2) {
        VT sv = *(VT*)&array[(a.nr_chunks-1)*a.NR_ELEM];
        res ^= tvx::hxor(sv)[0];
    }
#endif

    //FIXME TODO once intrinsic has mask, rewrite with mask
    for (std::size_t i = a.nr_chunks*a.NR_ELEM; i < a.nr_elem; ++i) {
        res ^= array[i];
    }
    /*if (a.nr_tail_elem) {
        tvx::pmask_t mask = tvx::_pmask(a.nr_tail_elem * sizeof(T));
        VT sv = tvx::vld(mask, (VT*)&array[a.nr_chunks*a.NR_ELEM]);
        res &= tvx::hand(sv)[0]; //FIXME use mask
    }*/

    return res;
}

/// Reduction xor of a contiguous sequence of objects.
///
/// span can be C array or STL container.
///
template<typename T, std::size_t VLEN = 512>
T array_xor(std::span<const T> array)
{
    return vil::array_xor<T,VLEN>(array.data(), array.size());
}
/// Sum of a[i]*a[i+1] pairs.
///
///
template<typename T, std::size_t VLEN = 512>
T array_mul_pair_sum(const T* array, std::size_t nr_elem)
{
    using AI = array_info<T,VLEN>;
    AI a(nr_elem);
    using VT = AI::VT;

    T sum {0};

    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT sv = *(VT*)&array[i*a.NR_ELEM];
        sum += vil::insn::hmpsum(sv)[0];
    }

    //FIXME TODO once intrinsic has mask, rewrite with mask
    for (std::size_t i = a.nr_chunks*a.NR_ELEM; i < a.nr_elem; i += 2) {
        sum += array[i] * array[i+1];
    }

    return sum;
}

template<typename T, std::size_t VLEN = 512>
T array_mul_pair_sum(std::span<const T> array)
{
    return vil::array_mul_pair_sum<T,VLEN>(array.data(), array.size());
}

// AUTOGENERATED

/// Reduction product of all elements.
///
///
template<typename T, std::size_t VLEN = 512>
T array_product(const T* array, std::size_t nr_elem)
{
    using AI = array_info<T,VLEN>;
    AI a(nr_elem);
    using VT = AI::VT;

    T res { 1  };

#if 0
    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT sv = *(VT*)&array[i*a.NR_ELEM];
        res *= vil::insn::hprod(sv)[0];
    }
#else
    for (std::size_t i = 0; (i+1) < a.nr_chunks; i += 2) {
        VT sv1 = *(VT*)&array[i*a.NR_ELEM];
        VT sv2 = *(VT*)&array[(i+1)*a.NR_ELEM];
        res *= vil::insn::hprod(sv1)[0] * vil::insn::hprod(sv2)[0];
    }
    if (a.nr_chunks % 2) {
        VT sv = *(VT*)&array[(a.nr_chunks-1)*a.NR_ELEM];
        res *= vil::insn::hprod(sv)[0];
    }
#endif

    //FIXME TODO once intrinsic has mask, rewrite with mask
    for (std::size_t i = a.nr_chunks*a.NR_ELEM; i < a.nr_elem; ++i) {
        res *= array[i];
    }
    /*if (a.nr_tail_elem) {
        tvx::pmask_t mask = tvx::_pmask(a.nr_tail_elem * sizeof(T));
        VT sv = tvx::vld(mask, (VT*)&array[a.nr_chunks*a.NR_ELEM]);
        res &= tvx::hand(sv)[0]; //FIXME use mask
    }*/

    return res;
}

/// Reduction product of a contiguous sequence of objects.
///
/// span can be C array or STL container.
///
template<typename T, std::size_t VLEN = 512>
T array_product(std::span<const T> array)
{
    return vil::array_product<T,VLEN>(array.data(), array.size());
}
/// Find element with maximum value.
///
///
template<typename T, std::size_t VLEN = 512>
T array_find_max(const T* array, std::size_t nr_elem)
{
    using AI = array_info<T,VLEN>;
    AI a(nr_elem);
    using VT = AI::VT;

    T maxel {array[0]};

    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT v = *(VT*)&array[i*a.NR_ELEM];
        maxel = std::max(maxel, vil::insn::hmax(v)[0]);
    }

    //FIXME TODO once hsum intrinsic has mask, rewrite with hsum+mask
    for (std::size_t i = a.nr_chunks*a.NR_ELEM; i < a.nr_elem; ++i) {
        maxel = std::max(maxel, array[i]);
    }

    return maxel;
}

template<typename T, std::size_t VLEN = 512>
T array_find_max(std::span<const T> array)
{
    return vil::array_find_max<T,VLEN>(array.data(), array.size());
}

/// Find element with minimum value.
///
///
template<typename T, std::size_t VLEN = 512>
T array_find_min(const T* array, std::size_t nr_elem)
{
    using AI = array_info<T,VLEN>;
    AI a(nr_elem);
    using VT = AI::VT;

    T minel {array[0]};

    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT v = *(VT*)&array[i*a.NR_ELEM];
        minel = std::min(minel, vil::insn::hmin(v)[0]);
    }

    //FIXME TODO once hsum intrinsic has mask, rewrite with hsum+mask
    for (std::size_t i = a.nr_chunks*a.NR_ELEM; i < a.nr_elem; ++i) {
        minel = std::min(minel, array[i]);
    }

    return minel;
}

template<typename T, std::size_t VLEN = 512>
T array_find_min(std::span<const T> array)
{
    return vil::array_find_min<T,VLEN>(array.data(), array.size());
}

/// Find first element with specified value.
///
///
template<typename T, std::size_t VLEN = 512>
std::optional<std::size_t>
array_find_first(const T val, const T* array, std::size_t nr_elem)
{
    using AI = array_info<T,VLEN>;
    AI a(nr_elem);
    using VT = AI::VT;

    VT vv = val - VT{};

    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT v = *(VT*)&array[i*a.NR_ELEM];
#if 1
        VT res = tvx::veq(v, vv);
        for (std::size_t j = 0; j < a.NR_ELEM; ++j) {
//printf("-- %lu:%lu: %lu=%lu cmp %lu\n", i, j, res[j], v[j], vv[j]);
            if (res[j] != 0) {
                return i*a.NR_ELEM + j;
            }
        }
#else
        tvx::pmask_t mask = vil::insn::veq2p(v, vv)
        uint64_t bits = mov_p2r(tvx::pmask_t src);
        if (bits) {
            ? = __builtin_ctz(bits);
            return ?;
        }
#endif
    }

    for (std::size_t i = a.nr_chunks*a.NR_ELEM; i < a.nr_elem; ++i) {
        if (array[i] == val) { return i; }
    }

    return std::nullopt;
}

template<typename T, std::size_t VLEN = 512>
std::optional<std::size_t>
array_find_first(const T val, std::span<const T> array)
{
    return vil::array_find_first<T,VLEN>(val, array.data(), array.size());
}

// AUTOGENERATED

/// add elements of 2 arrays.
///
///
template<typename T, std::size_t VLEN = 512>
void arrays_add(T* dst_array, const T* array1, const T* array2, std::size_t nr_elem)
{
    using AI = array_info<T,VLEN>;
    AI a(nr_elem);
    using VT = AI::VT;

    #pragma GCC unroll 2
    #pragma GCC ivdep // asserts that there are no loop-carried dependencies
    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT v1 = *(VT*)&array1[i*a.NR_ELEM];
        VT v2 = *(VT*)&array2[i*a.NR_ELEM];
        VT dv = tvx::vadd(v1, v2);
        //FIXME tvx::vst((VT*)&dst_array[i*a.VNR_ELEM], /*P0*/tvx::_pmask(63/*VSZ1*/), dv);
        __builtin_memcpy(&dst_array[i*a.NR_ELEM], &dv, sizeof(dv));
    }

    //FIXME TODO once intrinsic has mask, rewrite with mask
    if (a.nr_tail_elem) {
        tvx::pmask_t mask = tvx::_pmask(a.nr_tail_elem * sizeof(T));
        VT v1 = tvx::vld(mask, (VT*)&array1[a.nr_chunks*a.NR_ELEM]);
        VT v2 = tvx::vld(mask, (VT*)&array2[a.nr_chunks*a.NR_ELEM]);
        VT dv = tvx::vadd(v1, v2); //FIXME use mask
        tvx::vst((VT*)&dst_array[a.nr_chunks*a.NR_ELEM], mask, dv);
    }
}

template<typename T, std::size_t VLEN = 512>
void arrays_add(std::span<T> dst_array, std::span<const T> array1, std::span<const T> array2)
{
    assert(array2.size() >= array1.size());
    assert(dst_array.size() >= array1.size());

    vil::arrays_add<T,VLEN>(dst_array.data(), array1.data(), array2.data(), array1.size());
}
// AUTOGENERATED

/// mul elements of 2 arrays.
///
///
template<typename T, std::size_t VLEN = 512>
void arrays_mul(T* dst_array, const T* array1, const T* array2, std::size_t nr_elem)
{
    using AI = array_info<T,VLEN>;
    AI a(nr_elem);
    using VT = AI::VT;

    #pragma GCC unroll 2
    #pragma GCC ivdep // asserts that there are no loop-carried dependencies
    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT v1 = *(VT*)&array1[i*a.NR_ELEM];
        VT v2 = *(VT*)&array2[i*a.NR_ELEM];
        VT dv = tvx::vmul(v1, v2);
        //FIXME tvx::vst((VT*)&dst_array[i*a.VNR_ELEM], /*P0*/tvx::_pmask(63/*VSZ1*/), dv);
        __builtin_memcpy(&dst_array[i*a.NR_ELEM], &dv, sizeof(dv));
    }

    //FIXME TODO once intrinsic has mask, rewrite with mask
    if (a.nr_tail_elem) {
        tvx::pmask_t mask = tvx::_pmask(a.nr_tail_elem * sizeof(T));
        VT v1 = tvx::vld(mask, (VT*)&array1[a.nr_chunks*a.NR_ELEM]);
        VT v2 = tvx::vld(mask, (VT*)&array2[a.nr_chunks*a.NR_ELEM]);
        VT dv = tvx::vmul(v1, v2); //FIXME use mask
        tvx::vst((VT*)&dst_array[a.nr_chunks*a.NR_ELEM], mask, dv);
    }
}

template<typename T, std::size_t VLEN = 512>
void arrays_mul(std::span<T> dst_array, std::span<const T> array1, std::span<const T> array2)
{
    assert(array2.size() >= array1.size());
    assert(dst_array.size() >= array1.size());

    vil::arrays_mul<T,VLEN>(dst_array.data(), array1.data(), array2.data(), array1.size());
}
// AUTOGENERATED

/// and elements of 2 arrays.
///
///
template<typename T, std::size_t VLEN = 512>
void arrays_and(T* dst_array, const T* array1, const T* array2, std::size_t nr_elem)
{
    using AI = array_info<T,VLEN>;
    AI a(nr_elem);
    using VT = AI::VT;

    #pragma GCC unroll 2
    #pragma GCC ivdep // asserts that there are no loop-carried dependencies
    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT v1 = *(VT*)&array1[i*a.NR_ELEM];
        VT v2 = *(VT*)&array2[i*a.NR_ELEM];
        VT dv = tvx::vand(v1, v2);
        //FIXME tvx::vst((VT*)&dst_array[i*a.VNR_ELEM], /*P0*/tvx::_pmask(63/*VSZ1*/), dv);
        __builtin_memcpy(&dst_array[i*a.NR_ELEM], &dv, sizeof(dv));
    }

    //FIXME TODO once intrinsic has mask, rewrite with mask
    if (a.nr_tail_elem) {
        tvx::pmask_t mask = tvx::_pmask(a.nr_tail_elem * sizeof(T));
        VT v1 = tvx::vld(mask, (VT*)&array1[a.nr_chunks*a.NR_ELEM]);
        VT v2 = tvx::vld(mask, (VT*)&array2[a.nr_chunks*a.NR_ELEM]);
        VT dv = tvx::vand(v1, v2); //FIXME use mask
        tvx::vst((VT*)&dst_array[a.nr_chunks*a.NR_ELEM], mask, dv);
    }
}

template<typename T, std::size_t VLEN = 512>
void arrays_and(std::span<T> dst_array, std::span<const T> array1, std::span<const T> array2)
{
    assert(array2.size() >= array1.size());
    assert(dst_array.size() >= array1.size());

    vil::arrays_and<T,VLEN>(dst_array.data(), array1.data(), array2.data(), array1.size());
}
// AUTOGENERATED

/// add_abs elements of 2 arrays.
///
///
template<typename T, std::size_t VLEN = 512>
void arrays_add_abs(T* dst_array, const T* array1, const T* array2, std::size_t nr_elem)
{
    using AI = array_info<T,VLEN>;
    AI a(nr_elem);
    using VT = AI::VT;

    #pragma GCC unroll 2
    #pragma GCC ivdep // asserts that there are no loop-carried dependencies
    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT v1 = *(VT*)&array1[i*a.NR_ELEM];
        VT v2 = *(VT*)&array2[i*a.NR_ELEM];
        VT dv = tvx::vadda(v1, v2);
        //FIXME tvx::vst((VT*)&dst_array[i*a.VNR_ELEM], /*P0*/tvx::_pmask(63/*VSZ1*/), dv);
        __builtin_memcpy(&dst_array[i*a.NR_ELEM], &dv, sizeof(dv));
    }

    //FIXME TODO once intrinsic has mask, rewrite with mask
    if (a.nr_tail_elem) {
        tvx::pmask_t mask = tvx::_pmask(a.nr_tail_elem * sizeof(T));
        VT v1 = tvx::vld(mask, (VT*)&array1[a.nr_chunks*a.NR_ELEM]);
        VT v2 = tvx::vld(mask, (VT*)&array2[a.nr_chunks*a.NR_ELEM]);
        VT dv = tvx::vadda(v1, v2); //FIXME use mask
        tvx::vst((VT*)&dst_array[a.nr_chunks*a.NR_ELEM], mask, dv);
    }
}

template<typename T, std::size_t VLEN = 512>
void arrays_add_abs(std::span<T> dst_array, std::span<const T> array1, std::span<const T> array2)
{
    assert(array2.size() >= array1.size());
    assert(dst_array.size() >= array1.size());

    vil::arrays_add_abs<T,VLEN>(dst_array.data(), array1.data(), array2.data(), array1.size());
}

} // end of namespace vil
