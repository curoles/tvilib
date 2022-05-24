namespace insn {

template<typename VT>
VT hmax(VT v)
{
    using T = tvx::get_base<VT>::type;

    if constexpr(std::is_floating_point<T>::value) {
        return tvx::hmax(v); //TODO hmaxn?
    }
    else if constexpr(std::is_signed<T>::value) {
        return tvx::hmaxs(v);
    }
    else {
        return tvx::hmax(v);
    }
}

}
