

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
