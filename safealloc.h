/* 
   safealloc.h: Routines for calling dynamic memory function with error
   checking.

   This file is part of COMP20003 Assignment 1,
   Luke Ceddia, <lceddia@student.unimelb.edu.au>
   Student ID: 834076

   Functions only return on success; the program exits otherwise.
 */

#include <stdlib.h>

void *safe_malloc(size_t size);
void *safe_realloc(void *ptr, size_t size);
