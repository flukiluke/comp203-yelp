/*
   btree-std.c: Implementation of binary tree with 2 branches per node.
   
   This file is part of COMP20003 Assignment 1,
   Luke Ceddia, <lceddia@student.unimelb.edu.au>
   Student ID: 834076

   In this implementation, each node has 0, 1 or 2 branches. Data less than or
   equal are stored on the left branch, data greater than are stored on the 
   right branch.
   
   See btree.h for a description of the public interface.
 */

#include "safealloc.h" /* wrapper of malloc() and friends with checking */
#include "btree.h" /* Include header common to all btree implementations */
#include "btree-std.h" /* Definitions private to this implementation */


/* Argument is a function pointer to the comparison function to be used
   for this tree */
struct btree_t *btree_new(int (*cmp)(void *arg1, void *arg2)) {
  struct btree_t *btree;
  btree = safe_malloc(sizeof(struct btree_t));
  btree->root = NULL;
  btree->cmp = cmp;
  return btree;
}


void btree_add(struct btree_t *btree, void *key, void *data) {
  struct node_t *node;

  /* Initialise node */
  node = safe_malloc(sizeof(struct node_t));
  node->key = key;
  node->data = data;
  node->left = node->right = NULL;

  if (!btree->root) { /* Trivial case: empty tree */
    btree->root = node;
    return;
  }

  recursive_add(node, btree->root, btree->cmp);
}


/* Search for the entire tree for key, returning the first instance found.
   Also stores result in btree->current for btree_find_next() to make use of.
   Returns NULL if not found. */
void *btree_find(struct btree_t *btree, void *key) {
  btree->comparisons = 0;
  btree->current = recursive_find(key, btree->root,
                                  btree->cmp, &(btree->comparisons));
  return btree->current ? ((struct node_t *)btree->current)->data : NULL;
}  


/* Returns the next instance of a key in the tree, after calling btree_find().
   Returns NULL when there are no more results remaining. */
void *btree_find_next(struct btree_t *btree) {
  struct node_t *current = btree->current;
  if (!current) return NULL;
  btree->current = recursive_find(current->key, current->left,
                                  btree->cmp, &(btree->comparisons));
  return btree->current ? ((struct node_t *)btree->current)->data : NULL;
}


/* Given a node 'parent' in the tree and a node 'new' to be inserted, check
   if new should be on the left or right branch of parent, then either
   add it there or recursively call to add it to the node present.
 */
static void recursive_add(struct node_t *new,
                          struct node_t *parent,
                          int (*cmp)(void *arg1, void *arg2)) {
  struct node_t **branch;
  if (cmp(new->key, parent->key) <= 0) { /* Equal keys go on left branch */
    branch = &parent->left;
  }
  else {
    branch = &parent->right;
  }
  if (*branch) {
    recursive_add(new, *branch, cmp);
  }
  else {
    *branch = new;
    return;
  }
}


/* Recursively compare the key against each node in the tree, going down
   until we hit a match, or reach the end. */
static struct node_t *recursive_find(void *key,
                                     struct node_t *node,
                                     int (*cmp)(void *arg1, void *arg2),
                                     int *comparisons) {
  /* Recursion base case - end of tree with no match */
  if (!node) return NULL;
  
  int cmp_result = cmp(key, node->key);
  (*comparisons)++;
  if (cmp_result == 0) { /* We found a match */
    return node;
  }
  else if (cmp_result < 0) { 
    return recursive_find(key, node->left, cmp, comparisons);
  }
  else {
    return recursive_find(key, node->right, cmp, comparisons);
  } 
} 
