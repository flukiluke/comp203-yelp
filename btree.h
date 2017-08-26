/*
   btree.h: Public interface to binary tree implementations
   
   This file is part of COMP20003 Assignment 1,
   Luke Ceddia, <lceddia@student.unimelb.edu.au>
   Student ID: 834076

   These definitions are common to all implementations of a binary tree, and
   are the only functions that should be used to work with a tree.
 */


/* A general struct for describing a btree overall */
struct btree_t {
  void *root;
  void *current; /* Used to hold the current search position */
  enum {BTREE_NODE, BTREE_LINK} cur_type; /* The type of structure of above */
  int comparisons; /* Number comparisons made against the current search key */
  int (*cmp)(void *arg1, void *arg2); /* comparison function to use */
};


/* Initialise btree, using cmp as the comparison function */
struct btree_t *btree_new(int (*cmp)(void *arg1, void *arg2));

/* Add key, value pair to binary tree */
void btree_add(struct btree_t *btree, void *key, void *data);

/* Search for key in the binary tree, and return a pointer to data.
   Returns NULL if not found */
void *btree_find(struct btree_t *btree, void *key);

/* Return next result found; called after btree_find() to get full results.
   Returns NULL if there are no more mathces left */
void *btree_find_next(struct btree_t *btree);
