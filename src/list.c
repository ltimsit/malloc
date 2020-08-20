#include "malloc.h"

// insers le nouveau block occupé a la place du free, créer un block free avec ce quil reste
void insert_block(t_meta_data *ptr, size_t size, int type)
{
  t_meta_data *tmp;

  tmp = NULL;
  if (ptr->size > sizeof(t_meta_data) && size < ptr->size - sizeof(t_meta_data))
  {
    tmp = (void *)ptr + sizeof(t_meta_data) + size;
    tmp->size = ptr->size - (size + sizeof(t_meta_data));
    toggle_flag(tmp, 1, FREE);
    toggle_flag(tmp, 1, type);
    tmp->next = ptr->next;
    tmp->prev = ptr;
    ptr->next = tmp;
  }
  ptr->size = size;
  toggle_flag(ptr, 0, FREE);
  if (tmp)
  {
    if (!tmp->next)
      g_data.tail = tmp;
    else
      tmp->next->prev = tmp;
  }
}

// retourn le pointeur d'avant le nouvel espace
t_meta_data *find_space(int type, size_t size)
{
  size_t total_size;
  t_meta_data *list_ptr;

  total_size = size + sizeof(t_meta_data);
  list_ptr = g_data.head;
  while (list_ptr)
  {
    if (!get_flag_value(list_ptr, type))
    {
      list_ptr = list_ptr->page;
    }
    else
    {
      if (get_flag_value(list_ptr, FREE) && list_ptr->size >= size)
      {
        printf("\n\n jai trouve de la place %p \n\n", list_ptr);
        return list_ptr;
      }
      else
      {
        list_ptr = list_ptr->next;
      }
    }
  }
  return NULL;
}

void *ft_malloc(size_t size)
{
  void *allocated_ptr;
  // size_t proj_size;
  int type;

  //aligner les size en multiple de 4
  size = ((size - 1) >> 2 << 2) + 4;
  pthread_mutex_lock(&g_mutex);
  type = page_type_for_size(size + sizeof(t_meta_data));

  if (!(allocated_ptr = find_space(type, size)))
  {
    if (!(allocated_ptr = get_new_projection(size, type)))
      return NULL; //exit error
  }
  insert_block(allocated_ptr, size, type);
  pthread_mutex_unlock(&g_mutex);
  return allocated_ptr + sizeof(t_meta_data);
}
