/*
   btree-llist.c: Implementation of binary tree using a linked list.
   
   This file is part of COMP20003 Assignment 1,
   Luke Ceddia, <lceddia@student.unimelb.edu.au>
   Student ID: 834076

   In this implementation, left and right branches are as usual, but
   duplicates are stored in a linked list originating from the first instance
   of a key in the tree. Thus, finding a key gives a linked list to all other
   instances of that key.
   
   See btree.h for a description of the public interface.
 */

#include "safealloc.h" /* wrapper of malloc() and friends with checking */
#include "btree.h" /* Include header common to all btree implementations */
#include "btree-llist.h" /* Definitions private to this implementation */


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
  if (!btree->root) { /* Trivial case; empty tree */
    btree->root = create_node(key, data);
    return;
  }
  recursive_add(key, data, btree->root, btree->cmp);
}


/* Search for the entire tree for key, returning the first instance found.
   Also stores result in btree->current for btree_find_next() to make use of.
   Returns NULL if not found. */
void *btree_find(struct btree_t *btree, void *key) {
  btree->comparisons = 0;
  btree->current = recursive_find(key, btree->root,
                                  btree->cmp, &(btree->comparisons));
  btree->cur_type = BTREE_NODE;
  return btree->current ? ((struct node_t *)btree->current)->data : NULL;
}  


/* Returns the next instance of a key in the tree, after calling btree_find().
   Returns NULL when there are no more results remaining. */
void *btree_find_next(struct btree_t *btree) {
  struct node_t *cur_node = btree->current;
  struct llist_element_t *cur_link = btree->current;
  
  if (!btree->current) return NULL;
  if (btree->cur_type == BTREE_NODE) {
    btree->current = cur_node->llist;
    btree->cur_type = BTREE_LINK;
  }
  else {
    btree->current = cur_link->next;
  }
  return btree->current ? ((struct llist_element_t *)btree->current)->data : NULL;  
}


/* Given a node 'parent' in the tree and a key, data pair, check whether
   it should be on the left, right or the linked list of the parent, and
   either recursively add on the subbranch or actually add it in.
 */
static void recursive_add(void *key, void *data, struct node_t *parent,
                          int (*cmp)(void *arg1, void *arg2)) {
  struct node_t **branch;
  int res;
  res = cmp(key, parent->key);
  if (!res) { /* Duplicate */
    llist_append(parent, data);
    return;
  }
  if (res < 0) {
    branch = &parent->left;
  }
  else {
    branch = &parent->right;
  }
  if (*branch) {
    recursive_add(key, data, *branch, cmp);
  }
  else {
    *branch = create_node(key, data);
    return;
  }    
}


/* Create and initialise a tree node */
static struct node_t *create_node(void *key, void *data) {
  struct node_t *node;
  node = safe_malloc(sizeof(struct node_t));
  node->left = node->right = NULL;
  node->llist = NULL;
  node->key = key;
  node->data = data;
  return node;
}


/* Add an element to the linked list of a given node */
static void llist_append(struct node_t *parent, void *data) {
  struct llist_element_t *link;
  link = safe_malloc(sizeof(struct llist_element_t));
  link->next = parent->llist;
  parent->llist = link;
  link->data = data;
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
