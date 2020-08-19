#include "malloc.h"

void add_meta_data(t_meta_data *projection, size_t size, int type)
{
  // t_meta_data meta_data;

  projection->flag = 0;
  toggle_flag(projection, 1, FREE);
  toggle_flag(projection, 1, type);
  projection->next = NULL;
  projection->prev = NULL;
  projection->size = size - sizeof(t_meta_data);
  projection->page = NULL;
}

void *get_new_projection(size_t size, int type)
{
  void *projection;
  size_t proj_size;
  // int page_type;

  projection = NULL;
  proj_size = page_size_for_type(type, size + sizeof(t_meta_data));
  ft_printf("{red}%lu\n", proj_size);
  if (!(projection = mmap(NULL, proj_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0)))
    return NULL;
  add_meta_data((t_meta_data *)projection, proj_size, type);
  add_projection((t_meta_data *)projection);
  return projection;
}

void add_projection(t_meta_data *projection)
{
  t_meta_data *ptr;

  ptr = g_data.head;
  if (!ptr)
  {
    g_data.head = projection;
    g_data.tail = projection;
    g_data.page_tail = projection;
    g_data.tail->prev = NULL;
    return;
  }

  g_data.tail->next = projection;
  projection->prev = g_data.tail;
  g_data.page_tail->page = projection;
  g_data.tail = projection;
  g_data.page_tail = projection;
}