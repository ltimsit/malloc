#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#define _N 4096
#define _M 409600

typedef struct s_meta_data {
    void *curr_ptr;
    size_t size;
    struct s_meta_data *next;
    int free;

} t_meta_data;

t_meta_data *g_data = NULL;

void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);