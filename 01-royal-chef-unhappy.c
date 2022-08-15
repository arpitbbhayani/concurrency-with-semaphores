#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Activities of the Chef and the King
typedef enum { 
  CHEF_WAKES_UP,
  CHEF_PREPARATION_START,
  CHEF_PREPARATION_END,

  KING_WAKES_UP,
  KING_ADDRESSES_PUBLIC,
  KING_VISITS_DINING_AREA,
  KING_EATS_LUNCH,
  KING_ANGRY,
} activity_t;


// represents if the lunch is ready or not
int is_prepared = 0;

// random delay while doing an activity
void act(activity_t activity) {
  usleep(50);
}

// chef's routine
void * routine_chef() {
  act(CHEF_WAKES_UP);
  act(CHEF_PREPARATION_START);
  act(CHEF_PREPARATION_END);

  is_prepared = 1;
  return NULL;
}


// king's routine
void * routine_king() {
  act(KING_WAKES_UP);
  act(KING_ADDRESSES_PUBLIC);
  act(KING_VISITS_DINING_AREA);

  // if king sees that the food is not prepared
  // after visiting the dining area
  // the king gets angry
  if (!is_prepared) {
    act(KING_ANGRY);
    return (void *) KING_ANGRY;
  }
  
  // otherwise the king eats the lunch
  act(KING_EATS_LUNCH);
  return (void *) KING_EATS_LUNCH;
}


// resets everything
void reset() {
  is_prepared = 0;
  srand(time(NULL));
}

void test() {
  pthread_t th_king, th_chef;

  reset();

  // start the chef's routine
  if (pthread_create(&th_chef, NULL, &routine_chef, NULL) != 0) {
    perror("failed to create the chef thread");
  }

  // start the king's routine
  if (pthread_create(&th_king, NULL, &routine_king, NULL) != 0) {
    perror("failed to create the king thread");
  }

  // wait for the threads to complete
  if (pthread_join(th_chef, NULL) != 0) {
    perror("failed to join on chef thread");
  }

  // check the return value of king's routine
  // and see if he was angry or not
  activity_t state_king;
  if (pthread_join(th_king, (void *) &state_king) != 0) {
    perror("failed to join on king thread");
  }

  // print if king was angry
  printf("is king angry: %d\n", state_king == KING_ANGRY);
}

int main(int argc, char *argv[]) {
  for (int i = 0; i < 10; i++) {
    test();
  }
  return 0;
}