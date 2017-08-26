/*
   io.h: Define various functions for communicating with the outside world.
   
   This file is part of COMP20003 Assignment 1,
   Luke Ceddia, <lceddia@student.unimelb.edu.au>
   Student ID: 834076

 */

struct btree_t *read_db(char *filename);
int read_query(char *buf, int maxlen);
void write_datapair(FILE *stream, char *key, char *value);
void write_ccount(FILE *stream, char *key, int comparisons);