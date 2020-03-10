#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define BUFFER_SIZE 8

/*
 * pthread_mutex_lock
 * pthread_mutex_unlock
 * pthread_cond_wait
 * pthread_cond_signal
 * pthread_cond_broadcast
 */

typedef int buffer_item;

buffer_item buffer[BUFFER_SIZE];
void lily_sucks_and_is_a_traitor();
void micky_is_cool();

int main(int argc, char **argv) {
    int num_producer = atoi(argv[1]);
    int num_consumer = atoi(argv[2]);
    int num_items = atoi(argv[3]);
    int delay = atoi(argv[4]);
    int i;
    pthread_t prod_thread[num_producer];
    pthread_t con_thread[num_consumer];
    int producer[num_producer];
    int consumer[num_consumer];

    for(i = 0; i < num_producer; i++)
    {
        producer[i] = i + 1;
    }

    for(i = 0; i < num_consumer; i++)
    {
        consumer[i] = i + 1;
    }





    printf("%d %d %d %d", num_producer, num_consumer, num_items, delay);

    return 0;
}

//void lily_sucks_and_is_a_traitor(){
//    while(1){
//
//    }
//
//}
//
//void micky_is_cool(){
//    while(1){
//
//    }
//}