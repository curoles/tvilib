namespace insn {

template<typename VT, std::size_t VLEN>
VT vfill(typename tvx::get_base<VT>::type v)
{
    using T = tvx::get_base<VT>::type;
    //FIXME static_assert(T and Tv are the same type)

    if constexpr(VLEN == 512) {
        if constexpr(sizeof(T) == sizeof(uint64_t))
            return tvx::__vx512_fill_l(v);
        else if constexpr(sizeof(T) == sizeof(uint32_t))
            return tvx::__vx512_fill_w(v);
        else if constexpr(sizeof(T) == sizeof(uint16_t))
            return tvx::__vx512_fill_h(v);
        else if constexpr(sizeof(T) == sizeof(uint8_t))
            return tvx::__vx512_fill_b(v);
    }
    else if constexpr(VLEN == 256) {
        if constexpr(sizeof(T) == sizeof(uint64_t))
            return tvx::__vx256_fill_l(v);
        else if constexpr(sizeof(T) == sizeof(uint32_t))
            return tvx::__vx256_fill_w(v);
        else if constexpr(sizeof(T) == sizeof(uint16_t))
            return tvx::__vx256_fill_h(v);
        else if constexpr(sizeof(T) == sizeof(uint8_t))
            return tvx::__vx256_fill_b(v);
    }

}

}
