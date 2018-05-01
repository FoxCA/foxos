#ifndef TREE_H
#define TREE_H
#include <list.h>

typedef struct treenode {
    list_t * children;
    void * value;
}treenode_t;

typedef struct tree {
    treenode_t * root;
}tree_t;

tree_t * tree_create();

treenode_t * treenode_create(void * value);

treenode_t * tree_insert(tree_t * tree, treenode_t * subroot, void * value);

treenode_t * tree_find_parent(tree_t * tree, treenode_t * remove_node, int * child_index);

treenode_t * tree_find_parent_recur(tree_t * tree, treenode_t * remove_node, treenode_t * subroot, int * child_index);

void tree_remove(tree_t * tree, treenode_t * remove_node);

void tree2list_recur(treenode_t * subroot, list_t * list);

void tree2list(tree_t * tree, list_t * list);

void tree2array(tree_t * tree, void ** array, int * size);

void tree2array_recur(treenode_t * subroot, void ** array, int * size);

#endif