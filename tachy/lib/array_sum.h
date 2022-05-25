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

