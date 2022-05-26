namespace insn {

template<typename VT>
VT hmpsum(VT v)
{
    using T = tvx::get_base<VT>::type;

    if constexpr(std::is_floating_point<T>::value) {
        return tvx::hmpsumso(v);
    }
    else {
        //return tvx::hmpsum(v);
    }
}

}
