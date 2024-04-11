#!/bin/bash

# User name
user_name=seba

# Plantilla
plantilla_name=Plantilla_1

# Directorio donde se encuentran los videos
video_dir="/home/$user_name/Desktop/Digital_Signage_USM/Contenido/Plantillas/$plantilla_name"

# Archivo donde se guardará la lista de videos
lista_videos="/home/$user_name/Desktop/Digital_Signage_USM/Contenido/Plantillas/$plantilla_name/video_lista_1.txt"

# Comprobamos si el directorio de videos existe
if [ ! -d "$video_dir" ]; then
    echo "Error: El directorio de videos no existe."
    exit 1
fi

# Limpiar el archivo de lista de videos si ya existe
> "$lista_videos"

# Listar archivos de video ubicados en el directorio
archivos_videos=$(find "$video_dir" -type f \( -iname "*.mp4" -o -iname "*.mkv" -o -iname "*.avi" \))

#Iterar sobre cada archivo
while IFS= read -r video ; do
    echo "file '$(basename "$video")'">>"$lista_videos"
done <<< "$archivos_videos"
