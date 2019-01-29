cmake_minimum_required(VERSION 3.13)
project(BibVerwaltung C)

set(CMAKE_C_STANDARD 11)

set(GCC_COVERAGE_COMPILE_FLAGS "-std=c99 -Wall -Wextra -Wconversion -pedantic -ansi")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

include_directories(.)

add_executable(BibVerwaltung
        BuchLib.c
        BuchLib.h
        BuchLibSettings.h
        BuchLibLoadSave.c
        BuchLibLoadSave.h
        LinkedList.c
        LinkedList.h
        main.c
        BuchLibReadData.c
        BuchLibReadData.h
        )
