#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_TENANT_THREADS 5 

int avalilable_list_pop() {
  int max_random_number = 8999;
  int min_random_number = 1000;
  int random_number = rand() % max_random_number + min_random_number;
  return random_number;
}

void delivered_list_push(int property_code) {}

int rent_property() {
  int property_code = avalilable_list_pop();
  printf("alugando imóvel %d\n", property_code);
  return property_code;
}

void return_property(int property_code) {
  delivered_list_push(property_code);
  printf("devolvendo imóvel %d\n", property_code);
}

void *tenant_thread_function(void *arg) {
  while (1) {
    int property_code = rent_property();
    int max_random_number = 6;
    int min_random_number = 0;
    int random_number = rand() % max_random_number + min_random_number;
    sleep(random_number);
    return_property(property_code);
  }
}


int main() {
  pthread_t threads[NUM_TENANT_THREADS];
  long t;
  for(t=0; t<NUM_TENANT_THREADS; t++){
    printf("In main: creating thread %ld\n", t); 
    int res_thread;
    res_thread = pthread_create(&threads[t], NULL, tenant_thread_function, NULL);
    if (res_thread){
      printf("ERROR; return code from pthread_create() is %d\n", res_thread);
      exit(-1);
    }
  }
  for(t=0; t<NUM_TENANT_THREADS; t++){
    pthread_join(threads[t], NULL);
  }
  return 0;
}