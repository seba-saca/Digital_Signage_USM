#!/bin/bash

# ======================================================================
# Script para controlar la reproducción de videos usando MPlayer
# 
# Sintaxis de uso:
# ./control_device.sh <path_videos> <name_video> <option> <path_txt_file>
# 
# Parámetros:
# 1. path_videos   - Ruta donde se encuentran los videos.
# 2. name_video    - Nombre del video (sin extensión).
# 3. option        - Opción a ejecutar (reproducir, pausar, detener, etc.).
# 4. path_txt_file - Ruta donde se guardará el archivo de estado.
# 
# Ejemplo:
# ./control_device.sh /videos mi_video 1 /logs
# 
# Opción       Descripción
# -------      -----------
# 1            Reproducir video
# 2            Pausar/Reanudar video
# 3            Detener video
# 4            Adelantar 5 segundos
# 5            Retroceder 5 segundos
# 6            Subir volumen en 10
# 7            Bajar volumen en 10
# 8            Silenciar video
# 9            Poner en bucle el video
# 10           Reanudar video
#
# Firma: Sebastian Castillo
# ======================================================================

# Asignación de argumentos a variables
path_videos=$1
name_video=$2
OPTION=$3
path_txt_file=$4

# Definir rutas y archivos utilizados
VIDEO_PATH="$path_videos/$name_video.mkv"          # Ruta completa del archivo de video
CONTROL_FILE="/tmp/mplayer-control"                # Archivo FIFO para enviar comandos a MPlayer
OUTPUT_FILE="/tmp/mplayer-output"                  # Archivo temporal para capturar la salida de MPlayer
STATUS_FILE="$path_txt_file/mplayer_status.txt"    # Archivo donde se almacenará el estado del dispositivo

# Obtener la hora actual en formato "YYYY-MM-DD HH:MM:SS"
current_time() {
    echo $(date +"%Y-%m-%d %H:%M:%S")
}

# Crear archivo de control si no existe (FIFO)
if [ ! -p $CONTROL_FILE ]; then
    echo "$(current_time) - $name_video - No se ha encontrado el archivo de control."
    mkfifo $CONTROL_FILE
    echo "$(current_time) - $name_video - Archivo de control creado."
fi

# Crear archivo de estado si no existe
if [ ! -f $STATUS_FILE ]; then
    echo "$(current_time) - $name_video - Archivo de estado no encontrado, creando $STATUS_FILE"
    touch $STATUS_FILE
fi

# Registrar la acción realizada en el archivo de estado (sobreescribir)
log_status() {
    local action=$1
    {
        echo "============"
        echo "$(current_time)"
        echo "ESTADO: $DEVICE_STATUS"
        if [ "$action" != "Contenido detenido" ]; then
            echo "============"
            echo "ACCIÓN: $action"
        fi
        echo "============"
    } > "$STATUS_FILE"  # Utiliza ">" para sobreescribir
}

# Monitorizar el estado de MPlayer (si sigue ejecutándose)
monitor_mplayer() {
    while true; do
        # Verificar si el proceso MPlayer sigue ejecutándose
        if ! pgrep -f "mplayer" > /dev/null; then
            DEVICE_STATUS="No está reproduciendo"
            log_status "El dispositivo ya no está reproduciendo nada"
            break
        fi
        sleep 1
    done
}

# Procesar la opción seleccionada (case statement)
case $OPTION in
    1)
        # Reproducir video en pantalla completa
        mplayer -slave -input file=$CONTROL_FILE -display :0 -fs $VIDEO_PATH > $OUTPUT_FILE &
        sleep 2
        DEVICE_STATUS="Reproduciendo $name_video"
        log_status "Reproduciendo contenido"
        monitor_mplayer &  # Monitorear el estado de MPlayer en segundo plano
        ;;
    2)
        # Pausar o reanudar video
        echo "pause" > $CONTROL_FILE
        DEVICE_STATUS="No está reproduciendo"
        log_status "Contenido pausado"
        ;;
    3)
        # Detener video
        echo "quit" > $CONTROL_FILE
        DEVICE_STATUS="No está reproduciendo"
        log_status "Contenido detenido"
        ;;
    4)
        # Adelantar 5 segundos en el video
        DEVICE_STATUS="Reproduciendo $name_video"
        echo "seek 5" > $CONTROL_FILE
        log_status "Adelantado 5 segundos"
        ;;
    5)
        # Retroceder 5 segundos en el video
        DEVICE_STATUS="Reproduciendo $name_video"
        echo "seek -5" > $CONTROL_FILE
        log_status "Retrocedido 5 segundos"
        ;;
    6)
        # Subir volumen en 10 unidades
        DEVICE_STATUS="Reproduciendo $name_video"
        echo "volume 10" > $CONTROL_FILE
        log_status "Volumen subido en 10"
        ;;
    7)
        # Bajar volumen en 10 unidades
        DEVICE_STATUS="Reproduciendo $name_video"
        echo "volume -10" > $CONTROL_FILE
        log_status "Volumen bajado en 10"
        ;;
    8)
        # Silenciar el video
        DEVICE_STATUS="Reproduciendo $name_video"
        echo "mute" > $CONTROL_FILE
        log_status "Contenido silenciado"
        ;;
    9)
        # Repetir el video en bucle
        DEVICE_STATUS="Reproduciendo $name_video"
        echo "loop 1" > $CONTROL_FILE
        log_status "Contenido en bucle"
        ;;
    10)
        # Reanudar video después de pausar
        echo "pause" > $CONTROL_FILE
        DEVICE_STATUS="Reproduciendo $name_video"
        log_status "Contenido reanudado"
        ;;
    *)
        # Manejar opción inválida
        echo "Opción inválida."
        log_status "Opción inválida seleccionada"
        ;;
esac

# Verificar el resultado de la ejecución del script
if [ $? -eq 0 ]; then
    echo "La ejecución se ha completado correctamente."
else
    echo "Hubo un error durante la ejecución."
    log_status "Error durante la ejecución"
fi
