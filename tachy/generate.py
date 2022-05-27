#!/usr/bin/env python3
#
# Combine all header files into one.
# Copyright Igor Lesik 2022

from pathlib import Path
import jinja2

parts = [
    'header.h',
    'wrap/hsum.h', 'wrap/hmsum.h', 'wrap/hmax.h', 'wrap/hmin.h',
    'wrap/hmpsum.h', 'wrap/hprod.h',
    'array_sum.h', 'arrays_msum.h',
    'array_and.h', 'array_or.h', 'array_xor.h',
    'array_mul_pair_sum.h', 'array_product.h',
    'array_find_max.h', 'array_find_min.h',
    'array_find_first.h',
    'arrays_add.h', 'arrays_mul.h', 'arrays_and.h',
    'arrays_add_abs.h',
    'footer.h'
    ]

arrays_binary_ops = [
    ['arrays_add.h', 'add', 'tvx::vadd'],
    ['arrays_mul.h', 'mul', 'tvx::vmul'],
    ['arrays_add_abs.h', 'add_abs', 'tvx::vadda'],
    ['arrays_and.h', 'and', 'tvx::vand'],
    ]

array_reduction_ops = [
    ['array_or.h', 'or', 'tvx::hor', '|', '0'],
    ['array_and.h', 'and', 'tvx::hand', '&', '~0ul'],
    ['array_xor.h', 'xor', 'tvx::hxor', '^', '0'],
    ['array_product.h', 'product', 'vil::insn::hprod', '*', '1'],
    ]

def agglomerate(wrkdir: Path, parts: []):
    with open(wrkdir / 'vilib.h', 'w') as file:
        for part in parts:
            file.write( open(wrkdir / 'lib' / part, 'r').read() )
        file.close()

def generate_arrays_binary_operations(env, wrkdir: Path):
    template = env.get_template("arrays_bop.h")
    for op in arrays_binary_ops:
        txt = template.render(name=op[1], vop=op[2])
        with open(wrkdir / 'lib' / op[0], 'w') as file:
            file.write(txt)

def generate_array_reduction_operations(env, wrkdir: Path):
    template = env.get_template("array_rop.h")
    for op in array_reduction_ops:
        txt = template.render(name=op[1], vop=op[2], op=op[3], init=op[4])
        with open(wrkdir / 'lib' / op[0], 'w') as file:
            file.write(txt)

def main():
    script_dir = Path( __file__ ).parent.absolute()
    jinja_env = jinja2.Environment(loader=jinja2.PackageLoader("generate"))
    generate_arrays_binary_operations(jinja_env, script_dir)
    generate_array_reduction_operations(jinja_env, script_dir)
    agglomerate(script_dir, parts)
    return

main()
