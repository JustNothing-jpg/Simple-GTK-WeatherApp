#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FNV_32_PRIME 32

int fnv_hash(char *key,int size){
    int hval = 0;
    for(int i = 0;i<strlen(key);i++){
        hval ^= key[i];
        hval *= FNV_32_PRIME;
    }
    return hval % size;
}

void print_table(char *arr[][100],int size,int sec_size){
    for(int i = 0; i < size;i++){
        for(int j = 0;j<sec_size;j++){
            if(arr[i][j] != NULL)
                if(j == 0)
                    printf("{%s: ",arr[i][j]);
                else
                    printf(" %s, ",arr[i][j]);
        }
    }
    printf("\n");
}

int main(void){
    char *arr[100][100] = {NULL};
    arr[fnv_hash("Me",100)][0] = "Me";
    arr[fnv_hash("Me",100)][fnv_hash("Abulkhair",100)] = "Abulkhair";
    arr[fnv_hash("Me",100)][fnv_hash("Tredd",100)] = "Tredd";
    arr[fnv_hash("Abulkhair",100)][0] = "Abulkhair";
    arr[fnv_hash("Abulkhair",100)][fnv_hash("Me",100)] = "Me";
    arr[fnv_hash("Abulkhair",100)][fnv_hash("Tredd",100)] = "Tredd";
    print_table(arr,100,100);
    return 0;
}
