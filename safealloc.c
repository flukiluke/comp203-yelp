/*
   safealloc.c: Wrappers around malloc() realloc(). Functions exit with an
   error message if calls fail.
   
   This file is part of COMP20003 Assignment 1,
   Luke Ceddia, <lceddia@student.unimelb.edu.au>
   Student ID: 834076
 */

#include <stdio.h>
#include <stdlib.h>
#include "safealloc.h"

void *safe_malloc(size_t size) {
  void *p;
  p = malloc(size);
  if (!p) {
    printf("Failed to allocate %d bytes\n", (int)size);
    exit(EXIT_FAILURE);
  }
  return p;
}


void *safe_realloc(void *ptr, size_t size) {
  void *p;
  p = realloc(ptr, size);
  if (!p) {
    printf("Failed to reallocate %d bytes\n", (int)size);
    exit(EXIT_FAILURE);
  }
  return p;
}

