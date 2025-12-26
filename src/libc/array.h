#pragma once

typedef struct __array_t {
    void* data;
    int size;
    int max_size;
    int data_size;
} array_t;
