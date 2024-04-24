#!/bin/bash
clear
# User name
user_name=seba

# Numero Plantilla
plantilla_number=test_plantilla

# Overlay
fondo=fondo_usm_1.jpg

# Logo
logo=elo.jpg

# Rutas
lista_videos="/home/$user_name/Desktop/Digital_Signage_USM/Plantillas/$plantilla_number/video_lista_$plantilla_number.txt"

overlay="/home/$user_name/Desktop/Contenido/Imagenes/Overlays/$fondo"

logo="/home/$user_name/Desktop/Contenido/Imagenes/Logos/$logo"

titulares="/home/$user_name/Desktop/Digital_Signage_USM/Plantillas/$plantilla_number/video_titulares_$plantilla_number.txt"

ruta_video_final="/home/$user_name/Desktop/Digital_Signage_USM/Plantillas/$plantilla_number"

# Parametros
VBR="3000K"
FPS="24"
BUFSIZE="10000K"
QUAL="ultrafast"
AUDIO_ENCODER="aac"

# Iniciar la cadena de filtros
filtro_completo=""

# Filtro previo sin texto
filtro_completo+="[0:v]scale=1920:1080[fondo];\
[1:v]scale=iw*0.59:ih*0.41[scale_main_video];\
[fondo][scale_main_video]overlay=x=155:y=280[xy_video_main];\
[2:v]scale=iw*0.5:ih*0.5[scaled_logo];\
[xy_video_main][scaled_logo]overlay=1410:120,"

# Tamano letra parametros
# Ancho máximo deseado
ancho_maximo=10

# Leer el archivo línea por línea
while IFS='|' read -r inicio fin texto || [[ -n "$campo1" ]]; do
    # Longitud del texto
    longitud_texto=$(echo -n "$texto" | wc -m)
    # Calcular el tamaño de la fuente necesario
    tamano_fuente=($(ancho_maximo / longitud_texto))
    filtro_completo+="drawtext=text='${texto}':fontsize=${tamano_fuente}:x=150:y=830:fontsize=52:fontcolor=white:enable='between(t,${inicio},${fin})',"
done < "$titulares"


# Agregar la última parte de la cadena de filtros, le quita el ultimo caracter, en este caso la ,
filtro_completo="${filtro_completo%?}"

ffmpeg -i $overlay -f concat -safe 0 -i $lista_videos -i $logo \
-c:v libx264 -profile:v baseline -preset $QUAL \
-r $FPS -b:v $VBR -bufsize $BUFSIZE -maxrate $VBR \
-c:a $AUDIO_ENCODER -ar 44100 -b:a 128k -pix_fmt yuv420p \
-filter_complex "$filtro_completo" \
-f matroska - | ffplay -fs -autoexit -



