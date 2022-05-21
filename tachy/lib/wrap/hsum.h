namespace insn {

//TODO extended flavor when sizeof(TDst)=2*sizeof(T)
//
template<typename VT, typename VTDst=VT>
VTDst hsum(VT v)
{
    using T = tvx::get_base<VT>::type;

    if constexpr(std::is_floating_point<T>::value) {
        return tvx::hsumso(v);
    }
    else if constexpr(std::is_signed<T>::value) {
        //FIXME return tvx::hsums(v); //TODO add hsums intrinsic
    }
    else {
        return tvx::hsum(v);
    }
}

}
