#include "malloc.c"

/* find previous page ptr and change its next */
void relink_prev_page(t_meta_data *ptr)
{
    t_meta_data *tmp;

    tmp = g_data.head;

    while (tmp)
    {
        if (tmp->page == ptr)
        {
            tmp->page = ptr->page;
            if (tmp->page == NULL)
            {
                g_data.page_tail = tmp;
            }
            return;
        }
        tmp = tmp->page;
    }
}

int check_if_free_page(t_meta_data *ptr)
{
    size_t total_size;

    total_size = ptr->size + sizeof(t_meta_data);
    if (total_size == _N || total_size >= _M)
    {
        if (ptr->prev)
        {
            ptr->prev->next = ptr->next;
        }
        else
        {
            if (ptr->next)
            {
                ptr->next->prev = NULL;
                g_data.head = ptr->next;
            }
        }
        if (ptr->next)
        {
            ptr->next->prev = ptr->prev;
        }
        else
        {
            if (ptr->prev)
            {
                ptr->prev->next = NULL;
                g_data.tail = ptr;
            }
        }
        if ((munmap(ptr, total_size) == -1))
            return 0; //error
    }
}

t_meta_data *free_space(t_meta_data *ptr)
{
    size_t total_size;
    t_meta_data *tmp;
    total_size = ptr->size;

    tmp = ptr;
    toggle_flag(ptr, 1, FREE);
    if (ptr->prev && get_flag_value(ptr->prev, FREE))
    {
        total_size += sizeof(t_meta_data) + ptr->prev->size;
        tmp = ptr->prev;
    }
    if (ptr->next && get_flag_value(ptr->next, FREE))
    {
        total_size += sizeof(t_meta_data) + ptr->next->size;
        tmp->next = ptr->next->next;
        if (!tmp->next)
        {
            g_data.tail = tmp;
        }
    }
    else
    {
        tmp->next = ptr->next;
    }
    tmp->size = total_size;

    if (tmp->next)
    {
        tmp->next->prev = tmp;
    }
    return tmp;
}

void ft_free(void *ptr)
{
    t_meta_data *temp_ptr;

    if (!(temp_ptr = find_ptr_in_stack(ptr)))
        return;
    temp_ptr = free_space(temp_ptr);
    check_if_free_page(temp_ptr);
}