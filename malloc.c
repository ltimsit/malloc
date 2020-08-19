#include "ft_printf.h"
#include "malloc.h"

void memdump(char *adr)
{
  printf("%.4hhd ", *adr);
}

int toggle_flag(t_meta_data *ptr, int value, int flag) {
  if (value) {
    ptr->flag |= flag;
  } else {
    ptr->flag &= ~flag;
  }
}

int get_flag_value(t_meta_data *ptr, int flag) {
  return ptr->flag & flag;
}

void print_list()
{
  int i;
  t_meta_data *ptr;
  ptr = g_data.head;
  printf("start print list\n");
  while (ptr)
  {
    printf("addr: %p, size: %lu, free: %d, prev: %p, next: %p\n", ptr, ptr->size, ptr->flag & 1, ptr->prev, ptr->next);
    ptr = ptr->next;
  }
}

int page_type_for_size(size_t size) {
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

int page_size_for_type(int page_type, size_t size) {
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

void *add(size_t size)
{
  t_meta_data *ptr;
  int curr_page;
  int page_type;
  
  ptr = g_data.head;

  page_type = page_type_for_size(size + sizeof(t_meta_data));
  curr_page = 0;

  while (ptr)
  {
    if (ptr->page)
      curr_page = get_flag_value(ptr->page, page_type);
    if (curr_page && get_flag_value(ptr, FREE) && (ptr->size >= size + sizeof(t_meta_data)))
    {
      insert_block(ptr, size);
      return ptr + 1;
    }
    ptr = ptr->next;
  }
  return NULL;
}

void add_meta_data(void *projection, size_t size)
{
  t_meta_data meta_data;

  meta_data.flag = 0;
  toggle_flag(&meta_data, 1, FREE);
  if (size == _N)
    toggle_flag(&meta_data, 1, TINY_BLOCK);
  else if (size == _M)
    toggle_flag(&meta_data, 1, SMALL_BLOCK);
  else
    toggle_flag(&meta_data, 1, LARGE_BLOCK);
  meta_data.next = NULL;
  meta_data.size = size - sizeof(t_meta_data);
  meta_data.page = NULL;
  memcpy(projection, &meta_data, sizeof(t_meta_data));
}

void *get_new_projection(size_t total_data_size)
{
  void *projection;
  size_t proj_size;
  int page_type;
  
page_type = page_type_for_size(total_data_size);


  projection = NULL;
  proj_size = page_size_for_type(page_type, total_data_size);
/*   if (total_data_size < _N / 100)
  {
    proj_size = _N;
  }
  else if (total_data_size < _M / 100)
  {
    proj_size = _M;
  }
  else
  {
    proj_size = total_data_size;
  } */

  projection = mmap(NULL, proj_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
  add_meta_data(projection, proj_size);
  return projection;
}

void add_projection(void *projection)
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

  g_data.tail->next = (t_meta_data *)projection;
  g_data.tail->next->prev = g_data.tail;
  g_data.page_tail->next = (t_meta_data *)projection;
  g_data.tail = (t_meta_data *)projection;
  g_data.page_tail = (t_meta_data *)projection;
}

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

int check_enough_space(size_t size, void *ptr)
{
  t_meta_data *temp_ptr;

  if (!(temp_ptr = find_ptr_in_stack(ptr)))
    return PTR_ERROR; //error
  temp_ptr = free_space(temp_ptr);
  if (temp_ptr->size >= size) {
    insert_block(temp_ptr, size);
    return REALLOC_SUCCESS;
  }

  return REALLOC_FAILURE;
}

void *ft_realloc(void *ptr, size_t size)
{
  size = ((size - 1 >> 2) << 2) + 4;
  int realloc_try;

  if (!(realloc_try = check_enough_space(size, ptr)))
    return NULL; //error wrong ptr
  return realloc_try == 1 ? ft_malloc(size) : ptr;
}

t_meta_data *free_space(t_meta_data *ptr) {
  size_t total_size;
  t_meta_data *tmp;
  total_size = ptr->size;

  tmp = ptr;
  toggle_flag(ptr, 1, FREE);
  if (ptr->prev && get_flag_value(ptr->prev, FREE)) {
    total_size += sizeof(t_meta_data) + ptr->prev->size;
    tmp = ptr->prev;
  } else {
    tmp = ptr;
  }
  if (ptr->next && get_flag_value(ptr->next, FREE)) {
    total_size += sizeof(t_meta_data) + ptr->next->size;
    tmp->next = ptr->next->next;
    if (!tmp->next) {
      g_data.tail = tmp;
    }
  } else {
    tmp->next = ptr->next;
  }
  tmp->size = total_size;

  if (tmp->next) {
    tmp->next->prev = tmp;
  }
  return tmp;
}

void insert_block(t_meta_data *ptr, size_t size) {
  t_meta_data *tmp;
  tmp = (void *)ptr + sizeof(t_meta_data) + size;
  tmp->size = ptr->size - (size + sizeof(t_meta_data));
  toggle_flag(tmp, 1, FREE);
  ptr->size = size;
  toggle_flag(ptr, 0, FREE);
  tmp->next = ptr->next;
  tmp->page = NULL;
  ptr->next = tmp;
  tmp->prev = ptr;

  if (!tmp->next) {
    g_data.tail = tmp;
  } else {
    tmp->next->prev = tmp;
  }
}

int check_if_free_page(t_meta_data *ptr) {
  size_t total_size;

  total_size = ptr->size + sizeof(t_meta_data);
  if (total_size == _N || total_size >= _M) {
    if (ptr->prev) {
      ptr->prev->next = ptr->next;
    } else {
      if (ptr->next) {
        ptr->next->prev = NULL;
        g_data.head = ptr->next;
      }
    }
    if (ptr->next) {
      ptr->next->prev = ptr->prev;
    } else {
      if (ptr->prev) {
        ptr->prev->next = NULL;
        g_data.tail = ptr;
      }
    }
    if ((munmap(ptr, total_size) == -1))
      return 0; //error
  }
}

void ft_free(void *ptr)
{
  t_meta_data *temp_ptr;

  if (!(temp_ptr = find_ptr_in_stack(ptr)))
    return ;
  temp_ptr = free_space(temp_ptr);
  check_if_free_page(temp_ptr);
}

void *ft_malloc(size_t size)
{
  void *projection;
  void *allocated_ptr;
  size_t proj_size;
  //aligner les size en multiple de 4
  size = ((size - 1 >> 2) << 2) + 4;
  size_t total_data_size = size + sizeof(t_meta_data);
  while (!(allocated_ptr = add(size)))
  {
    projection = get_new_projection(total_data_size);
    if (!projection)
    {
      return NULL; //exit error
    }
    add_projection(projection);
  }
}

int main(int c, char **v)
{
  char *test, *test3;
  unsigned int *test2;
  int i;
  i = 0;

  printf("%ld\n", sizeof(t_meta_data));
  test = ft_malloc(1);
  printf("%ld\n", sizeof(size_t));
  ft_malloc(10000);
  print_list();
  test = ft_realloc(test, 50);
  printf("%p\n", test);
  print_list();
  ft_free(test);
  print_list();
  
}
