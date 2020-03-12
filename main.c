//Micky Chettanapanich mchettan 90208070
//Lily Pham lilyp3 46103697

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>
#define BUFFER_SIZE 8

typedef int item;
void sbuf_insert(item buffer_item);
void sbuf_remove(item *buffer_item);

item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t full_slot;
sem_t empty_slot;
int incoming_item_index = 0;
int outgoing_item_index = 0;
int buffer_item_count = 0;
int producer_count = 0;
int consumer_count = 0;
int num_items = 0;


pthread_cond_t *notempty = PTHREAD_COND_INITIALIZER;
pthread_cond_t *notfull = PTHREAD_COND_INITIALIZER;

void *producer(int delay);
void *consumer(int delay);

int main(int argc, char **argv) {
    int num_producer = atoi(argv[1]);
    int num_consumer = atoi(argv[2]);
    num_items = atoi(argv[3]);
    int delay = atoi(argv[4]);

    //Initialize buffer
    if (sem_init(&empty_slot, 0, BUFFER_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize empty_slot\n");
    }

    if (sem_init(&full_slot, 0, 0) != 0) {
        fprintf(stderr, "Failed to initialize full_slot\n");
    }

    // Initialize mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        fprintf(stderr, "Failed to initialize mutex\n");
    }

    // Initializing threads
    int i;
    pthread_t prod_thread[num_producer];
    pthread_t con_thread[num_consumer];

    for(i = 0; i < num_producer; i++)
    {
        //printf("Producer working\n");
        pthread_create(&prod_thread[i], NULL, producer, delay);
    }

    for(i = 0; i < num_consumer; i++)
    {
        //printf("Consumer working\n");
        pthread_create(&con_thread[i], NULL, consumer, delay);
    }


    return 0;
}

void *producer(int delay){
    item buffer_item = 1;
    int i;

    //for(i = producer_count * num_items; i < (producer_count + 1) * i - 1 ; i++){
    while(1){
        //printf("i value: %d\n", i);
        buffer_item = 1;
        sbuf_insert(buffer_item);

        if(delay == 0)
            usleep(500000);
    }

//    while(1){
//        pthread_mutex_lock(&mutex);
//
//        // Inserting the item
//        if (buffer_item_count != BUFFER_SIZE) {
//            pthread_cond_wait(&notfull, &mutex);
//        }
//
//        buffer[incoming_item_index] = buffer_item;
//        incoming_item_index = (incoming_item_index + 1) % BUFFER_SIZE;
//
//        buffer_item_count++;
//
//        printf("producer_%d produced item %d\n", 0, 1);
//
//        pthread_cond_signal(&notempty);
//        pthread_mutex_unlock(&mutex); // Unlock the buffer
//        //sem_post(&full_slot); // Increment # of filled spots (Unlocks semaphore)
//    }
}

void *consumer(int delay){
    item *buffer_item;
    while(1){
        buffer_item = 1;
        sbuf_remove(&buffer_item);

        if(delay == 1)
            usleep(500000);
    }

//    while(1) {
//        pthread_mutex_lock(&mutex);
//
//        // Inserting the item
//        if (buffer_item_count != 0) {
//            pthread_cond_wait(&notempty, &mutex);
//        }
//
//        *buffer_item = buffer[outgoing_item_index];
//        outgoing_item_index = (outgoing_item_index + 1) % BUFFER_SIZE;
//
//        buffer_item_count--;
//
//        printf("consumer_%d consumed item %d\n", 0, 1);
//
//        pthread_cond_signal(&notfull);
//        pthread_mutex_unlock(&mutex);
//
////        if(delay == 1)
////            usleep(500000);
//    }
}

void sbuf_insert(item buffer_item){
    sem_wait(&empty_slot);   // Decrement # of empty_slot slots (Locks semaphore)
    pthread_mutex_lock(&mutex); // Lock the buffer

    // Inserting the item
    if (buffer_item_count != BUFFER_SIZE) {
        buffer[incoming_item_index] = buffer_item;
        incoming_item_index = (incoming_item_index + 1) % BUFFER_SIZE;

        buffer_item_count++;
    }

    printf("producer_%d produced item %d\n", producer_count, 1);


    pthread_mutex_unlock(&mutex); // Unlock the buffer
    sem_post(&full_slot); // Increment # of filled spots (Unlocks semaphore)
}

void sbuf_remove(item *buffer_item){
    sem_wait(&full_slot); // Decrement # of filled spots (Locks semaphore)
    pthread_mutex_lock(&mutex); // Lock the buffer

    // Remove the item
    if (buffer_item_count != 0) {
        *buffer_item = buffer[outgoing_item_index];
        outgoing_item_index = (outgoing_item_index + 1) % BUFFER_SIZE;

        buffer_item_count--;
    }

    printf("consumer_%d consumed item %d\n", producer_count, 1);

    pthread_mutex_unlock(&mutex); // Unlock the buffer
    sem_post(&empty_slot); // Increment # of empty_slot spots (Unlocks semaphore)
}