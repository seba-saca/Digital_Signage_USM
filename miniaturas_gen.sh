#!/bin/bash

# Carpeta donde están ubicados los videos
VIDEO_DIR="/home/seba/Desktop/Contenido_ELO308/videos"
# Carpeta donde se guardarán las imágenes generadas (puede ser la misma que VIDEO_DIR)
IMAGE_DIR="/home/seba/Desktop/Contenido_ELO308/miniaturas"

# Asegúrate de que la carpeta de imágenes exista
mkdir -p "$IMAGE_DIR"

# Recorre todos los archivos de video en la carpeta
for video in "$VIDEO_DIR"/*; do
    # Verifica que sea un archivo
    if [ -f "$video" ]; then
        # Extrae el nombre del archivo sin la extensión
        filename=$(basename "$video")
        name="${filename%.*}"
        
        # Genera la imagen .jpg usando ffmpeg
        ffmpeg -i "$video" -vf "thumbnail,scale=720:480" -frames:v 1 "$IMAGE_DIR/$name.jpg"
    fi
done

echo "¡Imágenes generadas exitosamente!"
