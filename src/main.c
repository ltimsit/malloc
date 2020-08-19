#include "malloc.h"
#include "../libft/libft.h"

t_meta_data *ptr_by_name(char *name) {
    int type;
    int nb;
    t_meta_data *ptr;
    int tmp_nb;

    tmp_nb = 0;
    ptr = g_data.head;
    if (name[0] == 'T') {
        type = TINY_BLOCK;
    } else if (name[0] == 'S') {
        type = SMALL_BLOCK;
    } else if (name[0] == 'B') {
        type = LARGE_BLOCK;
    } else {
        return NULL;
    }
    if (!(nb = ft_atoi(name + 1)))
    {
        return NULL;
    }

    while (ptr) {
        if (!get_flag_value(ptr, type)) {
            ptr = ptr->page;
        }
        else
        {
            tmp_nb += 1;
            if (tmp_nb == nb) {
                return ptr + 1;
            }
            else {
                ptr = ptr->next;
            }
        }
    }
    return NULL;
}


int main(int a, char **c) {
    (void)a;
    (void)c;

    int r;
    int type;
    char buf[128];
    char tab[128];
    char **split;
    ft_bzero(tab, 128);

    // ft_malloc(5);
    // print_list();
    // ft_malloc(5);
    // print_list();

    while((r = read(0, buf, 128))) {
        buf[r] = '\0';
       type = 0;
        // ft_strcat(tab, buf);
        split = ft_strsplit(buf, ' ');
        printf("%d\n", ft_strncmp(tab, "malloc", 6));
        if (!ft_strcmp(split[0], "malloc") && split[1]) {
            // ft_bzero(tab, 128);
            type = 1;
            ft_malloc(ft_atoi(split[1]));
        } else if (!ft_strcmp(split[0], "realloc") && split[1] && split[2])
        {
            type = 2;
            ft_printf("{blue}%p{reset}\n", ptr_by_name(split[1]));
            ft_realloc(ptr_by_name(split[1]), ft_atoi(split[2]));
        } else if (!ft_strncmp(buf, "free", 4))
        {
            type = 3;
            // ft_free(ft_atoi(buf + 6));
        }
        if (type != 0) {
            print_list();
            // ft_bzero(tab, 128);
            type = 0;
        }
    }

    return (0);
}