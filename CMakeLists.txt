cmake_minimum_required(VERSION 3.14)
project(convert_comment C)

set(CMAKE_C_STANDARD 99)

add_executable(convert_comment
        src/convert.c
        src/utili.h
        src/log.h src/log.c
        src/convertcomment.h src/convertcomment.c
        src/input.c
        )