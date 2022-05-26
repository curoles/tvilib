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

