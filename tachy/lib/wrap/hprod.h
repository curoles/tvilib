namespace insn {

template<typename VT>
VT hprod(VT v)
{
    using T = tvx::get_base<VT>::type;

    if constexpr(std::is_floating_point<T>::value) {
        return tvx::hprodso(v);
    }
    //else if constexpr(std::is_signed<T>::value) {
    //    return tvx::xxx(v);
    //}
    else {
        return tvx::hprod(v);
    }
}

}
