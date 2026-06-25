cmake_minimum_required(VERSION 4.0)
project(qbit_era C)

set(CMAKE_C_STANDARD 11)

add_executable(qbit_era main.c
        complex.c
        complex.h
        matrix.h
        matrix.c
        threads.h
        threads.c
        circuit.c
        circuit.h
        parser.c
        parser.h
        measure.h
        measure.c
)
