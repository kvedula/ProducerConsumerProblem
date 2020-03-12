//Micky Chettanapanich 90208070, Lily Pham 46103697

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
int incoming_item_index = 0;
int outgoing_item_index = 0;
int buffer_item_count = 0;
int num_producer;
int num_consumer;
int num_items;
int delay;

pthread_cond_t notempty = PTHREAD_COND_INITIALIZER;
pthread_cond_t notfull = PTHREAD_COND_INITIALIZER;

void *producer(int *prod_count);
void *consumer(int *con_count);

int main(int argc, char *argv[])
{
    num_consumer = atoi(argv[2]);
    num_producer = atoi(argv[1]);
    num_items = atoi(argv[3]);
    delay = atoi(argv[4]);

    // Initialize mutex
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        fprintf(stderr, "Failed to initialize mutex\n");
    }

    // Initializing threads
    pthread_t prod_thread[num_producer];
    pthread_t con_thread[num_consumer];
    int producer_count, consumer_count;

    int prod_index[num_producer];
    int con_index[num_consumer];

    for (producer_count = 0; producer_count < num_producer; producer_count++)
    {
        prod_index[producer_count] = producer_count;
        printf("main(): producer_count=%d\n", producer_count);
        if (pthread_create(&prod_thread[producer_count], NULL, &producer, &prod_index[producer_count]) != 0)
        {
            fprintf(stderr, "Failed to create producer thread\n");
        }
    }

    for (consumer_count = 0; consumer_count < num_consumer; consumer_count++)
    {
        con_index[consumer_count] = consumer_count;
        if (pthread_create(&con_thread[consumer_count], NULL, &consumer, &con_index[consumer_count]) != 0)
        {
            fprintf(stderr, "Failed to create consumer thread\n");
        }
    }

    // Make sure all threads are done before exiting the program
    int i;
    for (i = 0; i < num_producer; i++)
    {
        pthread_join(prod_thread[i], NULL);
    }

    for (i = 0; i < num_consumer; i++)
    {
        pthread_join(con_thread[i], NULL);
    }

    exit(0); // Terminates all threads
}

void *producer(int *prod_count)
{
    int i;
    for (i = *prod_count * num_items; i < (*prod_count + 1) * num_items; i++)
    {
        sbuf_insert(i);
        printf("producer_%d produced item %d\n", *prod_count, i);
    }
}

void *consumer(int *con_count)
{
    item buffer_item;
    int i;
    for (i = 0; i < (num_producer * num_items) / num_consumer; i++)
    {
        sbuf_remove(&buffer_item);
        printf("consumer_%d consumed item %d\n", *con_count, buffer_item);
    }
}

void sbuf_insert(item buffer_item)
{
    // IMPLEMENTATION WITHOUT SEMAPHORES
    pthread_mutex_lock(&mutex);

    // Inserting the item
    while (buffer_item_count >= BUFFER_SIZE)
    {
        pthread_cond_wait(&notfull, &mutex);
    }

    buffer[incoming_item_index] = buffer_item;
    incoming_item_index = (incoming_item_index + 1) % BUFFER_SIZE;

    buffer_item_count++;

    if (delay == 0)
        usleep(500000);

    pthread_cond_signal(&notempty);
    pthread_mutex_unlock(&mutex); // Unlock the buffer
}

void sbuf_remove(item *buffer_item)
{
    // IMPLEMENTATION WITHOUT SEMAPHORES
    pthread_mutex_lock(&mutex);
    // Inserting the item
    while (buffer_item_count <= 0)
    {
        pthread_cond_wait(&notempty, &mutex);
    }

    *buffer_item = buffer[outgoing_item_index];
    outgoing_item_index = (outgoing_item_index + 1) % BUFFER_SIZE;

    buffer_item_count--;

    if (delay == 1)
        usleep(500000);

    pthread_cond_signal(&notfull);
    pthread_mutex_unlock(&mutex);
}