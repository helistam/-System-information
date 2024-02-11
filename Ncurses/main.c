#include <ncurses.h>
#include <stdio.h>
#include <locale.h>
#include <unistd.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include "OS.h"
#include "Hardware.h"
#include "Network.h"
#include "USB.h"
#include <pthread.h>
#include <errno.h>
#include <sys/ioctl.h>
pthread_t thread;

void Information_panel(WINDOW*wins,int max_x,int max_y)
{

    mvwprintw(wins,1,1,"ESC-Exit");
    mvwprintw(wins,1,15,"^ -Page up");
    mvwprintw(wins,1,max_x/2,"V- Page down");
    mvwprintw(wins,1,max_x-20,"Enter-Choose");
    box(wins, 0, 0);
    wrefresh(wins);
}

void Network_menu()
{
    int size_of_sub_options,choice=0;
    char *sub_options[] = {"Network adapters","Exit"};
    int input=0,max_y,max_x;
    size_of_sub_options = sizeof(sub_options) / sizeof(char *);
    getmaxyx(stdscr, max_y, max_x);
    WINDOW* win1 = subwin(stdscr, max_y, max_x / 2-10, 0, 0);
    WINDOW* win2 = subwin(stdscr, max_y, max_x / 2+10, 0, max_x / 2-10);
    // Рисуем границы окон
    box(win1, 0, 0);
    box(win2, 0, 0);
    wprintw(win1, "Selectable area");
    wprintw(win2, "Data");
    // обновляем экран
    refresh();

    WINDOW* wins = subwin(stdscr, 3,max_x,max_y-3, 0);
    Information_panel(wins,max_x,max_y);

    for (int i = 0; i < size_of_sub_options; i++) {
        mvwprintw(win1, i+1, 1, "%s", sub_options[i]);
    }wrefresh(win1);
    do {
        wrefresh(win1);
        for (int i = 0; i < size_of_sub_options; i++) {
            mvwprintw(win1, i+1, 1, "%s", sub_options[i]);
        }wrefresh(win1);

        switch(input) {
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
                // вывод сообщения
                wclear(win2);
                mvwprintw(wins,1,max_x/2,"%c- Page down",24);
                mvwprintw(wins,1,max_x-20,"Enter-Choose");
                box(wins, 0, 0);
                wrefresh(wins);
                box(win2, 0, 0);
                wprintw(win2, "Data");
                if(choice==0)Network_Adapters(win2);
                if(choice==1) return;
                wrefresh(win2);
                getch();
                break;
        }
        wattron(win1, A_REVERSE);
        mvwprintw(win1, choice+1, 1, "%s", sub_options[choice]);
        wattroff(win1, A_REVERSE);
        wrefresh(win1);
        rewind(stdin);

    } while ((input=getch())!=27);

}

void OS_menu()
{
    char *os_options[] = {"OS", "Users",     "Input devices"};
    int input=0,max_y,max_x,choice=0;
    int size_of_os_options = sizeof(os_options) / sizeof(char *);
    getmaxyx(stdscr, max_y, max_x);
    WINDOW* win3 = subwin(stdscr, max_y, max_x / 2-10, 0, 0);
    WINDOW* win4 = subwin(stdscr, max_y, max_x / 2+10, 0, max_x / 2-10);
    box(win3, 0, 0);
    box(win4, 0, 0);
    wprintw(win3, "Selectable area");
    wprintw(win4, "Data");
    WINDOW* wins = subwin(stdscr, 3,max_x,max_y-3, 0);
    Information_panel(wins,max_x,max_y);
    refresh();

    for (int i = 0; i < size_of_os_options; i++) {
        mvwprintw(win3, i+1, 1, "%s", os_options[i]);
    }wrefresh(win3);

    do {
        Information_panel(wins,max_x,max_y);
        wrefresh(win3);
        for (int i = 0; i < size_of_os_options; i++) {
            mvwprintw(win3, i+1, 1, "%s", os_options[i]);
        }wrefresh(win3);

        switch(input) {
            case KEY_UP:
                choice++;
                if(choice>2)
                    choice=0;
                break;
            case KEY_DOWN:
                choice--;
                if(choice<0)choice=2;
                break;
            case '\n':
                // вывод сообщения
                wclear(win4);
                box(win4, 0, 0);
                wprintw(win4, "Data");
                if(choice==0) {
                    OS_info(win4);
                    Information_panel(wins,max_x,max_y);
                }
                if(choice==1)GetUser(win4,max_x,max_y);
                if(choice==2) {
                    Information_panel(wins,max_x,max_y);
                    input_devices(win4);
                }
                wrefresh(win4);
                getch();
                break;
        }
        wattron(win3, A_REVERSE);
        mvwprintw(win3, choice+1, 1, "%s", os_options[choice]);
        wattroff(win3, A_REVERSE);
        wrefresh(win3);
        rewind(stdin);

    } while ((input=getch())!=27);



}
void submenu(int current_choice)
{
    int size_of_sub_options,choice=0;
    char *sub_options[] = {"Processor", "Hard Disk", "Monitor","Battery","Video card","Micropone"};
    int input=0,max_y,max_x;
    size_of_sub_options = sizeof(sub_options) / sizeof(char *);
    getmaxyx(stdscr, max_y, max_x);
    WINDOW* win1 = subwin(stdscr, max_y, max_x / 2-10, 0, 0);
    WINDOW* win2 = subwin(stdscr, max_y, max_x / 2+10, 0, max_x / 2-10);

    WINDOW* wins = subwin(stdscr, 3,max_x,max_y-3, 0);
    Information_panel(wins,max_x,max_y);

    // Рисуем границы окон
    box(win1, 0, 0);
    box(win2, 0, 0);
    wprintw(win1, "Selectable area");
    wprintw(win2, "Data");
    // обновляем экран
    refresh();

    for (int i = 0; i < size_of_sub_options; i++) {
        mvwprintw(win1, i+1, 1, "%s", sub_options[i]);
    }wrefresh(win1);
    do {
        wrefresh(win1);
        for (int i = 0; i < size_of_sub_options; i++) {
            mvwprintw(win1, i+1, 1, "%s", sub_options[i]);
        }wrefresh(win1);

        switch(input) {
            case KEY_UP:
                choice++;
                if(choice>5)
                    choice=0;
                break;
            case KEY_DOWN:
                choice--;
                if(choice<0)choice=5;
                break;
            case '\n':
                // вывод сообщения
                wclear(win2);
                mvwprintw(wins,1,max_x/2,"V- Page down");
                mvwprintw(wins,1,max_x-20,"Enter-Choose");
                box(wins, 0, 0);
                wrefresh(wins);
                box(win2, 0, 0);
                wprintw(win2, "Data");
                if(choice==0)CPU_Info(win2);
                if(choice==1) Hardware_Info( win2);
                if(choice==2)monitor_info(win2);
                if(choice==3)Battary_Info(win2);
                if(choice==4)Get_Video_Info(win2);
                if(choice==5)Audio_info(win2);
               // mvwprintw(win2, 1,1, "You chose %s", sub_options[choice]);
                wrefresh(win2);
                getch();
                break;
        }
        wattron(win1, A_REVERSE);
        mvwprintw(win1, choice+1, 1, "%s", sub_options[choice]);
        wattroff(win1, A_REVERSE);
        wrefresh(win1);
        rewind(stdin);

    } while ((input=getch())!=27);

}

void* print_up_time(void* arg) {
    initscr();
    sleep(1);
    cbreak();//не ожидать пробел
    keypad(stdscr,1);//allow arrows
    noecho();//dont display input
    curs_set(0);
    WINDOW* menu_win = (WINDOW*)arg;
    struct sysinfo info;
    int max_x,max_y;
    getmaxyx(menu_win, max_y, max_x);
    while (1) {
        if (sysinfo(&info) != 0) {
            wprintw(menu_win, "Error: Failed to get system information");
        } else {
            int uptime = info.uptime;
            int hours = uptime / 3600;
            int minutes = (uptime % 3600) / 60;
            int seconds = uptime % 60;


            mvwprintw(menu_win, 1, max_y+30, " Uptime: %d:%02d:%02d", hours, minutes, seconds);
        }

        wrefresh(menu_win);

        sleep(1);
    }
    endwin();
    pthread_exit(NULL);
}
void cancelThread() {
   pthread_cancel(thread); // Отменить выполнение потока
    pthread_join(thread, NULL); // Дождаться завершения потока
    endwin();
}
void designFunctions( WINDOW *menu_win)
{
    int max_y, max_x,size_of_main_menu=0,metka=0;
    int choice = 0;
    getmaxyx(menu_win, max_y, max_x);
    box(menu_win, 0, 0);
    wrefresh(menu_win);
    int mid_x = max_x / 2;           // середина экрана по горизонтали
    int mid_y = max_y / 2;           // середина экрана по вертикали
    char *options[5] = {"Hardware", "PO", "Network","USB","Update Info"};
    size_of_main_menu=sizeof(options) / sizeof(char*);
    int c=0;
    WINDOW* wins = subwin(menu_win, 3,max_x,max_y-3, 0);
    Information_panel(wins,max_x,max_y);
    int status = pthread_create(&thread, NULL, print_up_time, (void*) menu_win);
    if (status == EAGAIN) {
        fprintf(stderr, "The system lacked the necessary resources to create a thread\n");
        return;
    } else if (status == EPERM) {
        fprintf(stderr,
                "The caller doesn't have appropriate permission to set the required scheduling parameters or policy\n");
        return;
    } else if (status == EINVAL) {
        fprintf(stderr, "The value specified by attr is invalid\n");
        return;
    }
    do {

        box(menu_win, 0, 0);
        wrefresh(menu_win);
        for (int i = 0; i <5; i++) {
            mvwprintw(menu_win, i+1, 1, "%s", options[i]);
        }wrefresh(menu_win);

        switch(c) {
            case KEY_UP:
                choice++;
                if(choice>4)
                    choice=0;
                break;
            case KEY_DOWN:
                choice--;
                if(choice<0)choice=4;
                break;
            case '\n':
                // вывод сообщения
                clear();
                cancelThread();
                if(choice==0) {
                    submenu(choice);
                    metka=1;
                }
                if(choice==1)
                {
                    OS_menu();
                    metka = 1;
                }
                if(choice==2)
                {
                    Network_menu();
                    metka=1;
                }
                if(choice==3) {
                    USB_info();
                    metka = 1;
                }
                if(choice==4) {
                    getmaxyx(menu_win, max_y, max_x);
                    pthread_create(&thread, NULL, print_up_time, (void*) menu_win);
                    metka=1;
                    clear();
                }
                break;
        }
        if(metka==1)
        {
            wclear(menu_win);
            clear();
            box(menu_win, 0, 0);
            refresh();
            wrefresh(menu_win);
            for (int i = 0; i < 5; i++) {
                mvwprintw(menu_win, i+1, 1, "%s", options[i]);
            }wrefresh(menu_win);
            metka=0;

            mvwprintw(menu_win,max_y-2,1,"ESC-Exit");
            mvwprintw(menu_win,max_y-2,15,"^ -Page up");
            mvwprintw(menu_win,max_y-2,max_x/2,"V- Page down");
            mvwprintw(menu_win,max_y-2,max_x-20,"Enter-Choose");
            wrefresh(menu_win);

        }
        wattron(menu_win, A_REVERSE);
        mvwprintw(menu_win, choice+1, 1, "%s", options[choice]);
        wattroff(menu_win, A_REVERSE);
        wrefresh(menu_win);

    } while ((c=getch())!=27);



}
int main()
{
    char a;
    int c=0;
    int max_y, max_x;
    int y=11,x=11;
    initscr();// Переход в curses-реж
    getmaxyx(stdscr, max_y, max_x);
    WINDOW *menu_win = newwin(max_y-1, max_x-1, 0, 0);
    cbreak();//не ожидать пробел
    keypad(stdscr,1);//allow arrows
    noecho();//dont display input
    curs_set(0);
    refresh();
    designFunctions(menu_win);// Ожидание нажатия какой-либо клавиши пользователем
    refresh();
    endwin();                    // Выход из curses-режима. Обязательная команда.
    return 0;
}