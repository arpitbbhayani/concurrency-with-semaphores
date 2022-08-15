#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#define THREAD_COUNT 1000

// concurrent stack to hold int values
struct cstack {
  int len;
  int capacity;
  int *array;
  sem_t mutex;
  sem_t sem_ubound;
  sem_t sem_lbound;
};
typedef struct cstack cstack;

// creating a new instance of concurrent stack
cstack * cs_new(int capacity) {
  // allocating the stack
  cstack *s = (cstack *) malloc(1 * sizeof(cstack));
  s -> len = 0;
  s -> capacity = capacity;
  s -> array = (int *) malloc(capacity * sizeof(int));
  sem_init(&s -> mutex, 0, 1);
  sem_init(&s -> sem_ubound, 0, capacity);
  sem_init(&s -> sem_lbound, 0, 0);
  return s;
}

// destroying and marking the malloc'ed objects free for GC
void cs_destroy(cstack *s) {
  sem_destroy(&(s -> mutex));
  free(s -> array);
  free(s);
}

// push `x` on the stack
void cs_push(cstack *s, int x) {
  // allow only `capacity` number of elements in stack
  // post that wait
  sem_wait(&(s -> sem_ubound));

  // wait for the turn
  sem_wait(&(s -> mutex));

  // one thread to enter and insert into the stack
  s -> array[(s -> len)++] = x;

  // increase the counter allowing some other thread to enter
  sem_post(&(s -> mutex));

  // signal a thread who wants to pop
  sem_post(&(s -> sem_lbound));
}

// pops the last element from the stack
int cs_pop(cstack *s) {
  // wait if there is nothing to pop
  sem_wait(&(s -> sem_lbound));

  // wait for the turn
  sem_wait(&(s -> mutex));

  // one thread to enter and insert into the stack
  int x = s -> array[--(s -> len)];

  // increase the counter allowing some other thread to enter
  sem_post(&(s -> mutex));

  // since an element is popped, we can allow another thread
  // to push a new element on the stack
  sem_post(&(s -> sem_ubound));

  return x;
}

// struct to pass an argument to the thread
// at the beginning of the execution
struct arg_cstack_push {
  cstack * s;
  int x;
};
typedef struct arg_cstack_push arg_cstack_push;

void * push_through_thread(void * _arg) {
  arg_cstack_push *arg = (arg_cstack_push *) _arg;
  cs_push(arg->s, arg->x);
  return NULL;
}

// struct to pass an argument to the thread
// at the beginning of the execution
struct arg_cstack_pop {
  cstack * s;
};
typedef struct arg_cstack_pop arg_cstack_pop;

void * pop_through_thread(void * _arg) {
  arg_cstack_pop *arg = (arg_cstack_pop *) _arg;
  cs_pop(arg->s);
  return NULL;
}

void test() {
  cstack *s = cs_new(THREAD_COUNT);
  pthread_t *threads = (pthread_t *) malloc(THREAD_COUNT * sizeof(pthread_t));

  // creating the threads to push elements on the stack
  for (int i = 0; i < THREAD_COUNT/2; i++) {
    arg_cstack_pop arg = {s};
    if (pthread_create(&threads[i], NULL, pop_through_thread, &arg) != 0) {
      perror("error creating a producer thread");
      return;
    }
  }

  // creating the threads to push elements on the stack
  for (int i = THREAD_COUNT/2; i < THREAD_COUNT; i++) {
    arg_cstack_push arg = {s, i};
    if (pthread_create(&threads[i], NULL, push_through_thread, &arg) != 0) {
      perror("error creating a producer thread");
      return;
    }
  }

  // wait for all the threads to complete
  for (int i = 0; i < THREAD_COUNT; i++) {
    if (pthread_join(threads[i], NULL)) {
      perror("unable to wait for the thread to complete");
      return;
    }
  }

  printf("entries in stack: expected %d, observed %d\n", 0, s->len);

  free(threads);
  cs_destroy(s);
}

int main(int argc, char *argv[]) {
  for (int i = 0; i < 25; i++) {
    test();
  }
  return 0;
}
