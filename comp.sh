#!/bin/bash
printf "\n--------------------------Cliente:---------------------------- \n"
gcc -std=c99 -Wall -Werror -pedantic -pedantic-errors -ggdb -DDEBUG -fno-inline -o client client.c encoder.c
echo ""
echo "CPP LINT"
echo ""
python ./cpplint.py --filter=`cat filter_options` client.c encoder.c encoder.h
printf "\n--------------------------Servidor: ----------------------------\n"
gcc -std=c99 -Wall -Werror -pedantic -pedantic-errors -ggdb -DDEBUG -fno-inline -o server server.c decoder.c amino_counter.c
echo ""
echo "CPP LINT"
echo ""
python ./cpplint.py --filter=`cat filter_options` server.c decoder.c amino_counter.c decoder.h amino_counter.h
