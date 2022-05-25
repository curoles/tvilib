/// and elements of 2 arrays.
///
///
template<typename T, std::size_t VLEN = 512>
void arrays_and(T* dst_array, const T* array1, const T* array2, std::size_t nr_elem)
{
    using AI = array_info<T,VLEN>;
    AI a(nr_elem);
    using VT = AI::VT;

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