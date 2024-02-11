//
// Created by user on 13.5.23.
//
#include "Hardware.h"
void CPU_Info(WINDOW* win2)
{
    long double a[4], b[4], loadavg;
    FILE *fps;
    char dump[50];

    fps = fopen("/proc/loadavg", "r");
    fscanf(fps, "%Lf %Lf %Lf %s %Lf", &a[0], &a[1], &a[2], dump, &a[3]);
    fclose(fps);

    int num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    for (int i = 0; i < 3; i++) {
        b[i] = (a[i] / num_cpus) * 100;
    }

    loadavg = b[0];
    mvwprintw(win2, 6, 1, "CPU Working: %.2Lf%%\n", loadavg);


    struct utsname buffer1; //архитектура процессора
    if (uname(&buffer1) != 0) {
        perror("uname");
    }
    long num_cores = sysconf(_SC_NPROCESSORS_ONLN);//колич ядер
    int nprocs = get_nprocs();//колич процессоров
    int max_x,max_y;
    getmaxyx(win2, max_y, max_x);
    char *vendor_id = NULL;
    char buffer[256];
    char cpu_model[100],cpu_print[100];
    FILE *fp;
    fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL) {
        printf("Failed to read CPU information.\n");

    }

    while (fgets(cpu_model, 100, fp) != NULL) {
        if (sscanf(cpu_model, "model name : %[^\n]", cpu_model) == 1) {
            break;
        }
    }
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, "vendor_id")) {
            vendor_id = strchr(buffer, ':') + 2;
            break;
        }
    }

    fclose(fp);
    int mid_y = max_y / 2;
    int mid_x = max_x / 2;

    // выводим текст в левую часть окна
    mvwprintw(win2, 1, 1, "CPU Model:");
    mvwprintw(win2, 1, mid_x-15, "%s", cpu_model);
    mvwprintw(win2, 2, 1, "Manufacturer:");
    mvwprintw(win2, 2, mid_x-15, "%s", vendor_id);
    mvwprintw(win2, 3, 1,"Machine: \n");
    mvwprintw(win2, 3, mid_x-15, "%s", buffer1.machine);
    mvwprintw(win2, 4, 1,"Yadra: \n");
    mvwprintw(win2, 4, mid_x-15, "%ld", num_cores);
    mvwprintw(win2, 5, 1,"Koll proc: \n");
    mvwprintw(win2, 5, mid_x-15, "%d", nprocs);
    wrefresh(win2);
}
void Audio_info(WINDOW* win2)
{


    FILE *fp;
    char buf[1024];

    fp = fopen("/proc/asound/cards", "r");
    if (fp == NULL) {
        mvwprintw(win2, 11, 1, "Ошибка: Невозможно открыть файл /proc/asound/cards.\n");
        return ;
    }
    int i=1;
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        mvwprintw(win2, i, 1, "%s", buf);
        i++;
    }

    fclose(fp);
    wrefresh(win2);
}
void Get_Video_Info(WINDOW* win2)
{
    FILE *fp;
    char path[1035];
    char vendor[1035];
    char model[1035];

    fp = fopen("/sys/class/drm/card0/device/vendor", "r");
    if (fp == NULL) {
        printf("Failed to open vendor file\n");
        exit(1);
    }

    fgets(vendor, sizeof(vendor)-1, fp);

    fclose(fp);

    fp = fopen("/sys/class/drm/card0/device/label", "r");
    if (fp == NULL) {
        printf("Failed to open model file\n");
        exit(1);
    }

    fgets(model, sizeof(model)-1, fp);
    fclose(fp);

    char value[10];

    fp = fopen("/sys/class/drm/card0/device/max_link_speed", "r");
    if (fp == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    if (fgets(value, sizeof(value), fp) == NULL) {
        perror("Failed to read file");
        exit(EXIT_FAILURE);
    }

    fclose(fp);

    char status[10];
    fp = fopen("/sys/class/drm/card0/device/power_state", "r");
    if (fp == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    if (fgets(status, sizeof(status), fp) == NULL) {
        perror("Failed to read file");
        exit(EXIT_FAILURE);
    }

    fclose(fp);

    mvwprintw(win2, 1, 1, "GPU Vendor: %s", vendor);
    mvwprintw(win2, 2, 1, "GPU Model: %s", model);
    mvwprintw(win2, 3, 1, "Data transfer rate : %s", value);
    mvwprintw(win2, 4, 1, "Status: %s", status);
}
void Battary_Info(WINDOW* win2)
{
    FILE *fp;
    char type[20];
    int capacity;
    int full;
    fp = fopen("/sys/class/power_supply/BAT0/capacity", "r");
    fscanf(fp, "%d", &capacity);
    fclose(fp);
    fp = fopen("/sys/class/power_supply/BAT0/energy_full", "r");
    if (fp == NULL) {
        printf("Unable to open file\n");

    }
    fscanf(fp, "%d", &full);
    fclose(fp);


    fp = fopen("/sys/class/power_supply/BAT0/technology", "r");
    if (fp == NULL) {
        printf("Unable to open file\n");

    }
    fscanf(fp, "%s", type);
    fclose(fp);


    fp = fopen("/sys/class/power_supply/BAT0/capacity", "r");
    fscanf(fp, "%d", &capacity);
    fclose(fp);
    fp = fopen("/sys/class/power_supply/BAT0/energy_full", "r");
    if (fp == NULL) {
        printf("Unable to open file\n");

    }
    fscanf(fp, "%d", &full);
    fclose(fp);
    char status[20];
    fp = fopen("/sys/class/power_supply/BAT0/status", "r");
    if (fp == NULL) {
        printf("Unable to open file\n");
    }

    fscanf(fp, "%s", status);
    mvwprintw(win2, 1, 1,"Battery full capacity: %d mAh\n", full/1000);
    mvwprintw(win2, 2, 1,"Battery status: %s \n", status);
    mvwprintw(win2, 3, 1, "Battery level: %d%%\n", capacity);
    mvwprintw(win2, 4, 1,"Technology: %s\n", type);

}
void monitor_info(WINDOW* win2)
{
    Display *display = XOpenDisplay(NULL);
    int screen = DefaultScreen(display);
    int width = DisplayWidth(display, screen);
    int height = DisplayHeight(display, screen);
    int mm_width = DisplayWidthMM(display, screen);
    int mm_height = DisplayHeightMM(display, screen);


    mvwprintw(win2, 1, 1, "Resolution: %dx%d pixels\n", width, height);
    mvwprintw(win2, 2, 1, "Size: %dx%d mm\n", mm_width, mm_height);

    XCloseDisplay(display);
}
int Hardware_Info(WINDOW* win2)
{

    FILE* meminfo = fopen("/proc/meminfo", "r");
    if (meminfo == NULL) {
        perror("Cannot open /proc/meminfo");
        return EXIT_FAILURE;
    }

    char line[256];
    unsigned long long total_mem_kb = 0, free_mem_kb = 0, available_mem_kb = 0;
    while (fgets(line, sizeof(line), meminfo)) {
        if (sscanf(line, "MemTotal: %llu kB", &total_mem_kb) == 1) {
            continue;
        }
        if (sscanf(line, "MemFree: %llu kB", &free_mem_kb) == 1) {
            continue;
        }
        if (sscanf(line, "MemAvailable: %llu kB", &available_mem_kb) == 1) {
            continue;
        }
    }

    fclose(meminfo);

    mvwprintw(win2, 5, 1, "Total RAM memory: %llu GB\n", total_mem_kb / 1024 / 1024);

    mvwprintw(win2, 6, 1,"Free RAM memory: %llu GB\n", free_mem_kb / 1024 / 1024);

    mvwprintw(win2, 7, 1,"Available RAM memory: %llu GB\n", available_mem_kb / 1024 / 1024);

    int max_y,max_x;
    getmaxyx(win2, max_y, max_x);
    int mid_y = max_y / 2;
    int mid_x = max_x / 2;
    struct statvfs stat;
    if (statvfs("/", &stat) != 0) {
        printf("Ошибка при получении информации о файловой системе\n");
        return 1;
    }
    if (statvfs("/", &stat) == 0) {
        long long block_size = stat.f_bsize;
        long long total_blocks = stat.f_blocks;
        long long free_blocks = stat.f_bfree;
        long long disk_size = total_blocks * block_size;

        double total_gb = (double)disk_size / 1000000000;
        double free_gb = (double)(free_blocks * block_size) / 1000000000;
        mvwprintw(win2, 1, 1, "Total disk size: %.2f GB\n", total_gb);
        mvwprintw(win2, 2, 1, "Free disk space: %.2f GB\n", free_gb);
    } else {
        mvwprintw(win2, 3, 1, "Unable to get file system stats.");
    }

}