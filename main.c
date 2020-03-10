#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define BUFFER_SIZE 8

typedef int buffer_item;

buffer_item buffer[BUFFER_SIZE];

int main(int argc, char **argv) {
    int num_producer = atoi(argv[1]);
    int num_consumer = atoi(argv[2]);
    int num_items = atoi(argv[3]);
    int delay = atoi(argv[4]);

    printf("%d %d %d %d", num_producer, num_consumer, num_items, delay);

    return 0;
}
