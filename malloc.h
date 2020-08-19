#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ft_printf.h"


#define _N 4096
#define _M 409600

<<<<<<< HEAD
#define PTR_ERROR 0
#define REALLOC_FAILURE 1
#define REALLOC_SUCCESS 2

#define FREE 1
#define TINY_BLOCK 2
#define SMALL_BLOCK 4
#define LARGE_BLOCK 8

=======
>>>>>>> edcfc56a2c6470b1d533bee4f256480f8d44be24
typedef struct s_meta_data {
    struct s_meta_data *prev;
    size_t size;
    struct s_meta_data *next;
    struct s_meta_data *page;
    int flag;

} t_meta_data;

typedef struct s_list_infos {
    t_meta_data *head;
    t_meta_data *tail;
    t_meta_data *page_tail;
} t_list_info;

t_list_info g_data = {(void*)0, (void*)0, (void*)0};

void *ft_malloc(size_t size);
void ft_free(void *ptr);
void *ft_realloc(void *ptr, size_t size);


t_meta_data *free_space(t_meta_data *ptr);



/* projection */

void add_meta_data(t_meta_data *projection, size_t size, int type);
void *get_new_projection(size_t size, int type);
void add_projection(t_meta_data *projection);

/* list */
void insert_block(t_meta_data *ptr, size_t size, int type);
t_meta_data *find_space(int type, int size);

/* utils */
int get_flag_value(t_meta_data *ptr, int flag);
int page_size_for_type(int page_type, size_t size);
int page_type_for_size(size_t size);
int toggle_flag(t_meta_data *ptr, int value, int flag);

/* free */
t_meta_data *free_space(t_meta_data *ptr);
int check_if_free_page(t_meta_data *ptr);
void relink_prev_page(t_meta_data *ptr);

/* realloc */
void copy_data_here(t_meta_data *new_ptr, t_meta_data *ptr, size_t old_size);

<<<<<<< HEAD
=======
} t_meta_data;

t_meta_data *g_data = NULL;

void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
>>>>>>> edcfc56a2c6470b1d533bee4f256480f8d44be24
