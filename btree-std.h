/*
   btree-std.h: Implementation of binary tree with 2 branches per node.
   
   This file is part of COMP20003 Assignment 1,
   Luke Ceddia, <lceddia@student.unimelb.edu.au>
   Student ID: 834076

   This file contains definitons specific to this implementation of a binary
   tree, and should not be included by other modules. See btree.h for public
   functions relating to the binary tree.
 */

struct node_t {
  void *key;
  void *data;
  struct node_t *left;
  struct node_t *right;
};

static void recursive_add(struct node_t *new,
                          struct node_t *parent,
                          int (*cmp)(void *arg1, void *arg2));

static struct node_t *recursive_find(void *key,
                                     struct node_t *node,
                                     int (*cmp)(void *arg1, void *arg2),
                                     int *comparisons);

