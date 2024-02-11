//
// Created by user on 13.5.23.
//
#include "Network.h"

void Network_Adapters(WINDOW* win2)
{



    struct ifaddrs *ifaddr, *ifa;
    int n = 0;
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }
    int str=1;
    // Создаем множество для хранения уникальных имен интерфейсов
    typedef struct { char name[IFNAMSIZ]; } interface;
    interface interfaces[10];
    int num_interfaces = 0;
    mvwprintw(win2, str, 1, "Interface");
    mvwprintw(win2, str, 30, "Status");
    mvwprintw(win2, str, 42, "MAC Address");
    str++;

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;

        // Проверяем, есть ли интерфейс с таким именем в множестве
        int interface_found = 0;
        for (int i = 0; i < num_interfaces; i++) {
            if (strcmp(interfaces[i].name, ifa->ifa_name) == 0) {
                interface_found = 1;
                break;
            }
        }

        if (interface_found) {
            continue;
        }

        // Добавляем имя интерфейса в множество
        strcpy(interfaces[num_interfaces].name, ifa->ifa_name);
        num_interfaces++;

        if (ifa->ifa_flags & IFF_UP) {
            mvwprintw(win2, str, 1, "%d:  %s \n", ++n, ifa->ifa_name);
            mvwprintw(win2, str, 30, "up");
            // Получаем MAC-адрес с помощью функции ether_ntoa из библиотеки net/ethernet.h
            struct ether_addr *mac;
            mac = (struct ether_addr*)ifa->ifa_addr->sa_data;
            mvwprintw(win2, str, 42, "%s\n", ether_ntoa(mac));
            str++;
        } else {
            mvwprintw(win2, str, 1, "%d:  %s \n", ++n, ifa->ifa_name);
            mvwprintw(win2, str, 30, "down");
            str++;
        }
    }

    freeifaddrs(ifaddr);
}