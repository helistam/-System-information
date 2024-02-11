//
// Created by user on 13.5.23.
//
#include "OS.h"
void OS_info(WINDOW *win4)
{
    struct utsname buffer1;
    uname(&buffer1);//заполнение buffer1;
    mvwprintw(win4, 1, 1, "OS name: %s", buffer1.sysname);
    mvwprintw(win4, 2, 1, "Network host name: %s", buffer1.machine);
    mvwprintw(win4, 3, 1, "Kernel version number: %s", buffer1.nodename);
    mvwprintw(win4, 4, 1, "Kernel version description: %s", buffer1.release);
    mvwprintw(win4, 5, 1, "Hardware architecture: %s", buffer1.version);
    wrefresh(win4);
}
void GetUser(WINDOW* win, int max_x, int max_y) {
    struct passwd *pw;
    int row = 3;
    int start_col = 1;
    int name_col = 5;
    int dir_col = 30;
    int uid_col = 60;
    int gid_col = 70;
    int end_col = max_x - 2;
    int num_users = 0;

    // Выводим заголовки столбцов
    mvwprintw(win, 1, start_col, "User List:");
    mvwprintw(win, 2, name_col, "User Name");
    mvwprintw(win, 2, dir_col, "Home Directory");
    mvwprintw(win, 2, uid_col, "UID");
    mvwprintw(win, 2, gid_col, "GID");
    keypad(win, TRUE);
    // Выводим список пользователей
    while ((pw = getpwent()) != NULL && row <= max_y - 2) {
        num_users++;
        mvwprintw(win, row, name_col, "%s", pw->pw_name);
        mvwprintw(win, row, dir_col, "%s", pw->pw_dir);
        mvwprintw(win, row, uid_col, "%d", pw->pw_uid);
        mvwprintw(win, row, gid_col, "%d", pw->pw_gid);

        row++;

        // Если дошли до конца экрана, выводим стрелки для прокрутки
        if (row >= max_y - 2) {
            mvwprintw(win, max_y - 1, start_col, "Press UP/DOWN arrows to scroll");
            int ch;
            while (ch != KEY_UP && ch != KEY_DOWN) {
                ch = wgetch(win);
            }
            if (ch == KEY_UP) {
                row -= num_users + 1;
                if (row < 3) {
                    row = 3;
                }
            } else if (ch == KEY_DOWN) {
                row = 3;
                num_users = 0;
            }
            wclear(win);
            box(win, 0, 0);
            mvwprintw(win, 1, start_col, "User List:");
            mvwprintw(win, 2, name_col, "User Name");
            mvwprintw(win, 2, dir_col, "Home Directory");
            mvwprintw(win, 2, uid_col, "UID");
            mvwprintw(win, 2, gid_col, "GID");
        }
    }

    // Закрываем поток паролей
    endpwent();
}
void input_devices(WINDOW*win4)
{
    DIR *dir = opendir("/sys/devices/virtual/input");
    int i=1;
    if (dir) {
        struct dirent *ent;
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_name[0] != '.') { // пропускаем скрытые файлы и директории
                char path[1024];
                snprintf(path, sizeof(path), "/sys/devices/virtual/input/%s/name", ent->d_name); // создаем путь к файлу с именем устройства
                FILE *f = fopen(path, "r");
                if (f) {
                    char name[1024];
                    fgets(name, sizeof(name), f); // читаем имя устройства
                    mvwprintw(win4, i++, 1, "DEVICE: %s\n", name);
                    fclose(f);
                }
            }
        }
        closedir(dir);
    } else {
        mvwprintw(win4, 11, 1, "Can't open this dirictory: /sys/devices/virtual/input\n");
        return ;
    }

}