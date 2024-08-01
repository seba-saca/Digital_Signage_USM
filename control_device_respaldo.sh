#!/bin/bash
path_videos=$1
name_video=$2
OPTION=$3


# Definir variables
VIDEO_PATH="$path_videos/$name_video.mkv"

CONTROL_FILE="/tmp/mplayer-control"

if [ ! -f $CONTROL_FILE ]; then
    echo "No se ha encontrado el archivo de control."
    mkfifo $CONTROL_FILE
    echo "Archivo de control creado."
fi

case $OPTION in
    1)
        # Reproducir video
        mplayer -slave -input file=$CONTROL_FILE -display :0 -fs $VIDEO_PATH
        ;;
    2)
        # Pausar video
        echo "pause" > $CONTROL_FILE
        ;;
    3)
        # Detener video
        echo "quit" > $CONTROL_FILE
        ;;
    4)  
        # Adelantar 5seg
        echo "seek 5" > $CONTROL_FILE
        ;;
    5) 
        # Retroceder 5seg
        echo "seek -5" > $CONTROL_FILE
        ;;
    6)
        # Subir 5 volumen
        echo "volume 10" > $CONTROL_FILE
        ;;
    7) 
        # Bajar 5 volumen
        echo "volume -10" > $CONTROL_FILE
        ;;
    8) 
        # Silenciar video
        echo "mute" > $CONTROL_FILE
        ;;
    9) 
        # Bucle
        echo "loop 1" > $CONTROL_FILE
        ;;
    *)
        echo "Opción inválida."
        ;;
esac

if [ $? -eq 0 ]; then
    echo "La ejecución se ha completado correctamente."
else
    echo "Hubo un error durante la ejecución."
fi