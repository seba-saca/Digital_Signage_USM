#!/bin/bash

# ======================================================================
# Script para generar un video con sobreposición de textos y logos usando ffmpeg.
#
# Sintaxis de uso:
# ./video.sh <admin_name> <name_output>
#
# Parámetros:
# 1. admin_name   - Nombre de usuario administrador en la máquina local.
# 2. name_output  - Nombre del archivo de salida para el video generado.
#
# Ejemplo:
# ./video.sh usuario_administrador nombre_video_salida
#
# Firma: Sebastian Castillo
# ======================================================================

# Parámetros
admin_name=$1                                   # Nombre de usuario administrador
parametro_plantilla=Plantilla_1                 # Plantilla utilizada para la configuración
name_output=$2                                  # Nombre del archivo de salida
path_general="/home/$admin_name/Desktop/Contenido_ELO308/Plantillas/$parametro_plantilla"
playlist="$path_general/Videos.txt"             # Archivo con la lista de videos a concatenar
title_grande="$path_general/Titular_Grande.txt" # Archivo con los textos grandes a mostrar
title_chico="$path_general/Titular_Pequeno.txt" # Archivo con los textos pequeños a mostrar
fondo="/home/$admin_name/Desktop/Digital_Signage_USM/Plantillas/Imagenes/Overlays/$parametro_plantilla/fondo.jpg"
logo_elo="/home/$admin_name/Desktop/Digital_Signage_USM/Plantillas/Imagenes/Logos/elo.jpg"
accion_video=2                                  # Acción predeterminada (1 para tiempo real, 2 para diferido con titular)
ruta_video_final="/home/$admin_name/Desktop/Contenido_ELO308/Videos" # Ruta para guardar el video final

# Inicializar inicio y fin de los tiempos para sobreposición de textos
inicio=0
fin=5

# Parámetros de configuración para ffmpeg
VBR="3000K"          # Tasa de bits del video
FPS="24"             # Cuadros por segundo
BUFSIZE="10000K"     # Tamaño del buffer de video
QUAL="ultrafast"     # Calidad del preset de codificación
AUDIO_ENCODER="aac"  # Codificador de audio

# Iniciar la cadena de filtros para ffmpeg
filtro_base=""

# Filtros iniciales para escalar y sobreponer el video principal y el logo
filtro_base+="[0:v]scale=1920:1080[fondo];\
[1:v]scale=iw*0.59:ih*0.41[scale_main_video];\
[fondo][scale_main_video]overlay=x=155:y=280[xy_video_main];\
[2:v]scale=iw*0.5:ih*0.5[scaled_logo];\
[xy_video_main][scaled_logo]overlay=1410:120"

# Iniciar la cadena completa de filtros incluyendo los textos
filtro_completo=""

# Agregar la base de filtros
filtro_completo+="$filtro_base,"

# Leer y aplicar los textos pequeños desde el archivo Titular_Pequeno.txt
while IFS= read -r archivo_txt 
do
    filtro_completo+="drawtext=textfile='${archivo_txt}':x=150:y=830:fontsize=48:fontcolor=white:fontfile=path/to/font.ttf:shadowcolor=gray@0.5:shadowx=2:shadowy=2:enable='between(t,${inicio},${fin})',"
    inicio=$((inicio + 5))
    fin=$((fin + 5))
done < "$title_chico"

# Reiniciar inicio y fin para los textos grandes
inicio=0
fin=5

# Leer y aplicar los textos grandes desde el archivo Titular_Grande.txt
while IFS= read -r archivo_txt
do
    filtro_completo+="drawtext=textfile='${archivo_txt}':x=1380:y=280:fontsize=40:fontcolor=white:fontfile=path/to/font.ttf:shadowcolor=gray@0.5:shadowx=2:shadowy=2:enable='between(t,${inicio},${fin})',"
    inicio=$((inicio + 5))
    fin=$((fin + 5))
done < "$title_grande"

# Eliminar la última coma de la cadena de filtros
filtro_completo="${filtro_completo%?}"

# Menú de selección de acción
echo "Seleccione acción"
echo "Opción 1: Tiempo real"
echo "Opción 2: Diferido con titular"

# Asignar la opción seleccionada
opcion=$accion_video

case $opcion in
    1)
        # Procesar video en tiempo real y reproducir
        ffmpeg -i $fondo -f concat -safe 0 -i $playlist -i $logo_elo \
        -c:v libx264 -profile:v baseline -preset $QUAL \
        -r $FPS -b:v $VBR -bufsize $BUFSIZE -maxrate $VBR \
        -c:a $AUDIO_ENCODER -ar 44100 -b:a 128k -pix_fmt yuv420p \
        -filter_complex "$filtro_completo" \
        -f matroska - | ffplay -fs -autoexit -
        ;;
    2)
        # Procesar video y guardar en archivo
        yes | ffmpeg -i $fondo -f concat -safe 0 -i $playlist -i $logo_elo \
        -c:v libx264 -profile:v baseline -preset $QUAL \
        -r $FPS -b:v $VBR -bufsize $BUFSIZE -maxrate $VBR \
        -c:a $AUDIO_ENCODER -ar 44100 -b:a 128k -pix_fmt yuv420p \
        -filter_complex "$filtro_completo" \
        "$ruta_video_final/$name_output.mkv"
        ;;
    *)
        # Opción inválida
        echo "Opción inválida. Por favor, seleccione 1 o 2."
        ;;
esac

# Generar una miniatura del video final
yes | ffmpeg -i "$ruta_video_final/$name_output.mkv" -vf "thumbnail,scale=720:480" -frames:v 1 "/home/$admin_name/Desktop/Contenido_ELO308/miniaturas/$name_output.jpg"
