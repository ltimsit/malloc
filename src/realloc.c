#include "malloc.h"

void copy_data_here(t_meta_data *new_ptr, void *ptr, size_t old_size, size_t new_size)
{
    if (new_ptr == ptr)
    {
        return;
    }
    printf("test\n");
    memcpy(new_ptr, ptr, old_size < new_size ? old_size : new_size);
    printf("test2\n");
}

void *ft_realloc(void *ptr, size_t size)
{
    t_meta_data *temp_ptr;
    t_meta_data *temp2_ptr;
    size_t old_size;
    int type;

    pthread_mutex_lock(&g_mutex);
    if (!(temp_ptr = find_ptr_in_stack(ptr)))
    {
        pthread_mutex_unlock(&g_mutex);
        return NULL;
    }
    size = ((size - 1) >> 2 << 2) + 4;
    old_size = temp_ptr->size;
    temp_ptr = free_space_next(temp_ptr);


    type = page_type_for_size(size + sizeof(t_meta_data));
    // return temp_ptr + sizeof(t_meta_data);

    if (!get_flag_value(temp_ptr, type) || temp_ptr->size < size)
    {
        ft_printf("{red}test %lu{reset}", temp_ptr->size);
        temp2_ptr = temp_ptr;
        temp_ptr = ft_malloc(size);
        copy_data_here(temp_ptr, ptr, old_size, size);
        temp2_ptr = free_space_prev(temp2_ptr);
        check_if_free_page(temp2_ptr);
    }
    else
    {
        ft_printf("{red}test2 %lu{reset}", temp_ptr->size);
        insert_block(temp_ptr, size, type);
        temp_ptr += 1;
        copy_data_here(temp_ptr, ptr, old_size, size);
    }
    pthread_mutex_unlock(&g_mutex);
    return temp_ptr;
}