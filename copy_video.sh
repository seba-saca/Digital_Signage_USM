#!/bin/bash

# ======================================================================
# Script para codificar un video usando ffmpeg con parámetros específicos y guardarlo en un destino dado.
#
# Sintaxis de uso:
# ./copy_video.sh <video_original> <ruta_destino> <name_output>
#
# Parámetros:
# 1. video_original - Ruta del video original.
# 2. ruta_destino   - Ruta de destino para guardar el video codificado.
# 3. name_output    - Nombre del video de salida (sin extensión).
#
# Ejemplo:
# ./copy_video.sh /path/to/original/video.mp4 /path/to/destination/ output_name
#
# Firma: Sebastian Castillo
# ======================================================================

# Variables
video_original=$1       # Ruta del video original
ruta_destino=$2         # Ruta de destino para copiar el video
name_output=$3          # Nombre del video de salida

# Parámetros de codificación
VBR="3000K"             # Tasa de bits de video
FPS="24"                # Cuadros por segundo
BUFSIZE="10000K"        # Tamaño del buffer de video
QUAL="ultrafast"        # Configuración de la velocidad de codificación
AUDIO_ENCODER="aac"     # Codificador de audio

# Ejecutar el comando ffmpeg para codificar el video
yes | ffmpeg -i "$video_original" \
            -c:v libx264 -profile:v baseline -preset $QUAL \
            -r $FPS -b:v $VBR -bufsize $BUFSIZE -maxrate $VBR \
            -c:a $AUDIO_ENCODER -ar 44100 -b:a 128k -pix_fmt yuv420p \
            "$ruta_destino/$name_output.mkv"
