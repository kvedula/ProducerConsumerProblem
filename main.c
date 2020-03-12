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
// sem_t full_slot;
// sem_t empty_slot;
int incoming_item_index = 0;
int outgoing_item_index = 0;
int buffer_item_count = 0;
int num_producer;
int num_consumer;
int num_items;
int delay;

pthread_cond_t notempty = PTHREAD_COND_INITIALIZER;
pthread_cond_t notfull = PTHREAD_COND_INITIALIZER;

void *producer(int* prod_count);
void *consumer(int* con_count);

int main(int argc, char *argv[]) {
    num_consumer = atoi(argv[2]);
    num_producer = atoi(argv[1]);
    num_items = atoi(argv[3]);
    delay = atoi(argv[4]);
/*
    //Initialize semaphores
    if (sem_init(&empty_slot, 0, BUFFER_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize empty_slot\n");
    }

    if (sem_init(&full_slot, 0, 0) != 0) {
        fprintf(stderr, "Failed to initialize full_slot\n");
    }*/

    // Initialize mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        fprintf(stderr, "Failed to initialize mutex\n");
    }

    // Initializing threads
    // int i;
    pthread_t prod_thread[num_producer];
    pthread_t con_thread[num_consumer];
    int producer_count, consumer_count;

    for(producer_count = 0; producer_count < num_producer; producer_count++)
    {
        printf("main(): Creating producers\n");
        if(pthread_create(&prod_thread[producer_count], NULL, producer, &producer_count) != 0) {
            fprintf(stderr, "Failed to create producer thread\n");
        }
        // pthread_join(prod_thread[producer_count], NULL);
    }

    for(consumer_count = 0; consumer_count < num_consumer; consumer_count++)
    {
        printf("main(): Creating consumers\n");
        pthread_create(&con_thread[consumer_count], NULL, consumer, &consumer_count);
        // pthread_join(con_thread[consumer_count], NULL);
    }

    // exit(0);    // Terminates all threads
    return 0;
}

void *producer(int* prod_count){
    printf("producer(): entered function; prod_count=%d\n", *prod_count);
    int i;
    for(i = *prod_count * num_items; i < (*prod_count + 1) * num_items; i++){
        // printf("producer(): i value=%d\n", i);
        sbuf_insert(i);
        printf("producer_%d produced item %d\n", *prod_count, i);
        if(delay == 0)
            usleep(500000);
    }
}

void *consumer(int* con_count){
    item buffer_item;
    int i;
    for(i = 0; i < (num_producer*num_items)/num_consumer; i++) {
        // printf("consumer(): i value=%d\n", i);
        sbuf_remove(&buffer_item);
        printf("consumer_%d consumed item %d\n", *con_count, buffer_item);    //TODO: fix lolol
        if(delay == 1)
            usleep(500000);
    }

}

void sbuf_insert(item buffer_item){
    /*sem_wait(&empty_slot);   // Decrement # of empty_slot slots (Locks semaphore)
    pthread_mutex_lock(&mutex); // Lock the buffer

    // Inserting the item
    // TODO: technically i don't think the if statement is needed since sem_Wait on empty_slot is called
    if (buffer_item_count != BUFFER_SIZE) {
        buffer[incoming_item_index] = buffer_item;
        incoming_item_index = (incoming_item_index + 1) % BUFFER_SIZE;

        buffer_item_count++;
    }

    pthread_mutex_unlock(&mutex); // Unlock the buffer
    sem_post(&full_slot); // Increment # of filled spots (Unlocks semaphore)*/

    // IMPLEMENTATION WITHOUT SEMAPHORES
    pthread_mutex_lock(&mutex);

    // Inserting the item
    while(buffer_item_count >= BUFFER_SIZE) {
        pthread_cond_wait(&notfull, &mutex);
    }

    buffer[incoming_item_index] = buffer_item;
    incoming_item_index = (incoming_item_index + 1) % BUFFER_SIZE;

    buffer_item_count++;

    pthread_cond_signal(&notempty);
    pthread_mutex_unlock(&mutex); // Unlock the buffer
}

void sbuf_remove(item *buffer_item){
    /*sem_wait(&full_slot); // Decrement # of filled spots (Locks semaphore)
    pthread_mutex_lock(&mutex); // Lock the buffer

    // Remove the item
    // if (buffer_item_count != 0) {
        int n = buffer[outgoing_item_index];
        outgoing_item_index = (outgoing_item_index + 1) % BUFFER_SIZE;

        buffer_item_count--;
    // }

    // printf("consumer_%d consumed item %d\n", producer_count, n);

    pthread_mutex_unlock(&mutex); // Unlock the buffer
    sem_post(&empty_slot); // Increment # of empty_slot spots (Unlocks semaphore)*/

    // IMPLEMENTATION WITHOUT SEMAPHORES
    pthread_mutex_lock(&mutex);
    printf("sbuf_remove(): num_consumer=%d\n", num_consumer);
    // Inserting the item
    while(buffer_item_count <= 0) {
        printf("sbuf_remove(): in while loop; buffer_item_count=%d\n", buffer_item_count);
        pthread_cond_wait(&notempty, &mutex);
    }

    *buffer_item = buffer[outgoing_item_index];
    outgoing_item_index = (outgoing_item_index + 1) % BUFFER_SIZE;

    buffer_item_count--;

    pthread_cond_signal(&notfull);
    pthread_mutex_unlock(&mutex);
}