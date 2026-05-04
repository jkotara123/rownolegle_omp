#include "bucket.h"

void bucket_init(bucket_t *b, int capacity) {
    b->size = 0;
    b->capacity = capacity;
    b->data = malloc(capacity * sizeof(int));
}

void bucket_push(bucket_t *b, int value) {
    if (b->size == b->capacity) {
        b->capacity *= 2;
        b->data = realloc(b->data, b->capacity * sizeof(int));
    }
    b->data[b->size++] = value;
}

void bucket_free(bucket_t *b) {
    free(b->data);
    b->data = NULL;
    b->size = 0;
    b->capacity = 0;
}