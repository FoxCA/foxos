#include <kernel.h>
/*
 * Create a tree with root = NULL
 * */
tree_t * tree_create() {
    return (tree_t*)kcalloc(sizeof(tree_t), 1);
}

/*
 * Create a tree node with specified value, and a list of 0 children
 * */
treenode_t * treenode_create(void * value) {
    treenode_t * n = kcalloc(sizeof(treenode_t), 1);
    n->value = value;
    n->children = list_create();
    return n;
}

/*
 * Insert a node under subroot
 * */
treenode_t * tree_insert(tree_t * tree, treenode_t * subroot, void * value) {
    // Create a treenode
    treenode_t * treenode = kcalloc(sizeof(treenode_t), 1);
    treenode->children = list_create();
    treenode->value = value;

    // Insert it
    if(!tree->root) {
        tree->root = treenode;
        return treenode;
    }
    list_insert_front(subroot->children, treenode);
    return treenode;
}

treenode_t * tree_find_parent(tree_t * tree, treenode_t * remove_node, int * child_index) {
    // If subroot is the parent
    if(remove_node == tree->root) return NULL;
    return tree_find_parent_recur(tree, remove_node, tree->root, child_index);
}

treenode_t * tree_find_parent_recur(tree_t * tree, treenode_t * remove_node, treenode_t * subroot, int * child_index) {
    int idx;
    if((idx = list_contain(subroot->children, remove_node)) != -1) {
        *child_index = idx;
        return subroot;
    }
    foreach(child, subroot->children) {
        treenode_t * ret = tree_find_parent_recur(tree, remove_node, child->val, child_index);
        if(ret != NULL) {
            return ret;
        }
        // ret is NULL, keep searching.
    }
    return NULL;
}

void tree_remove(tree_t * tree, treenode_t * remove_node) {
    // Search for tree's parent and remove the node from parent's children list
    // If parent is NULL, then just set tree->root to NULL(yeah, I dont care about mem leaks)
    int child_index = -1;
    treenode_t * parent = tree_find_parent(tree, remove_node, &child_index);
    // Do treenode remove in here:
    if(parent != NULL) {
        treenode_t * freethis = list_remove_by_index(parent->children, child_index);
        // Free tree node here
        kfree(freethis);
    }
}

void tree2list_recur(treenode_t * subroot, list_t * list) {
    if(subroot== NULL)
        return;
    foreach(child, subroot->children) {
        treenode_t * curr_treenode = (treenode_t*)child->val;
        void * curr_val = curr_treenode->value;
        list_insert_back(list, curr_val);
        tree2list_recur(child->val, list);
    }
}

void tree2list(tree_t * tree, list_t * list) {
    tree2list_recur(tree->root, list);
}



void tree2array(tree_t * tree, void ** array, int * size) {
    tree2array_recur(tree->root, array, size);
}

void tree2array_recur(treenode_t * subroot, void ** array, int * size) {
    if(subroot== NULL)
        return;
    void * curr_val = (void*)subroot->value;
    array[*size] = curr_val;
    *size = *size + 1;
    foreach(child, subroot->children) {
        tree2array_recur(child->val, array, size);
    }
}
