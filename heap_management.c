#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> 

#define HEAP_SIZE 10000
#define MAX_DEGREE 10
char heap[HEAP_SIZE];  

typedef struct meta_block {     
    int size_of_memory;     
    int free;     
    struct meta_block *next;     
    int mark;  
} meta_data;  

meta_data *hdr = (void *)heap;  

void Initialize() {     
    hdr->size_of_memory = HEAP_SIZE - sizeof(struct meta_block);     
    hdr->free = 1;     
    hdr->next = NULL;     
    hdr->mark = 0;  
}  

void *alloc(int size_of_data) {     
    meta_data *curr;     
    void *result;     
    if (hdr->size_of_memory == 0) {         
        Initialize();     
    }     
    if (size_of_data <= 0 || size_of_data > HEAP_SIZE) {
        printf("Invalid allocation size\n");
        return NULL;
    }

    curr = hdr;     
    while (((curr->size_of_memory < size_of_data) || (curr->free == 0)) && (curr->next != NULL)) {         
        curr = curr->next;         
    }     
    if (curr->size_of_memory == size_of_data) {         
        curr->free = 0;         
        curr->mark = 0;         
        result = (void *)(++curr);     
    } else if (curr->size_of_memory > (size_of_data + sizeof(meta_data))) {         
        meta_data *Next = (void *)((char *)curr + size_of_data + sizeof(meta_data));          
        Next->size_of_memory = curr->size_of_memory - size_of_data - sizeof(meta_data);         
        Next->free = 1;         
        Next->next = curr->next;          
        Next->mark = 0;         
        curr->size_of_memory = size_of_data;         
        curr->free = 0;         
        curr->mark = 0;         
        curr->next = Next;         
        result = (void *)(++curr);     
    } else {         
        result = NULL;         
        printf("out of memory\n");     
    }     
    return result; 
}  

void merge() {     
    meta_data *curr, *Next;     
    curr = hdr;     
    Next = curr->next;     
    while (Next != NULL) {         
        if (curr->free == 1 && Next->free == 1) {             
            curr->size_of_memory += Next->size_of_memory + sizeof(meta_data);             
            curr->next = Next->next;             
            Next = curr->next; 
        } else {             
            curr = Next;             
            Next = Next->next;         
        }     
    } 
}  

void freee(void *ptr) {     
    if (ptr == NULL) {
        printf("Attempt to free NULL pointer\n");
        return;
    }

    if (((void *)heap <= ptr) && (ptr <= (void *)(heap + HEAP_SIZE))) {         
        meta_data *curr = (meta_data *)ptr;         
        --curr;         
        if (curr->free == 1) {
            printf("Double free detected\n");
            return;
        }
        curr->free = 1;         
        curr->mark = 0;         
        merge();     
    } else {         
        printf("freee called on unallocated memory\n");     
    } 
}  

void showHeap() {     
    meta_data *ptr;     
    ptr = hdr;     
    int block_count = 0;
    size_t total_free = 0;
    size_t total_used = 0;

    while (ptr != NULL) {         
        if (ptr->free == 0) {             
            printf("Block %d: ALLOCATED\n", block_count);             
            printf("Size: %d bytes\n", ptr->size_of_memory);
            total_used += ptr->size_of_memory;
        } else {             
            printf("Block %d: FREE\n", block_count);             
            printf("Size: %d bytes\n", ptr->size_of_memory);
            total_free += ptr->size_of_memory;
        }         
        ptr = ptr->next;
        block_count++;     
    }
    
    printf("\nTotal blocks: %d\n", block_count);
    printf("Total free memory: %zu bytes\n", total_free);
    printf("Total used memory: %zu bytes\n", total_used);
}  

int main() {    
    srand(time(NULL));
    
    printf("\n----Heap simulation initialized----\n\n");    
    Initialize();  

    int *p, *q, *r;  
    
    p = alloc(rand() % 1000 + 1);  
    showHeap();     
    printf("\n");     
    q = alloc(rand() % 1000 + 1);     
    showHeap();     
    printf("\n");     
    r = alloc(rand() % 1000 + 1);     
    showHeap();     
    printf("\n");     

    freee(p);     
    showHeap();     
    printf("\n");     
    freee(q);     
    showHeap();     
    printf("\n");     
    freee(r);     
    showHeap();     
    printf("\n");    

    printf("\n----Memory management completed----\n");    
    return 0; 
}
