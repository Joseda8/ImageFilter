#ifndef CONFIG_FILE_H_INCLUDED
#define CONFIG_FILE_H_INCLUDED
#include "util.h"

//->

/*
Se obtiene el valor de una llave en el config.conf
@param size Tamaño máximo de la línea en el config.conf
@param sub_str Llave buscada
@return img_path Valor correspondiente a la llave buscada
*/
char* path_str(int size, char* sub_str){
    char row[size];
    char img_path[size];
    FILE *config_file;

    config_file = fopen("config.conf", "r");

    while(fgets(row, size, (FILE*) config_file)) {
        if(check_subStr(row, sub_str) == 1){
            strcpy(img_path, _substr(row, length(sub_str) + 1, size));
            fclose(config_file);
            return strdup(_substr(img_path, 0, length(img_path)-1));
        }
    }
    return 0;
}

#endif
