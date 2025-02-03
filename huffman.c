#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/huffman.h"
#include "../include/bt.h"

q *create_element_q(char data,int freq,bt *left,bt *right){
    q *root=(q*)malloc(sizeof(q));
    root->ptr=create_node(data,freq,left,right);
    root->nxt=NULL;
    return root;
}


void countfreq(char *text,int *freq){
    int i=0;
    while(text[i]!='\0'){
        char charactere=text[i];
        freq[charactere]+=1;
        i++;
    }
}

q* insert_q(q* root, q* element) {
     if (!root){
        return element;
    }
    if(root->ptr->frequency<element->ptr->frequency){
        root->nxt=insert_q(root->nxt,element);
        return root;
    }else {
        element->nxt=root;
        return element;
    }
}

q *create_q(int *freq){
    q *root=NULL;
    for (int i=0;i<256;i++){
        if(freq[i]!=0){
            q *element=create_element_q(i,freq[i],NULL,NULL);
            root=insert_q(root,element);
        }
    }
    return root;
}

q* delete_element_q(q* root, char data) {
    if(!root){
        return root;
    }
    if(root->ptr->data==data){
        q *temp=root->nxt;
        free(root);
        return temp;
    }else{
        root->nxt=delete_element_q(root->nxt,data);
        return root;
    }
}

int size_q(q *root){
   if(!root){
    return 0;
   }
   return 1+size_q(root->nxt);
}

bt *createTree(q *root){
    if(!root){
        return NULL;
    }
    if(!root->nxt){
        return root->ptr;
    }
    q *temp=create_element_q('\0',root->ptr->frequency+root->nxt->ptr->frequency,root->ptr,root->nxt->ptr);
    root=delete_element_q(root,root->ptr->data);
    root=delete_element_q(root,root->ptr->data);
    root=insert_q(root,temp);
    return createTree(root);
}

void generate_code(bt *root,char *code[],char *codechar, int length){
    if(!root){
        return;
    }
    if(!root->left && !root->right){
        codechar[length] = '\0';
        code[root->data] = strdup(codechar);
        return;
    }
    codechar[length] = '0';
    generate_code(root->left, code, codechar, length + 1);
    codechar[length] = '1';
    generate_code(root->right, code, codechar, length + 1);
}

char* compress_text(char* text, char* code[]) {
    int size = strlen(text);
    int total_size = 0;
    for (int i = 0; i < size; i++) {
        total_size += strlen(code[(unsigned char)text[i]]);
    }
    char* textcompress = (char*)malloc(total_size + 1); 
    if (!textcompress) {
        return NULL; 
    }
    int index = 0; 
    for (int i = 0; i < size; i++) {
        char* huffmanCode = code[(unsigned char)text[i]];
        for (int j = 0; huffmanCode[j] != '\0'; j++) {
            textcompress[index++] = huffmanCode[j];
        }
    }
    textcompress[index] = '\0';
    return textcompress;
}


char* decompress_text(char* textcompress, bt* root) {
    if (!textcompress) {
        return NULL;
    }
    int size = strlen(textcompress);
    char* text = (char*)malloc(size + 1); 
    if (!text) {
        return NULL; 
    }
    int i = 0; 
    int j = 0; 
    do {
        bt* temp = root;
        do {
            if (textcompress[i] == '0') {
                temp = temp->left; 
            } else {
                temp = temp->right; 
            }
            i++; 
        } while (temp->left && temp->right); 
        text[j] = temp->data;
        j++; 
    } while (textcompress[i] != '\0'); 
    text[j] = '\0';
    return text;
}


char* read_text_file(FILE* readfile) {
    fseek(readfile, 0, SEEK_END);
    long file_size = ftell(readfile);
    fseek(readfile, 0, SEEK_SET);
    char *content = (char *)malloc(file_size + 1);
    if (content == NULL) {
        perror("Memory allocation failed");
        fclose(readfile);
        return NULL;
    }
    fread(content, 1, file_size, readfile);
    content[file_size] = '\0';
    return content;
}

void write_text_file(FILE *writefile,char *text){
    int size =strlen(text);
    fwrite(text,size*sizeof(char),1,writefile);
}

void write_q(FILE *writefile,q *root){
    int size=size_q(root);
    fwrite(&size,sizeof(int),1,writefile);
    q *temp=root;
    while(temp){
        fwrite(&temp->ptr->data,sizeof(char),1,writefile);
        fwrite(&temp->ptr->frequency,sizeof(int),1,writefile);
        temp=temp->nxt;
    }
}

void write_binary_file(FILE* writefile, char* textcompress) {
    if (!writefile || !textcompress) {
        fprintf(stderr, "Erreur : fichier ou texte compressé invalide.\n");
        return;
    }
    int size = strlen(textcompress);
    if (fwrite(&size, sizeof(int), 1, writefile) != 1) {
        perror("Erreur lors de l'écriture de la taille");
        return;
    }
    int rest = size % 8;
    char *padded_text = NULL;
    if (rest) {
        int padded_size = size + (8 - rest);
        padded_text = (char*)malloc(padded_size + 1);
        if (!padded_text) {
            perror("Erreur d'allocation mémoire");
            return;
        }
        strcpy(padded_text, textcompress);
        for (int i = 0; i < 8 - rest; i++) {
            padded_text[size + i] = '0';
        }
        padded_text[padded_size] = '\0';
        textcompress = padded_text; 
    }
    int newsize = strlen(textcompress);
    for (int i = 0; i < newsize / 8; i++) {
        int byte = 0;
        for (int j = 0; j < 8; j++) {
            if (textcompress[i * 8 + j] == '0') {
                byte = byte << 1;
            } else {
                byte = (byte << 1) + 1;
            }
        }
        if (fwrite(&byte, 1, 1, writefile) != 1) {
            perror("Erreur lors de l'écriture des octets");
            if (padded_text) free(padded_text);
            return;
        }
    }
    if (padded_text) free(padded_text); 
}


q* extract_q(FILE* readfile) {
    if (!readfile) {
        fprintf(stderr, "Erreur : fichier invalide.\n");
        return NULL;
    }
    int size;
    if (fread(&size, sizeof(int), 1, readfile) != 1) {
        perror("Erreur lors de la lecture de la taille");
        return NULL;
    }
    char data;
    int frequency;
    if (fread(&data, sizeof(char), 1, readfile) != 1 || fread(&frequency, sizeof(int), 1, readfile) != 1) {
        perror("Erreur lors de la lecture des données");
        return NULL;
    }
    q* root = create_element_q(data, frequency, NULL, NULL);
    if (!root) {
        fprintf(stderr, "Erreur : échec de la création de l'élément.\n");
        return NULL;
    }
    q* temp = root;
    for (int i = 1; i < size; i++) {
        if (fread(&data, sizeof(char), 1, readfile) != 1 || fread(&frequency, sizeof(int), 1, readfile) != 1) {
            perror("Erreur lors de la lecture des données");
            while (root) {
                q* next = root->nxt;
                free(root);
                root = next;
            }
            return NULL;
        }
        q* element = create_element_q(data, frequency, NULL, NULL);
        if (!element) {
            fprintf(stderr, "Erreur : échec de la création de l'élément.\n");
            while (root) {
                q* next = root->nxt;
                free(root);
                root = next;
            }
            return NULL;
        }
        temp->nxt = element;
        temp = temp->nxt;
    }
    return root;
}


char* extract_compress_text(FILE* readfile) {
    if (!readfile) {
        fprintf(stderr, "Erreur : fichier invalide.\n");
        return NULL;
    }
    int real_size;
    if (fread(&real_size, sizeof(int), 1, readfile) != 1) {
        perror("Erreur lors de la lecture de la taille du texte compressé");
        return NULL;
    }
    int size = real_size + (8 - real_size % 8);
    char* compresstext = (char*)malloc(size + 1); 
    if (!compresstext) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }
    int mask = 128;
    for (int i = 0; i < size / 8; i++) {
        int byte;
        if (fread(&byte, 1, 1, readfile) != 1) {
            perror("Erreur lors de la lecture des octets");
            free(compresstext);
            return NULL;
        }
        for (int j = 0; j < 8; j++) {
            if (byte & mask) {
                compresstext[i * 8 + j] = '1';
            } else {
                compresstext[i * 8 + j] = '0';
            }
            byte = byte << 1;
        }
    }
    compresstext[real_size] = '\0';
    return compresstext;
}