//
// Created by male on 31/03/17.
//
#include "amino_counter.h"
#include <string.h>
#include <stdio.h>


void amino_counter_create(amino_counter_t *self) {
    self->first = 0;
    self->second = 0;
    self->third = 0;
    self->amino_count = 0;
    self->amino_histogram[20] = {20};
}

void amino_counter_destroy(amino_counter_t *self){
    //nothing to do here
}

size_t amino_counter_get_first(amino_counter_t *self) {
    return self->first;
}

size_t amino_counter_get_second(amino_counter_t *self) {
    return self->second;
}

size_t amino_counter_get_third(amino_counter_t *self) {
    return self->third;
}

size_t amino_counter_get_amino_count(amino_counter_t *self) {
    return self->amino_count;
}

void amino_counter_process(amino_counter_t *self, size_t *amino){

}