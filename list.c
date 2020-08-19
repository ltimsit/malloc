#include "malloc.h"



// insers le nouveau block occupé a la place du free, créer un block free avec ce quil reste
void insert_block(t_meta_data *ptr, size_t size) {
  t_meta_data *tmp;
  
  tmp = (void *)ptr + sizeof(t_meta_data) + size;
  tmp->size = ptr->size - (size + sizeof(t_meta_data));
  toggle_flag(tmp, 1, FREE);
  ptr->size = size;
  toggle_flag(ptr, 0, FREE);
  tmp->next = ptr->next;
  ptr->next = tmp;
  tmp->prev = ptr;

  if (!tmp->next) {
    g_data.tail = tmp;
  } else {
    tmp->next->prev = tmp;
  }
}

// retourn le pointeur d'avant le nouvel espace
t_meta_data *find_space(int type, int size) {
    size_t total_size;
    t_meta_data *list_ptr;

    total_size = size + sizeof(t_meta_data);
    list_ptr = g_data.head;

    while (list_ptr) {
        if (list_ptr->page && !get_flag_value(list_ptr, type)) {
            list_ptr = list_ptr->page;
        }
        else
        {
            if (list_ptr->size >= size) {
                return list_ptr;
            }
            else {
                list_ptr = list_ptr->next;
            }
        }
    }
    return NULL;
}

void *ft_malloc(size_t size)
{
  void *allocated_ptr;
  size_t proj_size;
  int type;

  //aligner les size en multiple de 4
  size = ((size - 1 >> 2) << 2) + 4;


type = page_type_for_size(size + sizeof(t_meta_data));

if (!(allocated_ptr = find_space(type, size))) {
    if (!(allocated_ptr = get_new_projection(size, type)))
      return NULL; //exit error
}
insert_block(allocated_ptr, size);
return allocated_ptr;
}
