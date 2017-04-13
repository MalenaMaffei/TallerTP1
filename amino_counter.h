#ifndef __AMINO_COUNTER_H__
#define __AMINO_COUNTER_H__

#include <stdlib.h>
#define AMINO_Q 20
#define STOP_POS 19

//Tipo amino_counter_t, calcula las frecuencias de los aminoácidos
//y los ordena en base a ella.
typedef struct {
    size_t amino_count;
    size_t amino_histogram[AMINO_Q];
    size_t ordered_aminos[STOP_POS];
} amino_counter_t;

// Inicializa la instancia self para ser utilizada
// Pre: self apunta un sector válido de memoria
void amino_counter_create(amino_counter_t *self);

// Procesa la tira de aminoácidos amino de largo length.
// Pre: self fue inicializado mediante create
// Amino es un array de aminoácidos validos y length es <= a su tamaño.
void amino_counter_process(amino_counter_t *self, size_t *amino, size_t length);

// Devuelve el aminoácido con el rango dado o -1 si no existe.
// Pre: self fue inicializado mediante create y se han procesado aminoacidos.
int amino_counter_get_rank(amino_counter_t *self, int rank);

// Devuelve la cantidad de aaminoácidos procesados.
// Pre: self fue inicializado mediante create.
size_t amino_counter_get_amino_count(amino_counter_t *self);

// Devuelve la frecuencia del aminoácido dado y -1 si no existe.
// Pre: self fue inicializado mediante create.
size_t amino_counter_get_freq(amino_counter_t *self, size_t amino_code);

#endif //__AMINO_COUNTER_H__
