#include "../include/compresor.h"
#include "../include/compresorIO.h"
#include "../include/pixelio.h"

#include <stdio.h>

void comprimir( char* archivoEntrada, char* archivoSalida, int s, Modalidad modalidad ) {
    FILE * archivoComprimido;
    FILE * archivoDescomprimido;
    int anchoImagen;
    int kGolomb;
    int ultimoCaracterLeido;        /* Promoción temporal de x a entero        */
    unsigned char x;
    unsigned char xPrediccion;      /* x^                                      */
    int errorPrediccion;            /* e = x - x^                              */
    Extracto * fExtracto;           /* f(C)                                    */
    unsigned char a,b,c,d;          /* Contexto                                */
    int mapeoRice;
    int cantidadBitsImpresos;       /* Cantidad de bits impresos por iteración */
    int output;                     /* Salida al archivo descomprimido         */
    
    archivoComprimido = fopen(archivoSalida, "wb");
    archivoOriginal = fopen(archivoEntrada, "rb");
    
    escribirParametrosCabezal(archivoComprimido, s, modalidad);
    escribirCabezalPGM(archivoOriginal, archivoComprimido, &anchoImagen);
    inicializarExtractos(s);
    inicializarBuffer(anchoImagen);
    if (modalidad == RUN) {
        ultimoCaracterLeido = obtenerUltimoCaracter(archivoOriginal);
        while (ultimoCaracterLeido != EOF) {
            x = (unsigned char) ultimoCaracterLeido;
            determinarContexto(&a, &b, &c, &d);
            xPrediccion = predecirX(a, b, c);
            fExtracto = determinarExtracto(xPrediccion, a, b, c, s);
            errorPrediccion = x - xPrediccion;
            if (a!=b || b!=c || c!=d) {    /* No es modo de run */
                kGolomb = determinarGolombK(fExtracto);
                mapeoRice = determinarMapeoRice(errorPrediccion);
                largoGolombBinario = determinarLargoBinaryGolomb(kGolomb, mapeoRice, &golombBinario);
                largoGolombUnario = determinarLargoUnaryGolomb(kGolomb, mapeoRice);
                actualizarExtracto(fExtracto,errorPrediccion);
                ultimoCaracterLeido = obtenerUltimoCaracter(archivoOriginal);
            }
            else {
                int l = -1;    /* Ocurrencias del mismo caracter */
                kGolomb = 3;
                determinarContexto(&a, &b, &c, &d);

                do {
                    l++;
                    actualizarExtracto(fExtracto, errorPrediccion);
                    ultimoCaracterLeido = obtenerUltimoCaracter(archivoOriginal);
                    x = (unsigned char) ultimoCaracterLeido;
                } while (ultimoCaracterLeido != EOF && !esFinDeLinea() && x == a);
                largoGolombBinario = determinarLargoBinaryGolomb(kGolomb, l, &golombBinario);
                largoGolombUnario = determinarLargoUnaryGolomb(kGolomb, l);
            }
            imprimirCompresion(golombBinario,largoGolombBinario,largoGolombUnario,archivoComprimido);
        }
    }
    else {
        while ((ultimoCaracterLeido = obtenerUltimoCaracter()) != EOF) {
            x = (unsigned char)ultimoCaracterLeido;
            determinarContexto(&a, &b, &c, &d);
            xPrediccion = predecirX(a, b, c);
            fExtracto = determinarExtracto(xPrediccion,a , b, c, s);
            errorPrediccion = x - xPrediccion;
            kGolomb = determinarGolombK(fExtracto);
            mapeoRice = determinarMapeoRice(errorPrediccion, fExtracto);
            determinarGolomb(kGolomb, mapeoRice, &cantidadBitsImpresos, &output);
            actualizarExtracto(fExtracto, errorPrediccion);
            actualizarBuffer(output, cantidadBitsImpresos, archivoComprimido);
        }
    }
    liberarExtractos();
    vaciarBuffer(archivoComprimido);
    destruirBuffer();
    fclose(archivoComprimido);
    fclose(archivoDescomprimido);
}
