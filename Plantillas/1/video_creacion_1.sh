#!/bin/bash

# User name
user_name=seba

# Numero Plantilla
plantilla_number=1

# Overlay
fondo=fondo_usm_1.jpg

# Logo
logo=elo.jpg

# Rutas
lista_videos="/home/$user_name/Desktop/Digital_Signage_USM/Plantillas/$plantilla_number/video_lista_$plantilla_number.txt"

overlay="/home/$user_name/Desktop/Contenido/Imagenes/Overlays/$fondo"

logo="/home/$user_name/Desktop/Contenido/Imagenes/Logos/$logo"

titulares="/home/$user_name/Desktop/Digital_Signage_USM/Plantillas/$plantilla_number/video_titulares_$plantilla_number.ass"

ruta_video_final="/home/$user_name/Desktop/Digital_Signage_USM/Plantillas/$plantilla_number"

# Parametros
VBR="3000K"
FPS="24"
BUFSIZE="10000K"
QUAL="ultrafast"
AUDIO_ENCODER="aac"

echo "Seleccione accion"
echo "Opcion 1: Tiempo real"
echo "Opcion 2: Diferido"

read -p "Opción seleccionada: " opcion

    case $opcion in
        1)
            ffmpeg -i $overlay -f concat -safe 0 -i $lista_videos -i $logo \
            -c:v libx264 -profile:v baseline -preset $QUAL \
            -r $FPS -b:v $VBR -bufsize $BUFSIZE -maxrate $VBR \
            -c:a $AUDIO_ENCODER -ar 44100 -b:a 128k -pix_fmt yuv420p \
            -filter_complex "[0:v]scale=1920:1080[fondo]; \
            [1:v]scale=iw*0.59:ih*0.41[scale_main_video]; \
            [fondo][scale_main_video]overlay=x=155:y=280[xy_video_main];\
            [2:v]scale=iw*0.5:ih*0.5[scaled_logo];\
            [xy_video_main][scaled_logo]overlay=1410:120,ass='$titulares'" \
            -f matroska - | ffplay -fs -autoexit -
            ;;
        2)
            ffmpeg -i $overlay -f concat -safe 0 -i $lista_videos -i $logo \
            -c:v libx264 -profile:v baseline -preset $QUAL \
            -r $FPS -b:v $VBR -bufsize $BUFSIZE -maxrate $VBR \
            -c:a $AUDIO_ENCODER -ar 44100 -b:a 128k -pix_fmt yuv420p \
            -filter_complex "[0:v]scale=1920:1080[fondo]; \
            [1:v]scale=iw*0.59:ih*0.41[scale_main_video]; \
            [fondo][scale_main_video]overlay=x=155:y=280[xy_video_main];\
            [2:v]scale=iw*0.5:ih*0.5[scaled_logo];\
            [xy_video_main][scaled_logo]overlay=1410:120,ass='$titulares'" \
            "$ruta_video_final/video_plantilla_$plantilla_number.mkv"
            ;;
        *)
            echo "Opción inválida. Por favor, seleccione 1 o 2."
            ;;
    esac


