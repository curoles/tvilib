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
