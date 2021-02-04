#ifndef FILTERS_H_INCLUDED
#define FILTERS_H_INCLUDED

#ifndef MEDIAN_FILTER_H_INCLUDED
#define MEDIAN_FILTER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "util.h"



typedef struct { unsigned char r, g, b; } rgb_t;
typedef struct {
	int w, h;
	rgb_t **pix;
} image_t, *image;

typedef struct {
	int r[256], g[256], b[256];
	int n;
} color_histo_t;

int write_ppm(image im, char *fn)
{
	FILE *fp = fopen(fn, "w");
	if (!fp) return 0;
	fprintf(fp, "P6\n%d %d\n255\n", im->w, im->h);
	fwrite(im->pix[0], 1, sizeof(rgb_t) * im->w * im->h, fp);
	fclose(fp);
	return 1;
}

image img_new(int w, int h)
{
	int i;
	image im = malloc(sizeof(image_t) + h * sizeof(rgb_t*)
			+ sizeof(rgb_t) * w * h);
	im->w = w; im->h = h;
	im->pix = (rgb_t**)(im + 1);
	for (im->pix[0] = (rgb_t*)(im->pix + h), i = 1; i < h; i++)
		im->pix[i] = im->pix[i - 1] + w;
	return im;
}

int read_num(FILE *f)
{
	int n;
	while (!fscanf(f, "%d ", &n)) {
		if ((n = fgetc(f)) == '#') {
			while ((n = fgetc(f)) != '\n')
				if (n == EOF) break;
			if (n == '\n') continue;
		} else return 0;
	}
	return n;
}

image read_ppm(char *fn)
{
	FILE *fp = fopen(fn, "r");
	int w, h, maxval;
	image im = 0;
	if (!fp){
		printf("There is not image\n");
		return 0;
	}

	if (fgetc(fp) != 'P' || fgetc(fp) != '6' || !isspace(fgetc(fp)))
		goto bail;

	w = read_num(fp);
	h = read_num(fp);
	maxval = read_num(fp);
	if (!w || !h || !maxval) goto bail;

	im = img_new(w, h);
	fread(im->pix[0], 1, sizeof(rgb_t) * w * h, fp);
bail:
	if (fp) fclose(fp);
	return im;
}

void del_pixels(image im, int row, int col, int size, color_histo_t *h)
{
	int i;
	rgb_t *pix;

	if (col < 0 || col >= im->w) return;
	for (i = row - size; i <= row + size && i < im->h; i++) {
		if (i < 0) continue;
		pix = im->pix[i] + col;
		h->r[pix->r]--;
		h->g[pix->g]--;
		h->b[pix->b]--;
		h->n--;
	}
}

void add_pixels(image im, int row, int col, int size, color_histo_t *h)
{
	int i;
	rgb_t *pix;

	if (col < 0 || col >= im->w) return;
	for (i = row - size; i <= row + size && i < im->h; i++) {
		if (i < 0) continue;
		pix = im->pix[i] + col;
		h->r[pix->r]++;
		h->g[pix->g]++;
		h->b[pix->b]++;
		h->n++;
	}
}

void init_histo(image im, int row, int size, color_histo_t*h)
{
	int j;

	memset(h, 0, sizeof(color_histo_t));

	for (j = 0; j < size && j < im->w; j++)
		add_pixels(im, row, j, size, h);
}

int median(const int *x, int n)
{
	int i;
	for (n /= 2, i = 0; i < 256 && (n -= x[i]) > 0; i++);
	return i;
}

void median_color(rgb_t *pix, const color_histo_t *h)
{
	pix->r = median(h->r, h->n);
	pix->g = median(h->g, h->n);
	pix->b = median(h->b, h->n);
}

image median_filter(image in, int size){
	int row, col;
	image out = img_new(in->w, in->h);
	color_histo_t h;

	for (row = 0; row < in->h; row ++) {
		for (col = 0; col < in->w; col++) {
			if (!col) init_histo(in, row, size, &h);
			else {
				del_pixels(in, row, col - size, size, &h);
				add_pixels(in, row, col + size, size, &h);
			}
			median_color(out->pix[row] + col, &h);
		}
	}

	return out;
}


void median_filter_main(char* in_filename, char* out_filename){
    int size = 1;
	image in, out;

	if (size < 0) size = 3;

	in = read_ppm(in_filename);
	out = median_filter(in, size);

    char conversion_name[5] = {0}; 
    size_t new_name_len = 5;
    rand_string(conversion_name, new_name_len);
    char *rand_name = create_filename(conversion_name);

    strcat(rand_name, "median");
    strcat(out_filename, "Median/");
    strcat(out_filename, rand_name);

    write_ppm(out, out_filename);
    free(in);
    free(out);
    free(rand_name);
}


void mean_filter(char* in_filename, char* out_filename){

    image in, out;

    in = read_ppm(in_filename);
    out = img_new(in->w, in->h);

    char conversion_name[5] = {0}; 
    size_t new_name_len = 5;
    rand_string(conversion_name, new_name_len);
    char *rand_name = create_filename(conversion_name);

    strcat(rand_name, "mean");
    strcat(out_filename, "Mean/");
    strcat(out_filename, rand_name);

    int r[9];
    int g[9];
    int b[9];

    for (int row = 0; row < in->h; row ++) {
	for (int col = 0; col < in->w; col++) {

        get_pixel_value(in->pix[row]+col, &r[4], &g[4], &b[4]);

        if(row-1==-1 && col-1==-1){
            r[0] = 0; g[0] = 0; b[0] = 0;
            r[1] = 0; g[1] = 0; b[1] = 0;
            r[2] = 0; g[2] = 0; b[2] = 0;
            r[3] = 0; g[3] = 0; b[3] = 0;
            r[6] = 0; g[6] = 0; b[6] = 0;
            get_pixel_value(in->pix[row]+col+1, &r[5], &g[5], &b[5]);
            get_pixel_value(in->pix[row+1]+col, &r[7], &g[7], &b[7]);
            get_pixel_value(in->pix[row+1]+col+1, &r[8], &g[8], &b[8]);
        } else if(row-1==-1 && col-1!=-1 && col+1<in->w){
            r[0] = 0; g[0] = 0; b[0] = 0;
            r[1] = 0; g[1] = 0; b[1] = 0;
            r[2] = 0; g[2] = 0; b[2] = 0;
            get_pixel_value(in->pix[row]+col-1, &r[3], &g[3], &b[3]);
            get_pixel_value(in->pix[row]+col+1, &r[5], &g[5], &b[5]);
            get_pixel_value(in->pix[row+1]+col-1, &r[6], &g[6], &b[6]);
            get_pixel_value(in->pix[row+1]+col, &r[7], &g[7], &b[7]);
            get_pixel_value(in->pix[row+1]+col+1, &r[8], &g[8], &b[8]);
        } else if(row-1==-1 && col+1==in->w){
            r[0] = 0; g[0] = 0; b[0] = 0;
            r[1] = 0; g[1] = 0; b[1] = 0;
            r[2] = 0; g[2] = 0; b[2] = 0;
            r[5] = 0; g[5] = 0; b[5] = 0;
            r[8] = 0; g[8] = 0; b[8] = 0;
            get_pixel_value(in->pix[row]+col-1, &r[3], &g[3], &b[3]);
            get_pixel_value(in->pix[row+1]+col-1, &r[6], &g[6], &b[6]);
            get_pixel_value(in->pix[row+1]+col, &r[7], &g[7], &b[7]);
        } else if(row-1!=-1 && row+1<in->h && col-1==-1){
            r[0] = 0; g[0] = 0; b[0] = 0;
            r[3] = 0; g[3] = 0; b[3] = 0;
            r[6] = 0; g[6] = 0; b[6] = 0;
            get_pixel_value(in->pix[row-1]+col, &r[1], &g[1], &b[1]);
            get_pixel_value(in->pix[row-1]+col+1, &r[2], &g[2], &b[2]);
            get_pixel_value(in->pix[row]+col+1, &r[5], &g[5], &b[5]);
            get_pixel_value(in->pix[row+1]+col, &r[7], &g[7], &b[7]);
            get_pixel_value(in->pix[row+1]+col+1, &r[8], &g[8], &b[8]);
        } else if(row-1!=-1 && row+1<in->h && col-1!=-1 && col+1<in->w){
            get_pixel_value(in->pix[row-1]+col-1, &r[0], &g[0], &b[0]);
            get_pixel_value(in->pix[row-1]+col, &r[1], &g[1], &b[1]);
            get_pixel_value(in->pix[row-1]+col+1, &r[2], &g[2], &b[2]);
            get_pixel_value(in->pix[row]+col-1, &r[3], &g[3], &b[3]);
            get_pixel_value(in->pix[row]+col+1, &r[5], &g[5], &b[5]);
            get_pixel_value(in->pix[row+1]+col-1, &r[6], &g[6], &b[6]);
            get_pixel_value(in->pix[row+1]+col, &r[7], &g[7], &b[7]);
            get_pixel_value(in->pix[row+1]+col+1, &r[8], &g[8], &b[8]);
        } else if(row-1!=-1 && row+1<in->h && col+1==in->w){
            r[2] = 0; g[2] = 0; b[2] = 0;
            r[5] = 0; g[5] = 0; b[5] = 0;
            r[8] = 0; g[8] = 0; b[8] = 0;
            get_pixel_value(in->pix[row-1]+col-1, &r[0], &g[0], &b[0]);
            get_pixel_value(in->pix[row-1]+col, &r[1], &g[1], &b[1]);
            get_pixel_value(in->pix[row]+col-1, &r[3], &g[3], &b[3]);
            get_pixel_value(in->pix[row+1]+col-1, &r[6], &g[6], &b[6]);
            get_pixel_value(in->pix[row+1]+col, &r[7], &g[7], &b[7]);
        } else if(row+1==in->h && col-1==-1){
            r[0] = 0; g[0] = 0; b[0] = 0;
            r[3] = 0; g[3] = 0; b[3] = 0;
            r[6] = 0; g[6] = 0; b[6] = 0;
            r[7] = 0; g[7] = 0; b[7] = 0;
            r[8] = 0; g[8] = 0; b[8] = 0;
            get_pixel_value(in->pix[row-1]+col, &r[1], &g[1], &b[1]);
            get_pixel_value(in->pix[row-1]+col+1, &r[2], &g[2], &b[2]);
            get_pixel_value(in->pix[row]+col+1, &r[5], &g[5], &b[5]);
        } else if(row+1==in->h && col-1!=-1 && col+1<in->w){
            r[6] = 0; g[6] = 0; b[6] = 0;
            r[7] = 0; g[7] = 0; b[7] = 0;
            r[8] = 0; g[8] = 0; b[8] = 0;
            get_pixel_value(in->pix[row-1]+col-1, &r[0], &g[0], &b[0]);
            get_pixel_value(in->pix[row-1]+col, &r[1], &g[1], &b[1]);
            get_pixel_value(in->pix[row-1]+col+1, &r[2], &g[2], &b[2]);
            get_pixel_value(in->pix[row]+col-1, &r[3], &g[3], &b[3]);
            get_pixel_value(in->pix[row]+col+1, &r[5], &g[5], &b[5]);
        } else if(row+1==in->h && col+1==in->w){
            r[2] = 0; g[2] = 0; b[2] = 0;
            r[5] = 0; g[5] = 0; b[5] = 0;
            r[6] = 0; g[6] = 0; b[6] = 0;
            r[7] = 0; g[7] = 0; b[7] = 0;
            r[8] = 0; g[8] = 0; b[8] = 0;
            get_pixel_value(in->pix[row-1]+col-1, &r[0], &g[0], &b[0]);
            get_pixel_value(in->pix[row-1]+col, &r[1], &g[1], &b[1]);
            get_pixel_value(in->pix[row]+col-1, &r[3], &g[3], &b[3]);
            }

	int red_duplicate = 9-total_duplicate(r, 9, 0);
	int green_duplicate = 9-total_duplicate(g, 9, 0);
	int blue_duplicate = 9-total_duplicate(b, 9, 0);

	if(red_duplicate == 0){
	    red_duplicate=1;
	}
	if(green_duplicate == 0){
	    green_duplicate=1;
	} 
	if(blue_duplicate == 0){
	    blue_duplicate=1;
	}

        int new_red_pixel = suma_array(r, 9)/red_duplicate;
        int new_green_pixel = suma_array(g, 9)/green_duplicate;
        int new_blue_pixel = suma_array(b, 9)/blue_duplicate;

        mean_color(out->pix[row] + col, new_red_pixel, new_green_pixel, new_blue_pixel);

        }
    }

    write_ppm(out, out_filename);
    free(in);
    free(out);
    free(rand_name);
}


void mean_color(rgb_t *pix, int new_r, int new_g, int new_b){
	pix->r = new_r;
	pix->g = new_g;
	pix->b = new_b;
}



void get_pixel_value(rgb_t *pix, int *r, int *g, int *b){
    *r = pix->r;
    *g = pix->g;
    *b = pix->b;
}


void plus_rgb(rgb_t *pix, int *r, int *g, int *b){
    *r += pix->r;
    *g += pix->g;
    *b += pix->b;
}

void color_sorter(char* in_filename, char* out_filename){
    image in;

    in = read_ppm(in_filename);

    int r=0,g=0,b=0;

    for (int row = 0; row < in->h; row ++) {
	for (int col = 0; col < in->w; col++) {
            plus_rgb(in->pix[row]+col, &r, &g, &b);
        }
	}

	if(r>=g && r>=b){
        char color[4] = "Red/";
        strcat(out_filename, color);
	}else if(g>r && g>=b){
        char color[6] = "Green/";
        strcat(out_filename, color);
	}else if(b>g && b>r){
        char color[5] = "Blue/";
        strcat(out_filename, color);
	}

    char conversion_name[5] = {0}; 
    size_t new_name_len = 5;
    rand_string(conversion_name, new_name_len);
    char *rand_name = create_filename(conversion_name);
    
    strcat(rand_name, "sorted");
    strcat(out_filename, rand_name);

    write_ppm(in, out_filename);
    free(in);
    free(rand_name);

}


#endif // MEDIAN_FILTER_H_INCLUDED


#endif // FILTERS_H_INCLUDED
