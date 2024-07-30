#!/bin/bash

# Ruta del video original
video_original=$1

# Ruta de destino para copiar el video
ruta_destino=$2

# Nombre del video de salida
name_output=$3

# Parametros
VBR="3000K"
FPS="24"
BUFSIZE="10000K"
QUAL="ultrafast"
AUDIO_ENCODER="aac"

yes | ffmpeg -i $video_original \
            -c:v libx264 -profile:v baseline -preset $QUAL \
            -r $FPS -b:v $VBR -bufsize $BUFSIZE -maxrate $VBR \
            -c:a $AUDIO_ENCODER -ar 44100 -b:a 128k -pix_fmt yuv420p \
            "$name_output.mkv"
            