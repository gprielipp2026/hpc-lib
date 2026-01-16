#define _GNU_SOURCE

#include "timing.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <dlfcn.h>

// UTHASH
#include "uthash/include/uthash.h"

/**
 * information I want to collect on a function
 */
typedef struct tinfo {
  clock_t start;
  clock_t end;
  struct tinfo* next;
} tinfo_t;

typedef struct {
  char* func_name; /* key for struct hashing */
  uint32_t times_called;
  tinfo_t* timing; 
  UT_hash_handle hh; /* make the struct hashable */
} finfo_t;

// GLOBAL VARIABLE 
finfo_t *table;
// GLOBAL VARIABLE

/**
 * add a new entry to the finfo
 */
__attribute__ ((no_instrument_function))
void timing_add(finfo_t* finfo)
{ 
  // need to get the current time
  clock_t start = clock();

  // insert onto the head
  tinfo_t* tinfo = malloc(sizeof(*tinfo));
  tinfo->start = start;
  tinfo->next  = finfo->timing;
  
  finfo->timing = tinfo;
}

/**
 * find the last tinfo entry and put the end time in
 */
__attribute__ ((no_instrument_function))
void timing_stop(finfo_t* finfo)
{ 
  // need to get the current time
  clock_t end = clock();

  finfo->timing->end = end;
}

/**
 * clean up the hash table and free all associated memory
 * this function is automatically placed after main exits but before the program terminates
 */
__attribute__ ((no_instrument_function, destructor))
void timing_kill()
{ 
  // safely iterate over hash table and remove entries
  finfo_t* finfo, *tmp;

  HASH_ITER(hh, table, finfo, tmp) {
    // remove from table
    HASH_DEL(table, finfo);

    // free memory in finfo
    free(finfo->func_name);
    for(tinfo_t* tinfo = finfo->timing; tinfo != NULL; ) {
      tinfo_t* next = tinfo->next;

      free(tinfo);

      tinfo = next;
    }

    // free finfo
    free(finfo);
  }
}

/**
 * print the formatted time information
 */
__attribute((no_instrument_function)) 
void print_time(clock_t time) {
  int millis = time * 1000 / CLOCKS_PER_SEC;
  printf("Time: %d sec %d millisec\n", millis/1000, millis%1000);
}

/**
 * nicely display information that was gathered
 */
__attribute((no_instrument_function))
void timing_print() 
{
  finfo_t* finfo;

  for(finfo = table; finfo != NULL; finfo = finfo->hh.next) {
    // need to print the timing information out now
    printf("---------------\n");
    printf("%s:\n", finfo->func_name);
    clock_t totalTime = 0;
    for(tinfo_t* tinfo = finfo->timing; tinfo != NULL; tinfo = tinfo->next) {
      clock_t diff = tinfo->end - tinfo->start;
      totalTime += diff;

      print_time(diff);
    }
    printf("Total Time: ");
    print_time(totalTime);
    printf("Average Time: ");
    print_time(totalTime / finfo->times_called);
    printf("\n");
  }
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
    // Step 1
     const char* name = info.dli_sname ? info.dli_sname : "NOT FOUND";
    finfo_t* finfo;
    HASH_FIND_STR(table, name, finfo);
    // n
    if(finfo == NULL) {
      // need to add it to the table
      finfo = malloc(sizeof(*finfo));
      strcpy(finfo->func_name, name);
      finfo->times_called = -1;
      timing_add(finfo);
      // HASH_ADD_KEYPTR(hh_name, head, key_ptr, key_len, item_ptr)
      HASH_ADD_KEYPTR(hh, table, &finfo->func_name, strlen(finfo->func_name), finfo);
    }
    // y / step 2
    finfo->times_called++;

    // step 3
    timing_add(finfo);

    // step 4 -> not required with this hashing library
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
     
     */
    // Step 1
    const char* name = info.dli_sname ? info.dli_sname : "NOT FOUND";
    finfo_t* finfo;
    // step 2
    HASH_FIND_STR(table, name, finfo);
    // step 3
    timing_stop(finfo);

    // step 4 -> not required with this hashing library
    } else {
    // ?? error out ??
  }
}
