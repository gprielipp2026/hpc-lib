#ifndef TIMING_H
#define TIMING_H

/**
 * This is a library designed to make timing HPC labs easier
 *
 * Example use:
 * 
 * ttable_t table;
 * timing_start(&table);
 *
 * foo();
 * bar();
 * bat();
 *
 * timing_stop(&table);
 *
 * timing_print(&table);
 *
 * timing_free(&table);
 */

struct ttable;

typedef struct ttable ttable_t;

/**
 * initialize the table datastructure
 * tie into the system to intercept library calls
 */
void timing_start(ttable_t* table);

/**
 * disentangle the library from the system
 */
void timing_stop(ttable_t* table);

/**
 * nicely display information that was gathered
 */
void timing_print(ttable_t* table);

/**
 * clean up any memory that was stored in the table
 */
void timing_free(ttable_t* table);

#endif//TIMING_H
