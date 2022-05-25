/// Reduction AND of all elements.
///
///
template<typename T, std::size_t VLEN = 512>
T array_and(const T* array, std::size_t nr_elem)
{
    using AI = array_info<T,VLEN>;
    AI a(nr_elem);
    using VT = AI::VT;

    T res {~0ul};

    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT sv = *(VT*)&array[i*a.NR_ELEM];
        res &= tvx::hand(sv)[0];
    }

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

/// Reduction AND of a contiguous sequence of objects.
///
/// span can be C array or STL container.
///
template<typename T, std::size_t VLEN = 512>
T array_and(std::span<const T> array)
{
    return vil::array_and<T,VLEN>(array.data(), array.size());
}

