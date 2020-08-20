#include "malloc.h"
#include "../libft/libft.h"

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

int get_type(t_meta_data *ptr)
{
    if (ptr->flag & 2)
        return TINY_BLOCK;
    if (ptr->flag & 4)
        return SMALL_BLOCK;
    if (ptr->flag & 8)
        return LARGE_BLOCK;
    return 0;
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

void toggle_flag(t_meta_data *ptr, int value, int flag)
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

void get_type_name(t_meta_data *ptr, char **type, char *name, int *name_val)
{
    if (get_flag_value(ptr, TINY_BLOCK))
    {
        *type = "Tiny Block";
        name[0] = 'T';
        name[1] = '\0';
        name_val[0] += 1;
        ft_strcat(name, ft_itoa(name_val[0]));
    }
    else if (get_flag_value(ptr, SMALL_BLOCK))
    {
        *type = "Small Block";
        name[0] = 'S';
        name[1] = '\0';
        name_val[1] += 1;
        ft_strcat(name, ft_itoa(name_val[1]));
    }
    else
    {
        *type = "Big Block";
        name[0] = 'B';
        name[1] = '\0';
        name_val[2] += 1;
        ft_strcat(name, ft_itoa(name_val[2]));
    }
}

void show_data(t_meta_data *ptr, char *name, char *type)
{
        ft_putstrclr("Nom : ", "yellow");
        ft_putstrclr(name, "yellow");
        ft_putstr("\n");

        ft_putstrclr("Adresse : ", "red");
        ft_putstr("0x");
        ft_putnbr_base((long)ptr, 16);
        ft_putstr("\n");

        ft_putstrclr("Adresse data : ", "red");
        ft_putstr("0x");
        ft_putnbr_base((long)(ptr + 1), 16);
        ft_putstr("\n");

        ft_putstrclr("Type : ", "blue");
        ft_putstrclr(type, "blue");
        ft_putstr("\n");

        ft_putstrclr("Size : ", "blue");
        ft_putnbr(ptr->size);
        ft_putstr("\n");

        ft_putstrclr("Free : ", "blue");
        ft_putnbr(ptr->flag & 1);
        ft_putstr("\n");

        ft_putstrclr("Data : ", "magenta");
        ft_putstrclr((char *)(ptr + 1), "magenta");
        ft_putstr("\n");

        ft_putstrclr("Prev : ", "cyan");
        ft_putstr("0x");
        ft_putnbr_base((long)ptr->prev, 16);
        ft_putstr("\n");

        ft_putstrclr("Next : ", "cyan");
        ft_putstr("0x");
        ft_putnbr_base((long)ptr->next, 16);
        ft_putstr("\n");
        ft_putstr("\n");
}

void show_alloc_mem()
{
    int name_val[3];
    char name[128];
    char *type;
    t_meta_data *ptr;
    t_meta_data *next_page;

    name_val[0] = 0;
    name_val[1] = 0;
    name_val[2] = 0;

    ptr = g_data.head;
    while (ptr)
    {
        ft_bzero(name, 127);
        get_type_name(ptr, &type, name, name_val);
        if (ptr == g_data.head || ptr == next_page)
        {
            next_page = ptr->page;
            ft_putstrclr("Début de page : ", "green");
            ft_putstrclr(type, "green");
            ft_putstr("\n");
        }
        show_data(ptr, name, type);
        ptr = ptr->next;
    }
}

void print_list()
{
    t_meta_data *ptr;
    char *type;
    char name[128];
    int nb_t;
    int nb_s;
    int nb_b;
    int i;

    show_alloc_mem();
    return;

    i = 0;
    nb_t = 0;
    nb_s = 0;
    nb_b = 0;
    t_meta_data *next_page;

    ptr = g_data.head;
    printf("\n\n$$$$$$$$$$$$$$$$$$\nstart print list %p\n----------------", ptr);
    while (ptr)
    {
        if (get_flag_value(ptr, TINY_BLOCK))
        {
            type = "Tiny Block";
            name[0] = 'T';
            name[1] = '\0';
            nb_t += 1;
            ft_strcat(name, ft_itoa(nb_t));
        }
        else if (get_flag_value(ptr, SMALL_BLOCK))
        {
            type = "Small Block";
            name[0] = 'S';
            name[1] = '\0';
            nb_s += 1;
            ft_strcat(name, ft_itoa(nb_s));
        }
        else
        {
            type = "Big Block";
            name[0] = 'B';
            name[1] = '\0';
            nb_b += 1;
            ft_strcat(name, ft_itoa(nb_b));
        }
        if (ptr == g_data.head || ptr == next_page)
        {
            next_page = ptr->page;
            ft_printf("\n{green}Début de page : %s{reset}\n", type);
        }
        //     ft_printf("\n{yellow}Nom : %s{reset}\n\nAddresse: %p\nAdresse data: %p\n\n\
// Type: %s\nsize: %lu\nFree: %d\n\nPrev: %p, next: %p\n------------------------", name, ptr, ptr + 1, type, ptr->size, ptr->flag & 1, ptr->prev, ptr->next);
        ft_printf("\n{yellow}Nom : %s{reset}\n\nAddresse: %p\nAdresse data: %p\n\n\
Type: %s\nsize: %lu\nFree: %d\n\nData : %s\n\nPrev: %p, next: %p\n------------------------",
                  name, ptr, ptr + 1, type, ptr->size, ptr->flag & 1, (char *)(ptr + 1), ptr->prev, ptr->next);
        ptr = ptr->next;
    }
}