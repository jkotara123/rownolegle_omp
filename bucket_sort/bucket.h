#ifndef BUCKET_H
#define BUCKET_H

#include <stdlib.h>

typedef struct {
    int *data;
    int size;
    int capacity;
} bucket_t;

void bucket_init(bucket_t *b, int capacity);

void bucket_push(bucket_t *b, int value);

void bucket_free(bucket_t *b);

#endif