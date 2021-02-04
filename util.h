#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

//->

struct arrChar{
    char string[9];
};

/*
Obtiene una subcadena
@param cadena Cadena a la que se le aplica la operación
@param inicio Posición desde la que se toma la subcadena
@param longitud Cuantos caracteres de la cadena se toman
@return subcadena
*/
char *_substr(const char *cadena, size_t inicio, size_t longitud){
   char *subcadena, *aux;
   size_t i, j;

   if (longitud == 0)
      return NULL;

   for (i = 0; i < inicio && cadena[i] != '\0'; i++)
      ;
   if (cadena[i] == '\0')
      return NULL;

   if ((subcadena = malloc(longitud + 1)) == NULL)
      return NULL;

   for (j = 0; j < longitud; j++)
      if ((subcadena[j] = cadena[i + j]) == '\0')
         break;
   subcadena[j] = '\0';

   if ((aux = realloc(subcadena, j + 1)) == NULL){
      free(subcadena);
      return NULL;
   }else
      subcadena = aux;

   return subcadena;
}

/*
Revisa si una subcadena está contenida en una cadena
@param cadena Cadena a la que se le aplica la operación
@param subcadena Subcadena a revisar
@return True/False
*/
int check_subStr( char *cadena, char *subcadena )
{
   char *tmp = cadena;
   char *pdest;

   pdest = strstr( tmp, subcadena );
   if( pdest ) return 1;

   return 0;
}

/*
Posición de la última aparición de un carácter en una cadena
@param cadena Cadena a la que se le aplica la operación
@param charac Caracter a buscar
@return pos
*/
int last_position(const char *cadena, char charac){
    char * pch;
    pch=strchr(cadena, charac);
    int pos;
    while (pch!=NULL){
        pos = pch-cadena+1;
        pch=strchr(pch+1,'s');
    }
    return pos;
}

/*
Largo de una cadena
@param cadena Cadena a la que se aplicará la operacin
return largo Largo de la cadena
*/
int length(char cadena[])
{
	int largo=0;
	while (cadena[largo]!='\0'){
        largo++;
    }
	return largo;
}

/*
Cantidad de apariciones de un número en un array
@param array Array al que se le aplica la operación
@param len_array Largo del array
@param num Número a buscar
@return n Apariciones del número buscado
*/
int total_duplicate(int array[], int len_array, int num){
    int n = 0;
    for(int i = 0; i<len_array; i++){
        if(array[i]==num){
            n++;
        }
    }

    return n;
}

/*
Suma de los valores de un array
@param array Array al que se le aplica la operación
@param len_array Largo del array
@return suma Resultado de la suma
*/
int suma_array(int array[], int len_array){

    int suma = 0;
    for (int i=0; i<len_array; i++){
        suma += array[i];
        }
    return suma;
}

static void rand_string(char *dest, size_t length)
{
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

char *create_filename(char *filename) {
  char *extension = ".ppm";
  char *full_name;
  //full_name = malloc(strlen(filename) + 1 + 4);
  full_name = malloc(strlen(filename) + 1);
  strcpy(full_name, filename);
  //strcat(full_name, extension);
  return full_name;
}

#endif
