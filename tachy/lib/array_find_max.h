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
TRes array_find_max(std::span<const T> array)
{
    return vil::array_find_max(array.data(), array.size());
}

