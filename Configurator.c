#include <stdio.h>
#include <string.h>
#include "ConfStruct.c"

struct Conf get_conf(){

    char read_data;

    const char file_name[] = "config.conf";

    FILE *fp;

    struct Conf config;

    fp = fopen(file_name, "r");

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    char line[256];

    int line_nmbr = 0;

    while(fgets(line, 256, fp) != NULL)
    {
        char key[50], val[256];

        line_nmbr++;

        if(sscanf(line, "%s %s", key, val) != 2)
        {
                fprintf(stderr, "Syntax error, line %d\n", line_nmbr);
                continue;
        }

        if(line_nmbr == 1) config.port = atoi(val);
        else if(line_nmbr == 2) strcpy(config.dir_colors, val);
        else if(line_nmbr == 3) strcpy(config.dir_histo, val);
        else strcpy(config.dir_log, val);
    }

    fclose(fp);

    return config;
}
