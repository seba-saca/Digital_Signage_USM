#!/bin/bash

# User name
user_name=seba

# Numero Plantilla
plantilla_number=1

# Directorio donde se encuentran los videos
video_dir="/home/$user_name/Desktop/Contenido/Videos/Material_Prueba"

# Comprobamos si el directorio de videos existe
if [ ! -d "$video_dir" ]; then
    echo "Error: El directorio de videos no existe."
    exit 1
fi

# Lista de archivos en el directorio
files=("$video_dir"/*)

# Mostrar lista de archivos al usuario
echo "Archivos disponibles para seleccionar:"
for ((i=0; i<${#files[@]}; i++)); do
    echo "$i: $(basename ${files[i]})"
done

# Preguntar al usuario qué archivos desea seleccionar
echo "Ingrese los números de los archivos que desea seleccionar (separados por espacios):"
read -r selection

# Crear archivo de texto
output_file="/home/$user_name/Desktop/Digital_Signage_USM/Plantillas/$plantilla_number/video_lista_$plantilla_number.txt"
> "$output_file"
echo -n "" > "$output_file"

# Escribir nombres de archivos seleccionados en el archivo de texto
for index in $selection; do
    echo "file '${files[index]}'" >> "$output_file"
done

echo "Se han escrito los nombres de los archivos seleccionados en $output_file."

