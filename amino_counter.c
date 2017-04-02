//
// Created by male on 31/03/17.
//
#include "amino_counter.h"
#include <string.h>
#include <stdio.h>


#define STOP_POS 19
#define NO_AMINO -1

void amino_counter_create(amino_counter_t *self) {
    self->first = 0;
    self->second = 0;
    self->third = 0;
    self->amino_count = 0;
    for (int i = 0; i < 20; i++) self->amino_histogram[i] = 0;
}

void amino_counter_destroy(amino_counter_t *self){
    //nothing to do here
}

int amino_counter_get_first(amino_counter_t *self) {
    return self->first;
}

int amino_counter_get_second(amino_counter_t *self) {
    return self->second;
}

int amino_counter_get_third(amino_counter_t *self) {
    return self->third;
}

size_t amino_counter_get_amino_count(amino_counter_t *self) {
    return self->amino_count;
}

void amino_counter_process(amino_counter_t *self, size_t *amino, size_t length){
    for (int i = 0; i < length; ++i) {
        ++self->amino_histogram[amino[i]];
    }

    self->amino_count = self->amino_histogram[STOP_POS];

//    TODO TRES VECES CHEQUEAR EL MAYOR NESTED LOOP, USAR EL DE ARRIBA PARA SALTEAR EL QUE A FUE ELEGIDO ANTERIORMENTE
//    NO ME TENGO QUE PREOCIPAR POR ALFABETICO PORQUE YA ESTAN ORDENADOS.
    // size_t first_freq = -1;
    // size_t second_freq = -1;
    // size_t third_freq = -1;
    for (int k = 0; k < STOP_POS; ++k) {
        size_t current_freq = self->amino_histogram[k];
//TODO cambiar uso de self por la funcion
        if (current_freq > self->amino_histogram[self->first]){
            self->third = self->second;
            self->second = self->first;
            self->first = k;
        } else if (current_freq > self->amino_histogram[self->second]){
            self->third = self->second;
            self->second = k;
        } else if (current_freq > self->amino_histogram[self->third]){
            self->third = k;
        }
    }
    for (int j = 0; j < 20; ++j) {
//        printf("%zu\n", self->amino_histogram[j]);
    }
}

size_t amino_counter_get_freq(amino_counter_t *self, size_t amino_code){
    return self->amino_histogram[amino_code];
}

//int main(){
//    amino_counter_t counter;
//    amino_counter_create(&counter);
//    size_t amino[7] = {1,1,2,3,3,3,4};
//    amino_counter_process(&counter, amino, 7);
//    printf("primero: %i y  su freq: %zu\n", amino_counter_get_first(&counter), amino_counter_get_freq(&counter, amino_counter_get_first(&counter)));
//    printf("segundo: %i y  su freq: %zu\n", amino_counter_get_second(&counter), amino_counter_get_freq(&counter, amino_counter_get_second(&counter)));
//    printf("ter: %i y  su freq: %zu\n", amino_counter_get_third(&counter), amino_counter_get_freq(&counter, amino_counter_get_third(&counter)));
//    printf("stops: %zu\n", amino_counter_get_amino_count(&counter));
//
//    return (0);
//}
