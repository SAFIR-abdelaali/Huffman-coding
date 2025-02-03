#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/bt.h"
#include "../include/huffman.h"
#include "bt.c"
#include "huffman.c"

int main(){

    int choice;
    char txtpath[256], binpath[256];
    char *compress = NULL, *text = NULL;
    FILE *readfile,*writefile;
    q *q_root=NULL;
    bt *btroot=NULL;


    while(1){
        printf("\n1. Compress a file \n2. Decompress a file \n3. Exit \nChoose a number (Compress/Decompress): ");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                printf("Enter the path of your text file: \n");
                scanf("%s",txtpath);
                printf("Enter where to put the compressed data:\n");
                scanf("%s",binpath);
                readfile=fopen(txtpath,"r");
                if(!readfile){
                    printf("failed to open %s",readfile);
                    break;
                }
                writefile=fopen(binpath,"wb");
                if(!writefile){
                    printf("failed to open %s",writefile);
                    break;
                }
                char codechar[20];
                int freq[256]={0};
                char *code[256]={NULL};
                text=read_text_file(readfile);
                countfreq(text,freq);
                q_root=create_q(freq);
                write_q(writefile,q_root);
                btroot=createTree(q_root);
                generate_code(btroot,code,codechar,0);
                compress=compress_text(text,code);
                write_binary_file(writefile,compress);
                fclose(readfile);
                fclose(writefile);
                free(text);
                free(compress);
                break;
            case 2:
                printf("enter the path of your compressed file:\n");
                scanf("%s",&binpath);
                printf("enter where to put the new decompressed text:\n");
                scanf("%s",&txtpath);
                readfile=fopen(binpath,"rb");
                if(!readfile){
                    printf("Failed to open %s",readfile);
                    break;
                }
                writefile=fopen(txtpath,"w");
                if(!writefile){
                    printf("Failed to open %s",writefile);
                    break;
                }
                q_root=extract_q(readfile);
                btroot=createTree(q_root);
                compress=extract_compress_text(readfile);
                text=decompress_text(compress,btroot);
                fwrite(text,strlen(text),1,writefile);
                fclose(readfile);
                fclose(writefile);
                free(text);
                free(compress);
                break;
            case 3:
                return 0;
            default:
                printf("Invalide, please retry.\n");
        }
    }
    return 0;
}