//Micky Chettanapanich mchettan 90208070
//Lily Pham

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define BUFFER_SIZE 8

typedef int item;
item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t full, empty;
int incoming_item_index = 0;
int outgoing_item_index = 0;
int buffer_item_count = 0;
int producer_count = 0;
int consumer_count = 0;
int item_count = 0;

void *producer(int delay);
void *consumer(int delay);
int sbuf_insert(item buffer_item);
int sbuf_remove(item *buffer_item);

int main(int argc, char **argv) {
    int num_producer = atoi(argv[1]);
    int num_consumer = atoi(argv[2]);
    int num_items = atoi(argv[3]);
    int delay = atoi(argv[4]);
    pthread_mutex_init(&mutex, NULL);

    //Initialize buffer
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);

    // Initializing threads
    int i;
    pthread_t prod_thread[num_producer];
    pthread_t con_thread[num_consumer];

    for(i = 0; i < num_producer; i++)
    {
        pthread_create(&prod_thread[i], NULL, producer, NULL);
    }

    for(i = 0; i < num_consumer; i++)
    {
        pthread_create(&con_thread[i], NULL, consumer, NULL);
    }


    //printf("%d %d %d %d", num_producer, num_consumer, num_items, delay);

    return 0;
}

void *producer(int delay){
    item buffer_item;

    while(1){
        if(delay == 0)
            usleep(0.5);

        buffer_item = 0;
        sbuf_insert(buffer_item);

        printf("producer_%s produced item %s\n", producer_count, item_count);
    }
}

void *consumer(int delay){
    item buffer_item;

    while(1){
        if(delay == 1)
            usleep(0.5);

        sbuf_remove(&buffer_item);
        printf("consumer_%s consumed item %s\n", consumer_count, item_count);
    }
}

int sbuf_insert(item buffer_item){
    sem_wait(&empty);   // Wait for available slot
    pthread_mutex_lock(&mutex); // Lock the buffer

    // Inserting the item
    if(buffer_item_count != BUFFER_SIZE)
    {
        buffer[incoming_item_index] = buffer_item;
        incoming_item_index = (incoming_item_index + 1) % BUFFER_SIZE;

        buffer_item_count++;
    }


    pthread_mutex_unlock(&mutex); // Unlock the buffer
    sem_post(&full); // Announce available item

}

int sbuf_remove(item *buffer_item){
    sem_wait(&full); // Wait for available item
    pthread_mutex_lock(&mutex); // Lock the buffer

    // Remove the item
    if(buffer_item_count != 0)
    {
        *buffer_item = buffer[outgoing_item_index];
        outgoing_item_index = (outgoing_item_index + 1) % BUFFER_SIZE;

        buffer_item_count--;
    }


    pthread_mutex_unlock(&mutex); // Unlock the buffer
    sem_post(&empty); // Announce available spot
}