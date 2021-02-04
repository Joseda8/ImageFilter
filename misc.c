#ifndef MISC
#define MISC

#include <stdio.h>  
#include <stdlib.h>
#include <string.h>   //strlen  
#include <time.h>
#include "config_file.h"
#include "filters.h"

int createFiles(){
	FILE *fp = fopen("Red", "r");
	if (!fp) {
		system("mkdir Red");
		system("mkdir Green");
		system("mkdir Blue");
		system("mkdir Median");
		system("mkdir Mean");
		return 1;
	}
	return 0;
}

char* substring (const char* input, int offset, int len, char* dest)
{
  int input_len = strlen (input);

  
  if (offset + len > input_len)
  {
    return NULL;
  }

  strncpy (dest, input + offset, len);

  return dest;
}

void make_img_conversion() {

  char *filename = "img.ppm";
  char convert_cmd[26] = "convert img.jpg ";
  char mov_file[128] = "mv img.ppm ";

  strcat(convert_cmd, filename);
  //strcat(mov_file, getenv("IMG_PATH"));

  system(convert_cmd);
  //system(mov_file);
}

void filters(){
    int size_filter = 1024; //Tamaño máximo de una línea en el config.txt
    char img_path_median[size_filter];
    char img_path_mean[size_filter];
    char colors_path[size_filter];

    char* img_to_filter = "img.ppm";

    strcpy(img_path_median, path_str(size_filter, "DirLog:")); //img_path_str es el método para obtener la ruta
    strcpy(img_path_mean, path_str(size_filter, "DirLog:")); //img_path_str es el método para obtener la ruta
    strcpy(colors_path, path_str(size_filter, "DirColores:")); //img_path_str es el método para obtener la ruta

    //Parámetros: Imagen que se recibe y dirección en que se guarda
    write_log("Applying Median Filter");
    median_filter_main(img_to_filter, &img_path_median);
    write_log("Applying Mean Filter");
    mean_filter(img_to_filter, &img_path_mean);
    write_log("Sorting image by color");
    color_sorter(img_to_filter, &colors_path);

    char delete_file[128] = "rm -rf ";
    strcat(delete_file, img_to_filter);
    system(delete_file);

    system("rm -rf img.jpg");
    system("rm -rf img.ppm");

    write_log("Process Finished");
}

#endif
