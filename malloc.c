#include "malloc.h"

void memdump(char *adr) {
  printf("%.4hhd ", *adr);
}

void print_list() {
  int i;
  t_meta_data *ptr;
  ptr = g_data;
  printf("start print list\n");
  while(ptr) {
    printf("size: %lu, free: %d, next: %p\n", ptr->size, ptr->free, ptr->next);
    ptr = ptr->next;
  }
}

void insert_meta_data(void *ptr, size_t size, int free) {
  t_meta_data new_meta_data;

  new_meta_data.size = size;
  new_meta_data.next = ptr + sizeof(t_meta_data);
  new_meta_data.free = free;

  memcpy(ptr, &new_meta_data, sizeof(t_meta_data)); 
}

/* void *check_for_space(size_t size) {
  t_meta_data *ptr;

  ptr = g_data;
  while(ptr) {
    if (ptr->free) {
      return ptr;
    }
  }
} */

int add(size_t size) {
  t_meta_data *ptr;
  t_meta_data *temp;
  t_meta_data *new_free;

  ptr = g_data;
  temp = NULL;
  while (ptr) {
    if (ptr->free && ptr->size >= size + sizeof(t_meta_data)) {
      if (ptr->next) {
        temp = ptr->next;
      }
      new_free = ptr + size + sizeof(t_meta_data);
      new_free->next = temp;
      new_free->size = ptr->size - size;
      new_free->free = 1;
      ptr->next = new_free;
      ptr->size = size;
      ptr->free = 0;
      return 1;
    }
  }
  printf("test");
  return 0;
}

/* void add_to_allocated_space(t_meta_data *ptr, size_t size) {
  t_meta_data *

  ptr->next = ptr + size;
  ptr->free = 0;
  ptr->size = size - sizeof(t_meta_data);
} */

/* void add_to_allocated_list(void *ptr, size_t size, int free) {
  t_meta_data *curr_ptr;

  curr_ptr = g_data;

  while(curr_ptr->next) {
    curr_ptr = curr_ptr->next;
  }

  curr_ptr->next = ptr;
} */

void add_meta_data(void *projection, size_t size) {
  t_meta_data meta_data;
  meta_data.free = 1;
  meta_data.next = NULL;
  meta_data.size = size - sizeof(t_meta_data);
  memcpy(projection, &meta_data, size);
}

void *get_new_projection(size_t total_data_size) {
  void *projection;
  size_t proj_size;

  projection = NULL;
  if (total_data_size < _N) {
    proj_size = _N;
  } else if  (total_data_size < _M) {
    proj_size = _M;
  } else {
    proj_size = total_data_size;
  }

  projection = mmap(NULL, proj_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, -1);
  add_meta_data(projection, total_data_size);
  return projection;
}

void add_projection(void *projection) {
  t_meta_data *ptr;
  ptr = g_data;

  if (!ptr) {
    ptr = projection;
    return ;
  }

  while(ptr->next) {
    ptr = ptr->next;
  }
  ptr->next = (t_meta_data *)projection;
}

void *malloc(size_t size) {
  void *projection;
  size_t proj_size;

  size_t total_data_size = size + sizeof(t_meta_data);

  if (!add(size)) {
    projection = get_new_projection(total_data_size);
    add_projection(projection);
  }
}

int main(int c, char **v) {
  char *test, *test3;
  unsigned int *test2;
  int i;
  i = -4;

  printf("test");
  test = malloc(2);
  print_list();
  test2 = malloc(4);
  print_list();
  
}
