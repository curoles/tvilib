namespace insn {

template<typename VT>
VT hmsum(VT v1, VT v2)
{
    using T = tvx::get_base<VT>::type;

    if constexpr(std::is_floating_point<T>::value) {
        return tvx::hmsumso(v1, v2);
    }
    else if constexpr(std::is_signed<T>::value) {
        return tvx::hmsum(v1, v2);
    }
    else {
        return tvx::hmsum(v1, v2);
    }
}

}
