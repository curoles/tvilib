/// Find element with minimum value.
///
///
template<typename T, typename TRes=T>
TRes array_find_min(const T* array, std::size_t nr_elem)
{
    array_info<T> a(nr_elem);
    using VT = array_info<T>::VT;

    TRes minel {array[0]};

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

template<typename T, typename TRes=T>
TRes array_find_min(std::span<const T> array)
{
    return vil::array_find_min(array.data(), array.size());
}

