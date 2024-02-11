//
// Created by user on 13.5.23.
//

#ifndef NCURSES_HARDWARE_H
#define NCURSES_HARDWARE_H
#include <ncurses.h>
#include <X11/Xlib.h>
#include <stdlib.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <string.h>
void Audio_info(WINDOW* win2);
void Get_Video_Info(WINDOW* win2);
void CPU_Info(WINDOW* win2);
void Battary_Info(WINDOW* win2);
void monitor_info(WINDOW* win2);
int Hardware_Info(WINDOW* win2);
#endif //NCURSES_HARDWARE_H
