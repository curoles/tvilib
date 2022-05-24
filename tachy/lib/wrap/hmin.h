namespace insn {

template<typename VT>
VT hmin(VT v)
{
    using T = tvx::get_base<VT>::type;

    if constexpr(std::is_floating_point<T>::value) {
        return tvx::hmin(v); //TODO hminn?
    }
    else if constexpr(std::is_signed<T>::value) {
        return tvx::hmins(v);
    }
    else {
        return tvx::hmin(v);
    }
}

}
