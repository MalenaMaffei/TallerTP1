//
// Created by male on 29/03/17.
//

#ifndef __DECODER_H__
#define __DECODER_H__
#include <stdio.h>
#include <stdlib.h>

void decode_buffer(unsigned char *buffer, size_t *dest, size_t lenght);
const char* amino_name(size_t amino_code);

#endif //__DECODER_H__

