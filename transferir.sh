#!/bin/bash

# Variables
SOURCE_DIR=$1  # Directorio fuente donde están los archivos
DEST_DIR=$2  # Directorio destino en el dispositivo remoto
DEST_USER=$3  # Usuario en el dispositivo remoto
DEST_HOST=$4  # Dirección IP o nombre de host del dispositivo remoto
FILES_LIST=$5  # Ruta del archivo con la lista de archivos
PATH_CONTROL_DEVICE=$6  # Ruta del archivo de control en el dispositivo local
PATH_DESTINO_SINCRO=$7

# Verificar si el archivo de lista existe
echo "Ruta del archivo: $FILES_LIST"
if [ ! -f "$FILES_LIST" ]; then
    echo "El archivo $FILES_LIST no existe."
    exit 1
else
    echo "El archivo $FILES_LIST sí existe."
fi

# Verificar si el directorio de destino existe en el dispositivo remoto y crearlo si no existe
ssh "$DEST_USER@$DEST_HOST" "mkdir -p $DEST_DIR"
if [ $? -ne 0 ]; then
    echo "No se pudo crear el directorio de destino $DEST_DIR en el dispositivo remoto."
    exit 1
fi

# Imprimir el contenido del archivo
echo "Contenido del archivo:"
cat "$FILES_LIST"

# Leer el archivo de lista de archivos y buscar cada archivo en el directorio fuente
while IFS= read -r filename; do
    # Buscar archivos que coincidan con el nombre en el directorio fuente
    found_files=$(find "$SOURCE_DIR" -maxdepth 1 -type f -name "$filename.*" -o -name "$filename")
    
    # Si se encuentran archivos, sincronizar cada uno de ellos
    if [ -n "$found_files" ]; then
        for file in $found_files; do
            rsync -avr --delete "$file" "$DEST_USER@$DEST_HOST:$DEST_DIR"
        done
    else
        echo "No se encontraron archivos para: $filename"
    fi
done < "$FILES_LIST"

rsync -avr --delete "$PATH_CONTROL_DEVICE" "$DEST_USER@$DEST_HOST:$PATH_DESTINO_SINCRO"