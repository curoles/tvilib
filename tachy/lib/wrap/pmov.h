namespace insn {

//
// “r” is a GPR “x” is a FPR/VPR and “Yp” is a predicate reg
//
// what if I need to specify size of “r” or “x”? like b,h,w,l
// size will be automagically determined based on the size of the input/output object
//
static inline
tvx::pmask_t mov_r2p(uint64_t src)
{
    tvx::pmask_t dst;
    asm("pmov %[d], %[s]"
            : [d] "=Yp" (dst)
            : [s] "r" (src)
            :);
    return dst;
}

static inline
uint64_t mov_p2r(tvx::pmask_t src)
{
    uint64_t dst;
    asm("pmov %[d], %[s]"
            : [d] "=r" (dst)
            : [s] "Yp" (src)
            :);
    return dst;
}

}
