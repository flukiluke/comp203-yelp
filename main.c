#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "btree.h"
#include "safealloc.h"
#include "io.h"

#include <time.h>

#define MAX_QUERY_SIZE 64

static void show_help(char *argv0);

int main(int argc, char **argv) {
  struct btree_t *btree;
  FILE *data_out, *diag_out; /* Data and diagnostics (comparison count) */
  char query[MAX_QUERY_SIZE];
  char *result;

  if (argc != 3) {
    show_help(argv[0]);
    exit(EXIT_FAILURE);
  }

  /* Initialise streams and database */
  data_out = fopen(argv[2], "w");
  if (!data_out) {
    fprintf(stderr, "Cannot open %s\n", argv[2]);
    exit(EXIT_FAILURE);
  }
  diag_out = stdout;

  clock_t c1;
  c1 = clock();
  btree = read_db(argv[1]);
  printf("Load time: %f\n", (float)(clock() - c1) / CLOCKS_PER_SEC);
  /* Here we go! */
  while (read_query(query, MAX_QUERY_SIZE)) {
    c1 = clock();
    result = btree_find(btree, query);
    printf("Initial search time: %f\n", (float)(clock() - c1) / CLOCKS_PER_SEC);
    if (result) {
      write_datapair(data_out, query, result);
      c1 = clock();
      while ((result = btree_find_next(btree))) {
    printf("Next search time: %f\n", (float)(clock() - c1) / CLOCKS_PER_SEC);        write_datapair(data_out, query, result);
    c1 = clock();
      }
    }
    else {
      write_datapair(data_out, query, "NOTFOUND");
    }
    write_ccount(diag_out, query, btree->comparisons);
  }

  /* No need to free() btree or close files */
  return 0;
}


/* Print some basic description/help message. Also describes the underlying
   data structure compiled in. */
void show_help(char *argv0) {
  printf("UNIX Yelp - A COMP20003 project\n"
         "Usage: %s [data file] [output file]\n\n"
         "Where <data file> is the file to read entries from,\n"
         "and <output file> is the file to write results to.\n"
         , argv0);
}
