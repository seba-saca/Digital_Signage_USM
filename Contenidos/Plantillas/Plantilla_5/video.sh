#!/bin/bash

#Parametros
admin_name=$1
parametro_plantilla=Plantilla_5
name_output=$2
path_general="/home/$admin_name/Desktop/Contenido_ELO308/Plantillas/$parametro_plantilla"
playlist="$path_general/Videos.txt"
title_grande="$path_general/Titular_Grande-1.txt"
title_grande_2="$path_general/Titular_Grande-2.txt"
fondo="/home/$admin_name/Desktop/Digital_Signage_USM/Plantillas/Imagenes/Overlays/$parametro_plantilla/fondo.jpg"
logo_elo="/home/$admin_name/Desktop/Digital_Signage_USM/Plantillas/Imagenes/Logos/elo.jpg"
accion_video=2
ruta_video_final="/home/$admin_name/Desktop/Contenido_ELO308/Videos"

# Inicializar inicio y fin
inicio=0
fin=5

# Parametros
VBR="3000K"
FPS="24"
BUFSIZE="10000K"
QUAL="ultrafast"
AUDIO_ENCODER="aac"

# Iniciar la cadena de filtros
filtro_base=""

# Filtro previo sin texto
filtro_base+="[0:v]scale=1920:1080[fondo]; \
    [1:v]scale=iw*0.57:ih*0.57[scale_main_video]; \
    [fondo][scale_main_video]overlay=x=150:y=280[xy_video_main];\
    [2:v]scale=iw*0.5:ih*0.5[scaled_logo];\
    [xy_video_main][scaled_logo]overlay=1410:120"

# Iniciar la cadena de filtros
filtro_completo=""

# Filtro previo sin texto
filtro_completo+="$filtro_base,"



# Leer el archivo línea por línea
while IFS= read -r archivo_txt
do
    filtro_completo+="drawtext=textfile='${archivo_txt}':x=1325:y=270:fontsize=40:fontcolor=white:fontfile=path/to/font.ttf:shadowcolor=gray@0.5:shadowx=2:shadowy=2:enable='between(t,${inicio},${fin})',"
    inicio=$((inicio + 5))
    fin=$((fin + 5))
done < "$title_grande"

inicio=0
fin=5

# Leer el archivo línea por línea
while IFS= read -r archivo_txt
do
    filtro_completo+="drawtext=textfile='${archivo_txt}':x=1325:y=620:fontsize=40:fontcolor=white:fontfile=path/to/font.ttf:shadowcolor=gray@0.5:shadowx=2:shadowy=2:enable='between(t,${inicio},${fin})',"
    inicio=$((inicio + 5))
    fin=$((fin + 5))
done < "$title_grande_2"


# Agregar la última parte de la cadena de filtros, le quita el ultimo caracter, en este caso la ,
filtro_completo="${filtro_completo%?}"


echo "Seleccione accion"
echo "Opcion 1: Tiempo real"
echo "Opcion 2: Diferido con titular"

#read -p "Opción seleccionada: " opcion
opcion=$accion_video

    case $opcion in
        1)
            ffmpeg -i $fondo -f concat -safe 0 -i $playlist -i $logo_elo \
            -c:v libx264 -profile:v baseline -preset $QUAL \
            -r $FPS -b:v $VBR -bufsize $BUFSIZE -maxrate $VBR \
            -c:a $AUDIO_ENCODER -ar 44100 -b:a 128k -pix_fmt yuv420p \
            -filter_complex "$filtro_completo" \
            -f matroska - | ffplay -fs -autoexit -
            ;;
        2)
            yes | ffmpeg -i $fondo -f concat -safe 0 -i $playlist -i $logo_elo \
            -c:v libx264 -profile:v baseline -preset $QUAL \
            -r $FPS -b:v $VBR -bufsize $BUFSIZE -maxrate $VBR \
            -c:a $AUDIO_ENCODER -ar 44100 -b:a 128k -pix_fmt yuv420p \
            -filter_complex "$filtro_completo" \
            "$ruta_video_final/$name_output.mkv"
            ;;
        *)
            echo "Opción inválida. Por favor, seleccione 1 o 2."
            ;;
    esac

yes | ffmpeg -i "$ruta_video_final/$name_output.mkv" -vf "thumbnail,scale=720:480" -frames:v 1 "/home/$admin_name/Desktop/Contenido_ELO308/miniaturas/$name_output.jpg"