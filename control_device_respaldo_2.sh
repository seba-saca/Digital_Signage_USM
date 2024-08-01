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

# Crear archivo de control si no existe
if [ ! -p $CONTROL_FILE ]; then
    echo "No se ha encontrado el archivo de control."
    mkfifo $CONTROL_FILE
    echo "Archivo de control creado."
fi
# Crear archivo de control si no existe
if [ ! -p $CONTROL_FILE ]; then
    echo "No se ha encontrado el archivo de control."
    mkfifo $CONTROL_FILE
    echo "Archivo de control creado."
fi

# Crear archivo de estado si no existe
if [ ! -f $STATUS_FILE ]; then
    echo "Archivo de estado no encontrado, creando $STATUS_FILE"
    touch $STATUS_FILE
fi

# Iniciar mplayer si la opción es reproducir
if [ $OPTION -eq 1 ]; then
    mplayer -slave -input file=$CONTROL_FILE -display :0 -fs $VIDEO_PATH > $OUTPUT_FILE &
    sleep 2
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

    # Leer la última línea del archivo de salida de mplayer
    result=$(tail -n 1 $OUTPUT_FILE)
    echo "$result"
}

# Procesar la opción seleccionada
case $OPTION in
    1)
        # Reproducir video (iniciado más arriba)
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
        # Adelantar 5 segundos
        echo "seek 5" > $CONTROL_FILE
        ;;
    5) 
        # Retroceder 5 segundos
        echo "seek -5" > $CONTROL_FILE
        ;;
    6)
        # Subir volumen 10
        echo "volume 10" > $CONTROL_FILE
        ;;
    7) 
        # Bajar volumen 10
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
    10)
        # Obtener información y escribir en el archivo de estado
        time_pos=$(get_mplayer_info "get_time_pos")
        percent_pos=$(get_mplayer_info "get_percent_pos")
        time_length=$(get_mplayer_info "get_time_length")
        volume=$(get_mplayer_info "get_property volume")

        # Formatear la información de una manera más amigable
        {
            echo "-----------------------------"
            echo "Estado de Reproducción de Video"
            echo "-----------------------------"
            echo "Posición actual: $time_pos segundos"
            echo "Porcentaje reproducido: $percent_pos%"
            echo "Duración total del video: $time_length segundos"
            echo "Volumen actual: $volume"
            echo "-----------------------------"
        } > "$STATUS_FILE"

        echo "Información de estado escrita en $STATUS_FILE"
        ;;
    *)
        echo "Opción inválida."
        ;;
esac

# Verificar el resultado de la ejecución
if [ $? -eq 0 ]; then
    echo "La ejecución se ha completado correctamente."
else
    echo "Hubo un error durante la ejecución."
fi