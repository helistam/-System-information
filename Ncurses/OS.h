//
// Created by user on 13.5.23.
//
#include <ncurses.h>
#include <sys/utsname.h>
#ifndef NCURSES_OS_H
#define NCURSES_OS_H
#include <pwd.h>
#include <dirent.h>
void OS_info(WINDOW *win4);
void GetUser(WINDOW* win, int max_x, int max_y) ;
void input_devices(WINDOW*win4);
#endif //NCURSES_OS_H
