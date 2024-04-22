cmake_minimum_required(VERSION 3.27)
project(COMP20200A3 C)

set(CMAKE_C_STANDARD 11)

add_executable(COMP20200A3
        server.c
        client.c
        QuizDB.h
        printquiz.c
        functions.h)
