#include "malloc.h"

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

t_meta_data *find_previous_page(t_meta_data *ptr)
{
    t_meta_data *tmp;

    tmp = g_data.head;
    while (tmp)
    {
        if (tmp->page == ptr)
        {
            return tmp;
        }
        tmp = tmp->page;
    }
    return NULL;
}

int check_if_free_page(t_meta_data *ptr)
{
    size_t total_size;
    t_meta_data *previous_page;

    total_size = ptr->size + sizeof(t_meta_data);
    ft_printf("{cyan}%lu %d %d{reset}\n\n", total_size, _N, _M);
    if (total_size == _N || total_size >= _M / 100)
    {
        previous_page = find_previous_page(ptr);
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
            else
            {
                g_data.head = NULL;
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
                g_data.tail = ptr->prev;
            }
        }
        printf("banane previous page %p\n", previous_page);
        if (previous_page)
        {
            previous_page->page = ptr->page;
            if (!previous_page->page)
            {
                g_data.page_tail = previous_page;
            }
        }

        printf("banane2\n");
        if ((munmap(ptr, total_size) == -1))
            return 0; //error
    }
    return (1);
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

t_meta_data *free_space_next(t_meta_data *ptr)
{
    toggle_flag(ptr, 1, FREE);
    if (ptr->next && get_flag_value(ptr->next, FREE))
    {
        ptr->size += sizeof(t_meta_data) + ptr->next->size;
        ptr->next = ptr->next->next;
        if (!ptr->next)
        {
            g_data.tail = ptr;
        }
        else
        {
            ptr->next->prev = ptr;
        }
    }
    return ptr;
}

t_meta_data *free_space_prev(t_meta_data *ptr)
{
    toggle_flag(ptr, 1, FREE);
    if (ptr->prev && get_flag_value(ptr->prev, FREE))
    {
        if (get_type(ptr) != get_type(ptr->prev))
            return ptr;
        ptr->prev->size += sizeof(t_meta_data) + ptr->size;
        ptr->prev->next = ptr->next;
        ptr = ptr->prev;
        if (ptr->next)
        {
            ptr->next->prev = ptr;
        }
    }
    return ptr;
}

void ft_free(void *ptr)
{
    t_meta_data *temp_ptr;

    pthread_mutex_lock(&g_mutex);
    if (!(temp_ptr = find_ptr_in_stack(ptr)))
    {
        pthread_mutex_unlock(&g_mutex);
        return;
    }
    temp_ptr = free_space_next(temp_ptr);
    temp_ptr = free_space_prev(temp_ptr);
    check_if_free_page(temp_ptr);
    pthread_mutex_unlock(&g_mutex);
}