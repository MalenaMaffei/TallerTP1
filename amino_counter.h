//
// Created by male on 31/03/17.
//

#ifndef __AMINO_COUNTER_H__
#define __AMINO_COUNTER_H__

#include <string.h>
#include <stdio.h>
#define AMINO_Q 20
#define STOP_POS 19

typedef struct {
    int first;
    int second;
    int third;
    size_t amino_count;
    size_t amino_histogram[AMINO_Q];
    size_t ordered_aminos[STOP_POS];
} amino_counter_t;

void amino_counter_create(amino_counter_t *self);
int amino_counter_get_rank(amino_counter_t *self, int rank);
size_t amino_counter_get_amino_count(amino_counter_t *self);
void amino_counter_process(amino_counter_t *self, size_t *amino, size_t length);
size_t amino_counter_get_freq(amino_counter_t *self, size_t amino_code);
void amino_counter_destroy(amino_counter_t *self);
#endif //__AMINO_COUNTER_H__

