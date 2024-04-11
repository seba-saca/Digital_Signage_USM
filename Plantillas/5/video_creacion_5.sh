#!/bin/bash

# User name
user_name=seba

# Numero Plantilla
plantilla_number=5

# Overlay
fondo=fondo_usm_5.jpg

# Logo
logo=elo.jpg

# Rutas
lista_videos="/home/$user_name/Desktop/Digital_Signage_USM/Contenido/Plantillas/$plantilla_number/video_lista_$plantilla_number.txt"

overlay="/home/$user_name/Desktop/Digital_Signage_USM/Contenido/Imagenes/Overlays/$fondo"

logo="/home/$user_name/Desktop/Digital_Signage_USM/Contenido/Imagenes/Logos/$logo"

titulares="/home/$user_name/Desktop/Digital_Signage_USM/Contenido/Plantillas/$plantilla_number/video_titulares_$plantilla_number.ass"

# Parametros
VBR="3000K"
FPS="24"
BUFSIZE="10000K"
QUAL="ultrafast"
AUDIO_ENCODER="aac"

# Script ffmpeg 
ffmpeg -i $overlay -f concat -i $lista_videos -i $logo \
-c:v libx264 -profile:v baseline -preset $QUAL \
-r $FPS -b:v $VBR -bufsize $BUFSIZE -maxrate $VBR \
-c:a $AUDIO_ENCODER -ar 44100 -b:a 128k -pix_fmt yuv420p \
-filter_complex "[0:v]scale=1920:1080[fondo]; \
[1:v]scale=iw*0.418:ih*0.41[scale_main_video]; \
[fondo][scale_main_video]overlay=x=100:y=318[pepe];\
[2:v]scale=iw*0.5:ih*0.5[scaled_logo];\
[pepe][scaled_logo]overlay=1410:120,ass='$titulares'" \
-f matroska - | ffplay -fs -autoexit -