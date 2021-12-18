/* 
 * TAREA LOCO ATIPI - NTI/FING/UDELAR
 * 
 * Autores:
 * 	Alexis Baladón
 * 	Juan Ignacio Fernandez
 */

/* 
 * main.c
 *
 * Para correr el archivo 'main.c' desde la consola, usar:
 * ./main -d/[c modalidad] -r/n -[s valorS] inputFile outputFile
 * 
 * Ejemplos:
 *   ./main -c 1 -s 10 "imagen.pgm" "imagen.bin"
 *   ./main -d -s 10 "imagen.bin" "imagen.pgm"
 *
 * Donde:
 * -c <number> habilita la compresión,
 * si recibe 1 comprime en modo de run,
 * si recibe un 0 lo hace en modo normal.
 * -d habilita la compresión
 * -s <number>: parámetro s del sesgo
 * -n habilita la compr
 * inputFile: archivo de entrada
 * outputFile: archivo de salida
 */

#include "include/descompresor.h"
#include "include/compresor.h"

#include <stdlib.h>
#include <stdio.h>

#ifndef bool
    #define bool char
#endif

typedef enum { COMPRIMIR, DESCOMPRIMIR } Funcionalidad;

typedef struct {
    char            *archivoEntrada;
    char            *archivoSalida;
    int             s;                  /* 0 <= s <= 10 */
    bool            run;
} Parametros;

/*
 * Declaraciones para rutinas locales
 */
void abortar( char* mensaje );

int main( int argc, char* argv[] ) {
    /*
     * Por defecto los parámetros del programa son los siguientes:
     *     (funcionalidad,s,run) = (Comprimir,0,false)
     * En caso de no presentarse nombres de archivos ya sean
     * de entrada como de salida se devolverá error.
     */
    Funcionalidad funcionalidad = COMPRIMIR;
    Parametros parametros = {NULL,NULL,0,0};
    
    int i = 1;
    while( i < argc ) {
        if ( argv[i][0] == '-' ) {
            switch ( argv[i][1] ) {
                case 's':
                    i++;
                    if ( sscanf(argv[i], "%d", &(parametros.s)) != 1 ) {
                        abortar("Valor de 's' invalido.");
                    }
                    i++;
                    break;
                case 'c':
                    funcionalidad = COMPRIMIR;
                    i++;
                    int run;
                    if ( sscanf(argv[i], "%d", &(run)) != 1 ) {
                        parametros.run = 0;
                        i--;
                    }
                    else if (run != 0 && run != 1) {
                        abortar("Valor de modalidad inválido.");
                    }
                    else {
                        parametros.run = run;
                    }
                    i++;
                    break;
                case 'd':
                    funcionalidad = DESCOMPRIMIR;
                    i++;
                    break;
                default:
                    abortar("Flag invalida");
                    break;
            }
        } else {
            if( parametros.archivoEntrada == NULL ) {
                parametros.archivoEntrada = argv[i];
                i++;
            } else if ( parametros.archivoSalida == NULL ) {
                parametros.archivoSalida = argv[i];
                i++;
            } else {
                abortar("Demasiados parametros.");
            }
        }
    }
    
    /*
     * Control de precondiciones
     */
    if( parametros.archivoEntrada == NULL ) {
        abortar("Ubicacion de archivo de entrada no especificada.");
    }
    else {
        FILE * f = NULL;
        if ((f = fopen(parametros.archivoEntrada,"rb")) == NULL) {
            abortar("problema al abrir archivo de entrada.");
        }
        fclose(f);
    }
    if (parametros.archivoSalida == NULL) {
        abortar("Ubicacion de archivo de salida no especificada.");
    }
    if (parametros.s < 0 || parametros.s > 10) {
        abortar("el parametro 's' debe estar entre 0 y 10.");
    }
    
    /*
     * Inicio de compresión/descompresión
     */
    
    puts("\n Iniciando programa...");
    if (funcionalidad == COMPRIMIR) {
        comprimir(parametros.archivoEntrada,parametros.archivoSalida,
        parametros.s,parametros.run);
    }
    else {
        descomprimir(parametros.archivoEntrada,parametros.archivoSalida);
    }
    return EXIT_SUCCESS;
}

/*
 * Rutina para errores
*/
void abortar( char *mensaje ) {
    puts("Error: ");
    puts(mensaje);
    puts("\n ");
    exit(EXIT_FAILURE);
}

