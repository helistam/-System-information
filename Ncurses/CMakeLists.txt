cmake_minimum_required(VERSION 3.25)
project(Ncurses C)

set(CMAKE_C_STANDARD 11)

add_executable(Ncurses main.c OS.c OS.h Hardware.h Hardware.c Network.h Network.c USB.h USB.c)
