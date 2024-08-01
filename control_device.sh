#!/bin/bash

path_videos=$1
name_video=$2
OPTION=$3
path_txt_file=$4

# Definir variables
VIDEO_PATH="$path_videos/$name_video.mkv"
CONTROL_FILE="/tmp/mplayer-control"
OUTPUT_FILE="/tmp/mplayer-output"
STATUS_FILE="$path_txt_file/mplayer_status.txt"
DEVICE_STATUS="No está reproduciendo"  # Estado inicial del dispositivo, interfiere con pause, debe estar comentado

# Obtener la hora actual
current_time() {
    echo $(date +"%Y-%m-%d %H:%M:%S")
}

# Crear archivo de control si no existe
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

# Función para obtener información de mplayer
get_mplayer_info() {
    local command=$1
    local result

    # Limpiar el archivo de salida antes de enviar el comando
    > $OUTPUT_FILE

    # Enviar comando a mplayer
    echo "$command" > $CONTROL_FILE

    # Espera un momento para que mplayer procese el comando
    sleep 1

    # Leer la línea relevante del archivo de salida de mplayer
    result=$(grep "$command" $OUTPUT_FILE | tail -n 1 | awk -F= '{print $2}' | xargs)
    echo "$result"
}

# Registrar la acción realizada en el archivo de estado (sobreescribir)
log_status() {
    local action=$1
    {
        echo "============"
        echo "$(current_time)"
        echo "ESTADO: $DEVICE_STATUS"
        if [ "$action" == "Contenido detenido" ]; then
            continue
        else
            echo "============"
            echo "ACCIÓN: $action"
            #echo "Contenido seleccionado: $name_video"
        fi
               
        echo "============"
    } > "$STATUS_FILE"  # Utiliza ">" para sobreescribir
}


# Monitorizar el estado de mplayer
monitor_mplayer() {
    while true; do
        # Verifica si el proceso mplayer sigue ejecutándose
        if ! pgrep -f "mplayer" > /dev/null; then
            DEVICE_STATUS="No está reproduciendo"
            log_status "El dispositivo ya no está reproduciendo nada"
            break
        fi
        sleep 1
    done
}

# Procesar la opción seleccionada
case $OPTION in
    1)
        # Reproducir video
        mplayer -slave -input file=$CONTROL_FILE -display :0 -fs $VIDEO_PATH > $OUTPUT_FILE &
        sleep 2
        DEVICE_STATUS="Reproduciendo $name_video"
        log_status "Reproduciendo contenido"
        monitor_mplayer &
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
        # Adelantar 5 segundos
        DEVICE_STATUS="Reproduciendo $name_video"
        echo "seek 5" > $CONTROL_FILE
        log_status "Adelantado 5 segundos"
        ;;
    5)
        # Retroceder 5 segundos
        DEVICE_STATUS="Reproduciendo $name_video"
        echo "seek -5" > $CONTROL_FILE
        log_status "Retrocedido 5 segundos"
        ;;
    6)
        # Subir volumen 10
        DEVICE_STATUS="Reproduciendo $name_video"
        echo "volume 10" > $CONTROL_FILE
        log_status "Volumen subido en 10"
        ;;
    7)
        # Bajar volumen 10
        DEVICE_STATUS="Reproduciendo $name_video"
        echo "volume -10" > $CONTROL_FILE
        log_status "Volumen bajado en 10"
        ;;
    8)
        # Silenciar video
        DEVICE_STATUS="Reproduciendo $name_video"
        echo "mute" > $CONTROL_FILE
        log_status "Contenido silenciado"
        ;;
    9)
        # Bucle
        DEVICE_STATUS="Reproduciendo $name_video"
        echo "loop 1" > $CONTROL_FILE
        log_status "Contenido en bucle"
        ;;
    10)
        # Reanudar video
        echo "pause" > $CONTROL_FILE
        DEVICE_STATUS="Reproduciendo $name_video"
        log_status "Contenido reanudado"
        ;;
    *)
        echo "Opción inválida."
        log_status "Opción inválida seleccionada"
        ;;
esac

# Verificar el resultado de la ejecución
if [ $? -eq 0 ]; then
    echo "La ejecución se ha completado correctamente."
else
    echo "Hubo un error durante la ejecución."
    log_status "Error durante la ejecución"
fi
