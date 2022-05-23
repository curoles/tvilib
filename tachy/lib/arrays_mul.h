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
