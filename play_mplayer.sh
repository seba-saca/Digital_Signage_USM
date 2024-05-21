#!/bin/bash


CONTROL_FILE="/tmp/mplayer-control"
VIDEO="/home/seba/Desktop/Contenido_Compartir/Dispositivo_1/Plantilla_1/video_plantilla_1.mkv"

if [ ! -p "$CONTROL_FILE" ]; then
    echo "Error: Control file not found!"
    exit 1
fi

opcion=$1

    case $opcion in
        1)
            mplayer -slave -loop 0 -input file=$CONTROL_FILE $VIDEO
            ;;
        2)
            echo "pause" > $CONTROL_FILE
            ;;
        3)
            echo "quit" > $CONTROL_FILE
            ;;
        *)
            echo "Opción inválida. Por favor, seleccione 1, 2 o 3."
            ;;
    esac

    
