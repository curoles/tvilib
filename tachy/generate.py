#!/usr/bin/env python3
#
# Combine all header files into one.
# Copyright Igor Lesik 2022

from pathlib import Path
import jinja2

parts = [
    'header.h',
    'wrap/hsum.h', 'wrap/hmsum.h', 'wrap/hmax.h', 'wrap/hmin.h',
    'array_sum.h', 'arrays_msum.h', 'array_and.h',
    'array_find_max.h', 'array_find_min.h',
    'arrays_add.h', 'arrays_mul.h',
    'arrays_add_abs.h',
    'footer.h'
    ]

arrays_binary_ops = [
    ['arrays_add.h', 'add', 'tvx::vadd', 'array1[i] + array2[i]'],
    ['arrays_mul.h', 'mul', 'tvx::vmul', 'array1[i] * array2[i]'],
    ['arrays_add_abs.h', 'add_abs', 'tvx::vadda', 'std::abs(array1[i]) + std::abs(array2[i])'],
    ]

def agglomerate(wrkdir: Path, parts: []):
    with open(wrkdir / 'vilib.h', 'w') as file:
        for part in parts:
            file.write( open(wrkdir / 'lib' / part, 'r').read() )
        file.close()

def generate_arrays_binary_operations(wrkdir: Path):
    env = jinja2.Environment(loader=jinja2.PackageLoader("generate"))
    template = env.get_template("arrays_bop.h")
    for op in arrays_binary_ops:
        txt = template.render(name=op[1], vop=op[2], op=op[3])
        with open(wrkdir / 'lib' / op[0], 'w') as file:
            file.write(txt)

def main():
    script_dir = Path( __file__ ).parent.absolute()
    generate_arrays_binary_operations(script_dir)
    agglomerate(script_dir, parts)
    return

main()
