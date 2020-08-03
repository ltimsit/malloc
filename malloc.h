#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


typedef struct s_meta_data {
    void *curr_ptr;
    size_t size;
    struct s_meta_data *next;
    int free;

} t_meta_data;