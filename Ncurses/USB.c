//
// Created by user on 13.5.23.
//
#include "USB.h"
void USB_location(WINDOW* win2)
{
    DIR *dir;
    int stroka=1;
    struct dirent *ent;
    char path[MAX_PATH];
    char manufacturer[MAX_PATH];
    char product[MAX_PATH];
    char serial[MAX_PATH];
    FILE *file;
    dir = opendir("/sys/bus/usb/devices");
    if (dir == NULL) {
        perror("Ошибка: Невозможно открыть каталог /sys/bus/usb/devices/");
        exit(EXIT_FAILURE);
    }
    mvwprintw(win2, stroka, 1, "Device\tManufacturer\t\t\t\t\tModel\t\tSerial number\n");
    while ((ent = readdir(dir)) != NULL) {
        if (strncmp(ent->d_name, ".", 1) == 0 || strncmp(ent->d_name, "..", 2) == 0) {
            continue;
        }

        snprintf(path, MAX_PATH, "/sys/bus/usb/devices/%s/manufacturer", ent->d_name);
        file = fopen(path, "r");
        if (file != NULL) {
            fgets(manufacturer, MAX_PATH, file);
            fclose(file);
            if (strlen(manufacturer) > 0) {
                // Remove trailing newline
                manufacturer[strcspn(manufacturer, "\n")] = 0;
            } else {
                continue;
            }
        } else {
            continue;
        }

        snprintf(path, MAX_PATH, "/sys/bus/usb/devices/%s/product", ent->d_name);
        file = fopen(path, "r");
        if (file != NULL) {
            fgets(product, MAX_PATH, file);
            fclose(file);
            if (strlen(product) > 0) {
                // Remove trailing newline
                product[strcspn(product, "\n")] = 0;
            } else {
                snprintf(serial, MAX_PATH, "Unknown");
            }
        } else {
            snprintf(serial, MAX_PATH, "Unknown");
        }

        snprintf(path, MAX_PATH, "/sys/bus/usb/devices/%s/serial", ent->d_name);
        file = fopen(path, "r");
        if (file != NULL) {
            fgets(serial, MAX_PATH, file);
            fclose(file);
            if (strlen(serial) > 0) {
                // Remove trailing newline
                serial[strcspn(serial, "\n")] = 0;
            } else {
                snprintf(serial, MAX_PATH, "Unknown");
            }
        } else {
            snprintf(serial, MAX_PATH, "Unknown");
        }
        stroka++;
        mvwprintw(win2, stroka, 1, "%s\t%s\t%s\t%s\n", ent->d_name, manufacturer, product, serial);
    }

    closedir(dir);
}
void USB_info()
{
    int size_of_options = 2; // кол-во элементов в массиве options
    char *options[] = {"EXIT","Update info"};
    int input=0,max_y,max_x,choice=0,c=0;
    getmaxyx(stdscr, max_y, max_x);
    WINDOW* win1 = subwin(stdscr, max_y, max_x / 2-30, 0, 0);
    WINDOW* win2 = subwin(stdscr, max_y, max_x / 2+30, 0, max_x / 2-30);

// Рисуем границы окон
    box(win1, 0, 0);
    box(win2, 0, 0);
    wprintw(win1, "Selectable area");
    wprintw(win2, "Data");
    USB_location(win2);
    wrefresh(win2);
    do {
        box(win1, 0, 0);
        box(win2, 0, 0);

        wrefresh(win1);
        wrefresh(win2);
        for (int i = 0; i < size_of_options; i++) {
            mvwprintw(win1, i+1, 1, "%s", options[i]);
        }
        wrefresh(win1);

        switch(c) {
            case KEY_UP:
                choice++;
                if(choice>1)
                    choice=0;
                break;
            case KEY_DOWN:
                choice--;
                if(choice<0)choice=1;
                break;
            case '\n':
                if (choice == 0) {
                    wclear(stdscr);
                    return;
                }
                if (choice == 1) {
                    wclear(win2);
                    USB_location(win2);
                    box(win2, 0, 0);
                    wrefresh(win1);
                    wrefresh(win2);
                }
                break;
        }
        wattron(win1, A_REVERSE);
        mvwprintw(win1, choice+1, 1, "%s", options[choice]);
        wattroff(win1, A_REVERSE);
        wrefresh(win1);
    } while ((c=getch())!=27);



}