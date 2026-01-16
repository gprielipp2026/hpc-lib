# Timing Library for HPC

Developed by George Prielipp for SI458 - High Performance Computing.

## Installation

1. Determine your include path
   1. `gcc -v -x c -E /dev/null`
   2. Look for "`#include <...> search starts here:`"
2. `git clone --recurse-submodules <this library>` in one of the directories listed that you have access to.
3. Run `make` in `/hpc-lib`
4. Modify your `Makefile` in your project to include:
   1. `LDFLAGS=-L<location you installed>/hpc-lib/ -rdynamic`
   2. `LDLIBS=-lhpc`
   3. `CFLAGS=-finstrument-functions`
