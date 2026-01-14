# Timing Library for HPC

Developed by George Prielipp for SI458 - High Performance Computing.

## Installation

1. Determine your include path
   1. `g++ -v -x c -E /dev/null`
   2. Look for "`#include <...> search starts here:`"
2. `git clone <this library>` in one of the directories listed that you have access to.

## Use

```c

#include <hpc/timing.h>

...

// initialize the library
ttable_t table;
timing_start(&table);

// do the HPC lab / call functions
foo();
bar();
bat();

// stop the library
timing_stop(&table);

// display results to the screen
timing_print(&table);

// free any memory the library used
timing_free(&table);

```
