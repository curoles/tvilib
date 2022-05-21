#pragma once

#include <type_traits>
#include <vxintrin.hpp>

namespace vil {

constexpr unsigned int VSZ1 = 512 / 8;

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


/// Sum of all elements.
///
/// It is recommended to use
/// sizeof(TRes) = 2 * sizeof(T) for integral types.
///
template<typename T, typename TRes=T>
TRes array_sum(const T* array, unsigned int nr_elem)
{
    constexpr unsigned int VNR_ELEM = VSZ1 / sizeof(T);

    const unsigned int nr_bytes = nr_elem * sizeof(T);

    const unsigned int nr_chunks = nr_bytes / VSZ1;

    using VT = tvx::make<T,VNR_ELEM>::type;

    TRes sum {0};

    for (unsigned int i = 0; i < nr_chunks; ++i) {
        VT v = *(VT*)&array[i*VNR_ELEM];
        sum += vil::insn::hsum(v)[0];
    }

    //FIXME TODO once hsum intrinsic has mask, rewrite with hsum+mask
    for (unsigned int i = nr_chunks*VNR_ELEM; i < nr_elem; ++i) {
        sum += array[i];
    }

    return sum;
}

} // end of namespace vil
