#ifndef __ENCODER_H__
#define __ENCODER_H__
#include <stdio.h>
#include <stdlib.h>
unsigned char codon_encoder(unsigned char *codon);
void encode_codon_str(unsigned char *source, unsigned char *dest, size_t dest_size);

#endif
