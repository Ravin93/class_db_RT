#include "btree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

BTreeNode* create_node(int id, const char* name) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->id = id;
    strncpy(node->name, name, sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0';
    node->left = node->right = NULL;
    return node;
}

BTreeNode* insert_node(BTreeNode* root, int id, const char* name) {
    if (root == NULL) return create_node(id, name);

    if (id < root->id) {
        root->left = insert_node(root->left, id, name);
    } else if (id > root->id) {
        root->right = insert_node(root->right, id, name);
    }

    return root;
}

BTreeNode* search_node(BTreeNode* root, int id) {
    if (root == NULL || root->id == id) return root;

    if (id < root->id) {
        return search_node(root->left, id);
    } else {
        return search_node(root->right, id);
    }
}

void free_tree(BTreeNode* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

void inorder_traversal(BTreeNode* root) {
    if (root == NULL) return;
    inorder_traversal(root->left);
    printf("ID: %d, Name: %s\n", root->id, root->name);
    inorder_traversal(root->right);
}
