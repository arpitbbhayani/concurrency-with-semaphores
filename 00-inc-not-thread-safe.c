#include <stdio.h>
#include <pthread.h>

#define THREAD_COUNT 1000

int count = 0;

void * inc_count() {
  for (int i = 0; i < THREAD_COUNT; i++) {
    count ++;
  }
  return NULL;
}

void reset() {
  count = 0;
}

void test_inc() {
  pthread_t th[THREAD_COUNT];

  reset();

  for (int i = 0; i < THREAD_COUNT; i++) {
    if (pthread_create(&th[i], NULL, &inc_count, NULL) != 0) {
      perror("failed to create thread");
    }
  } 

  for (int i = 0; i < THREAD_COUNT; i++) {
    if (pthread_join(th[i], NULL) != 0) {
      perror("failed to join thread");
    }
  }

  printf("expected %d, observed %d\n", THREAD_COUNT * THREAD_COUNT, count);
}

int main(int argc, char *argv[]) {
  for (int i = 0; i < 10; i++) {
    test_inc();
  }
  return 0;
}