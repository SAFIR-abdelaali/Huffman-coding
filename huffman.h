#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "./bt.h"

typedef struct q {
    bt *ptr;
    struct q *nxt;
} q;


q *create_element_q(char data,int freq,bt *left,bt *right);
void countfreq(char *text,int *freq);
q *insert_q(q *root, q *element);
int size_q(q *root);
q *create_q(int *freq);
q *delete_element_q(q *root, char data);
bt *create_tree(q *root);
void generate_code(bt *root,char *code[],char *codechar, int length);
char *compress_text(char *text,char *code[]);
char *decompress_text(char *textcompress,bt *root);
char *read_txt_file(FILE *readfile);
void write_txt_file(FILE *writefile,char *text);
void write_queue(FILE *writefile,q *root);
void write_binary_file(FILE *writefile,char *textcompress);
q *extract_queue(FILE *readfile);
char *extract_compress_text(FILE *readfile);

#endif 