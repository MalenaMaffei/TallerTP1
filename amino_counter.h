//
// Created by male on 31/03/17.
//

#ifndef __AMINO_COUNTER_H__
#define __AMINO_COUNTER_H__

#include <string.h>
#include <stdio.h>

typedef struct {
    int first;
    int second;
    int third;
    size_t amino_count;
    size_t amino_histogram[20];
} amino_counter_t;

void amino_counter_create(amino_counter_t *self);
int amino_counter_get_first(amino_counter_t *self);
int amino_counter_get_second(amino_counter_t *self);
int amino_counter_get_third(amino_counter_t *self);
size_t amino_counter_get_amino_count(amino_counter_t *self);
void amino_counter_process(amino_counter_t *self, size_t *amino, size_t length);
size_t amino_counter_get_freq(amino_counter_t *self, size_t amino_code);

#endif //__AMINO_COUNTER_H__

