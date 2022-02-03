#!/bin/bash

MAIN=main
#Se selecciona donde se pondran los resultados
DIRECORIO_CSV=resultados/res.csv
DIRECTORIO_MAIN_A_RESULTADOS=../..
DIRECTORIO_MAIN_A_CSV=${DIRECTORIO_MAIN_A_RESULTADOS}/${DIRECORIO_CSV}
#Donde se escriben nombres de todos los archivos en texto
ARCHIVO_IMAGENES=imagenes.txt

#ingresar al directorio del programa
cd programa/datos

make
echo "Programa creado"

echo "Obteniendo datos"
echo "Esto podrìa tardar unos segundos..."
echo ""

touch ${ARCHIVO_IMAGENES}
for filename in ../../imagenes/*.pgm; do
    echo "Comprimiendo: ${filename}"
    echo "$filename" >> ${ARCHIVO_IMAGENES}
done

echo "Imagenes leidas"

#Se corre compresor
./${MAIN} "${ARCHIVO_IMAGENES}" "${DIRECTORIO_MAIN_A_CSV}"

rm ${ARCHIVO_IMAGENES}

echo "Obtencion de datos finalizada"
echo "Puede encontrar los resultados en: ${DIRECORIO_CSV}"
