#include "timing.h"

#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <dlfcn.h>

/**
 * information I want to collect on a function
 */
typedef struct tinfo {
  clock_t start;
  clock_t end;
  struct tinfo* next; 
} tinfo_t;

typedef struct {
  char* func_name;
  uint32_t times_called;
  tinfo_t* timing; 
} finfo_t;

/**
 * define what the ttable is actually going to hold
 */
typedef struct {


} ttable_t;

// GLOBAL VARIABLE 
ttable_t* ttable;
// GLOBAL VARIABLE

/**
 * initialize the table datastructure
 * tie into the system to intercept library calls
 */
__attribute__ ((no_instrument_function, constructor))
void timing_start(ttable_t* table)
{

}

/**
 * disentangle the library from the system
 */
__attribute__ ((no_instrument_function))
void timing_stop(ttable_t* table)
{

}

/**
 * nicely display information that was gathered
 */
__attribute__ ((no_instrument_function))
void timing_print()
{

}

/**
 * clean up any memory that was stored in the table
 */
__attribute__ ((no_instrument_function, destructor)) 
void timing_free(ttable_t* table)
{

}

// ------------------------------------------------------
// heart of the program <-- the things that gcc ties into

/**
 * Called when a function is called and is about to start
 */
__attribute__ ((no_instrument_function))
void __cyg_profile_func_enter (void *this_fn, void *call_site)
{
  // get the name of this function
  Dl_info info;
  if(dladdr(this_fn, &info)) {
    // modify the ttable
    /**
     * 1. does info.dli_sname exist in ttable?
     *   y: increase count
     *   n: create new entry in ttable (create finfo with the necessary information)
     * 2. grab the finfo entry from the ttable
     * 3. create new tinfo with the start time
     * 4. add the tinfo entry to the finfo entry
     */
  } else {
    // ?? error out ??
  }
}

/**
 * Called when a function returns
 */
__attribute__ ((no_instrument_function))
void __cyg_profile_func_exit  (void *this_fn, void *call_site)
{
  // get the name of this function
  Dl_info info;
  if(dladdr(this_fn, &info)) {
    // modify the ttable
    /**
     * 1. grab the finfo entry from the ttable
     * 2. grab the tinfo entry from the finfo entry 
     * 3. add the end time to the tinfo entry
     */} else {
    // ?? error out ??
  }
}
