//
// Created by user on 13.5.23.
//
#ifndef NCURSES_NETWORK_H
#define NCURSES_NETWORK_H
#include <ncurses.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <stdlib.h>
#include <string.h>

void Network_Adapters(WINDOW* win2);
#endif //NCURSES_NETWORK_H
