cmake_minimum_required(VERSION 3.13)
project(BibVerwaltung C)

set(CMAKE_C_STANDARD 99)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -Wconversion -pedantic")
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
        BuchLibMenu.c
        BuchLibMenu.h
        )
