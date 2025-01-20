#ifndef BTREE_H
#define BTREE_H

typedef struct BTreeNode {
    int id;
    char name[32];
    struct BTreeNode* left;
    struct BTreeNode* right;
} BTreeNode;

BTreeNode* create_node(int id, const char* name);
BTreeNode* insert_node(BTreeNode* root, int id, const char* name);
BTreeNode* search_node(BTreeNode* root, int id);
void free_tree(BTreeNode* root);
void inorder_traversal(BTreeNode* root);

#endif
