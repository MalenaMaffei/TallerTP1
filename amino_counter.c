#include "amino_counter.h"

#define AMINO_Q 20
#define STOP_POS 19
#define NO_AMINO -1

void amino_counter_create(amino_counter_t *self) {
    self->amino_count = 0;
    for (int i = 0; i < AMINO_Q; ++i) self->amino_histogram[i] = 0;
    for (int j = 0; j < STOP_POS; ++j) self->ordered_aminos[j] = j;
}

void amino_counter_destroy(amino_counter_t *self){
    //nothing to do here
}

int amino_counter_get_rank(amino_counter_t *self, int rank) {
    if (rank > AMINO_Q){return -1;}
    return self->ordered_aminos[rank - 1];
}


size_t amino_counter_get_amino_count(amino_counter_t *self) {
    return self->amino_count;
}

void sort_by_freq(amino_counter_t *self){
    for (int j = 0; j < STOP_POS; ++j) self->ordered_aminos[j] = j;
    size_t *histogram = self->amino_histogram;
    size_t current, next;
    for (int j = 0; j < STOP_POS; ++j) {
        for (int i = 0; i < STOP_POS - 1; ++i) {
            current = self->ordered_aminos[i];
            next = self->ordered_aminos[i+1];
            if (histogram[next] > histogram[current]){
                self->ordered_aminos[i] = self->ordered_aminos[i+1];
                self->ordered_aminos[i+1] = current;
            }
        }
    }
}

void amino_counter_process(amino_counter_t *self, size_t *amino, size_t length){
    for (int i = 0; i < length; ++i) {
        ++self->amino_histogram[amino[i]];
    }

    self->amino_count = self->amino_histogram[STOP_POS];
    sort_by_freq(self);
}

size_t amino_counter_get_freq(amino_counter_t *self, size_t amino_code){
    if (amino_code > AMINO_Q - 1){return -1;}
    return self->amino_histogram[amino_code];
}
