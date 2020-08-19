#include "malloc.h"

void copy_data_here(t_meta_data *new_ptr, t_meta_data *ptr, size_t old_size)
{
    if (new_ptr == ptr - 1)
    {
        return;
    }
    memcpy(new_ptr + 1, ptr, old_size);
}

void *ft_realloc(void *ptr, size_t size)
{
    t_meta_data *temp_ptr;
    size_t old_size;

    if (!(temp_ptr = find_ptr_in_stack(ptr)))
        return NULL;
    old_size = temp_ptr->size;
    temp_ptr = free_space(temp_ptr);

    if (!get_flag_value(temp_ptr, page_type_for_size(size + sizeof(t_meta_data))) || temp_ptr->size < size)
    {
    ft_printf("{red}%lu{reset}", temp_ptr->size);
        check_if_free_page(temp_ptr);
        temp_ptr = malloc(size);
    }
    else
    {
    ft_printf("{red}%lu{reset}", temp_ptr->size);
        toggle_flag(temp_ptr, 0, FREE);
        temp_ptr->size = size;
    }
    copy_data_here(temp_ptr, ptr, old_size);
    return temp_ptr + sizeof(t_meta_data);
}