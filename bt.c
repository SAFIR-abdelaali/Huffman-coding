#include <stdio.h>
#include <stdlib.h>
#include "../include/bt.h"
#include "../include/huffman.h"

bt *create_node(char data,int frequency,bt *left,bt *right){
    bt *root=(bt*)malloc(sizeof(bt));
    root->data=data;
    root->frequency=frequency;
    root->left=left;
    root->right=right;
    return root;
}

bt *insert(bt *root, char data,int frequency) {
    if (!root) {
        return create_node(data,frequency,NULL,NULL);
    }
    if (!root->left) {
        root->left = create_node(data,frequency,NULL,NULL);
    } else if (!root->right) {
        root->right = create_node(data,frequency,NULL,NULL);
    } else {
        insert(root->left, data,frequency);
    }
    return root;
}



bt* search(bt* root, char data) {
    if (!root) {
        return root;
    }
    if (root->data == data) {
        return root;
    }
    bt* left = search(root->left, data);
    if (left) {
        return left;
    }
    return search(root->right, data);
}

bt* deleteNode(bt* root, char data) {
    if (!root) {
        return root;
    }
    if (root->data == data) {
        if (root->left) {
            root->data = root->left->data; 
            root->left = deleteNode(root->left, root->data); 
        }
        else if (root->right) {
            root->data = root->right->data; 
            root->right = deleteNode(root->right, root->data); 
        }
        else {
            free(root); 
            return NULL; 
        }
        return root;
    }
    if (search(root->left, data)) {
        root->left = deleteNode(root->left, data);
    }
    else if (search(root->right, data)) {
        root->right = deleteNode(root->right, data);
    }
    return root;
}
