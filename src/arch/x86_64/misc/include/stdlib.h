

#ifndef __STDLIB_H__
#define __STDLIB_H__


/* -- Includes ------------------------------------------------- */
#include <system.h>


/* -- Prototypes ----------------------------------------------- */


/*
 *  int rand()
 *      Taken from the K&R C programming language book. Page 46.
 *      returns a pseudo-random integer of 0..32767. Note that
 *      this is compatible with the System V function rand(), not
 *      with the bsd function rand() that returns 0..(2**31)-1.
 */
int rand ();

/*
 *  srand(seed)
 *      companion routine to rand(). Initializes the seed.
 */
void srand(unsigned int seed);


#endif

