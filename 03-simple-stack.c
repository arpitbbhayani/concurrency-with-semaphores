#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

// concurrent stack to hold int values
struct cstack {
  int len;
  int capacity;
  int *array;
};
typedef struct cstack cstack;

// creating a new instance of concurrent stack
cstack * cs_new(int capacity) {
  // allocating the stack
  cstack *s = (cstack *) malloc(1 * sizeof(cstack));
  s -> len = 0;
  s -> capacity = capacity;
  s -> array = (int *) malloc(capacity * sizeof(int));
  return s;
}

// destroying and marking the malloc'ed objects free for GC
void cs_destroy(cstack *s) {
  free(s -> array);
  free(s);
}

// push `x` on the stack
void cs_push(cstack *s, int x) {
  s -> array[(s -> len)++] = x;
}

// pops the last element from the stack
int cs_pop(cstack *s) {
  return s -> array[--(s -> len)];
}

int main(int argc, char *argv[]) {
  cstack *s = cs_new(10);

  for (int i = 0; i < 10; i++) {
    cs_push(s, i);
  }
 
  for (int i = 0; i < 10; i++) {
    printf("%d\n", cs_pop(s));
  }

  cs_destroy(s);
  return 0;
}
