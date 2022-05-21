#!/usr/bin/env python3
#
# Combine all header files into one.
# Copyright Igor Lesik 2022

from pathlib import Path

parts = [
    'header.h',
    'wrap/hsum.h',
    'array_sum.h',
    'footer.h'
    ]

def agglomerate(wrkdir: Path, parts: []):
    with open(wrkdir / 'vilib.h', 'w') as file:
        for part in parts:
            file.write( open(wrkdir / 'lib' / part, 'r').read() )
        file.close()

def main():
    script_dir = Path( __file__ ).parent.absolute()
    agglomerate(script_dir, parts)
    return

main()
