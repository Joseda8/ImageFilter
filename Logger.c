#include <stdio.h> 
#include <time.h>
#include <string.h>

#define LOG_FILE_NAME "ImageServerLog.txt"

void create_log_file(char* log_dir) {
    char full_dir[300] = {0};
    strcat(full_dir, log_dir);
    strcat(full_dir, LOG_FILE_NAME);
    FILE* file = fopen(full_dir, "w");
    fclose(file);
}

char* get_current_time() {
    time_t curr_time;
    time(&curr_time);
    return ctime(&curr_time);
}

void write_log(char* message) {
    char compl_msg[256] = {0};
    FILE* fptr = fopen(LOG_FILE_NAME, "a");
    strcat(compl_msg, get_current_time());
    strcat(compl_msg, " - ");
    strcat(compl_msg, message);
    fprintf(fptr, compl_msg);
    fprintf(fptr, "\n");
    fclose(fptr);
}
 