#include "malloc.h"

int page_type_for_size(size_t size)
{
    if (size < _N / 100)
    {
        return TINY_BLOCK;
    }
    else if (size < _M / 100)
    {
        return SMALL_BLOCK;
    }
    else
    {
        return LARGE_BLOCK;
    }
}

int get_flag_value(t_meta_data *ptr, int flag)
{
    return ptr->flag & flag;
}

int page_size_for_type(int page_type, size_t size)
{
    if (page_type == TINY_BLOCK)
    {
        return _N;
    }
    else if (page_type == SMALL_BLOCK)
    {
        return _M;
    }
    else
    {
        return size;
    }
}

/* Parcours la liste pour trouver le ptr */
void *find_ptr_in_stack(void *ptr)
{
    t_meta_data *temp_ptr;

    temp_ptr = g_data.head;
    while (temp_ptr)
    {
        if (!get_flag_value(temp_ptr, FREE) && temp_ptr + 1 == ptr)
        {
            return temp_ptr;
        }
        temp_ptr = temp_ptr->next;
    }
    return NULL;
}

int toggle_flag(t_meta_data *ptr, int value, int flag)
{
    if (value)
    {
        ptr->flag |= flag;
    }
    else
    {
        ptr->flag &= ~flag;
    }
}