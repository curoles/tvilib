namespace insn {

template<typename VT>
VT vmadd(VT a, VT b, VT c)
{
    using T = tvx::get_base<VT>::type;

    if constexpr(std::is_floating_point<T>::value) {
        return tvx::vmadd(a, b, c);
    }
    else if constexpr(std::is_signed<T>::value) {
        return tvx::vmadds(a, b, c);
    }
    else {
        return tvx::vmadd(a, b, c);
    }
}

}
