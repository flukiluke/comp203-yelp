/*
   io.c: Routines for inputting and outputting data, for files and screen.
   
   This file is part of COMP20003 Assignment 1,
   Luke Ceddia, <lceddia@student.unimelb.edu.au>
   Student ID: 834076

   These routines handle all interaction with the outside world: loading and
   parsing input files, getting queries, and outputting results.

 */

#include <stdio.h>
#include <string.h>
#include "btree.h"
#include "safealloc.h"
#include "io.h"

static int get_filesize(FILE *file);

/* Initialise the database from a file on disk, returning the associated
   btree structure. The entire file is loaded into memory, NUL bytes are
   inserted to delimit the strings appropriately (replacing the newline
   characters), and the btree index is setup. We malloc() 1 byte more than
   the size of the file to allow for the case where there is no final newline
   character. Supports CRLF and LF linendings (though the former means 1 byte
   of wasted space per line). */
struct btree_t *read_db(char *filename) {
  FILE *file;
  int size;
  char *data_start, *p;
  char *key, *value;
  struct btree_t *btree;

  file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Cannot find file %s\n", filename);
    exit(EXIT_FAILURE);
  }
  size = get_filesize(file);
  p = data_start = safe_malloc(size + 1);
  *(p + size) = '\n'; /* In case the file does not have a final line ending */
  if (!fread(p, size, 1, file)) {
    fprintf(stderr, "Cannot read file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  btree = btree_new((int (*)(void *, void *))strcmp);
  while (p < data_start + size) {
    key = p;
    while (*(++p) != ',');
    *p = '\0';
    value = ++p;
    while (!(*(++p) == '\r' || *p == '\n'));
    if (*p == '\r') {
      *p = '\0';
      p++;
    }
    else {
      *p = '\0';
    }
    btree_add(btree, key, value);
    p++;
  }
  return btree;
}


/* Read a string from stdin into buf, with maximum length len. Any newline
   character is always stripped. Returns length of string read. */
int read_query(char *buf, int maxlen) {
  int real_len;
  if (!fgets(buf, maxlen + 1, stdin)) return 0;
  real_len = strlen(buf);
  if (buf[real_len - 1] == '\n') {
    buf[real_len - 1] = '\0';
    real_len--;
  }
  return real_len;
}


/* The write_ functions output found data and comparison counts */
void write_datapair(FILE *stream, char *key, char *value) {
  fprintf(stream, "%s −− > %s\n", key, value);
}

void write_ccount(FILE *stream, char *key, int comparisons) {
  fprintf(stream, "%s −− > %d\n", key, comparisons);
}


/* Get the size of an opened file. Returns file cursor to start of file. */
static int get_filesize(FILE *file) {
  int size;
  fseek(file, 0, SEEK_END);
  size = ftell(file);
  rewind(file);
  return size;
}
