/// Find first element with specified value.
///
///
template<typename T, std::size_t VLEN = 512>
std::optional<std::size_t>
array_find_first(const T val, const T* array, std::size_t nr_elem)
{
    using AI = array_info<T,VLEN>;
    AI a(nr_elem);
    using VT = AI::VT;

    VT vv = val - VT{};

    for (std::size_t i = 0; i < a.nr_chunks; ++i) {
        VT v = *(VT*)&array[i*a.NR_ELEM];
#if 1
        VT res = tvx::veq(v, vv);
        for (std::size_t j = 0; j < a.NR_ELEM; ++j) {
//printf("-- %lu:%lu: %lu=%lu cmp %lu\n", i, j, res[j], v[j], vv[j]);
            if (res[j] != 0) {
                return i*a.NR_ELEM + j;
            }
        }
#else
        tvx::pmask_t mask = vil::insn::veq2p(v, vv)
        uint64_t bits = mov_p2r(tvx::pmask_t src);
        if (bits) {
            ? = __builtin_ctz(bits);
            return ?;
        }
#endif
    }

    for (std::size_t i = a.nr_chunks*a.NR_ELEM; i < a.nr_elem; ++i) {
        if (array[i] == val) { return i; }
    }

    return std::nullopt;
}

template<typename T, std::size_t VLEN = 512>
std::optional<std::size_t>
array_find_first(const T val, std::span<const T> array)
{
    return vil::array_find_first<T,VLEN>(val, array.data(), array.size());
}

