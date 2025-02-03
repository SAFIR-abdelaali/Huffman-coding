#ifndef BT_H
#define BT_H


typedef struct bt {
    char data;
    int frequency;
    struct bt * left;
    struct bt * right;
} bt;

bt * create_node(char data,int frequency,bt *left,bt *right);
bt *insert(bt *root,char data,int frequency);
bt *delete_node(bt *root,char data);
bt *search(bt *root,char data);

#endif 