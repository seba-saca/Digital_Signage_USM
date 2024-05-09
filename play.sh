#!/bin/bash
device=$1
plantilla=$2

# Definir variables para directorios de origen y destino

user_destino="seba"
ip_destino="192.168.0.48"

destino="/home/seba/Desktop/Dispositivo_$device/dispo_play.sh $device $plantilla"

user_destino_1="seba"
ip_destino_1="192.168.0.48"

user_destino_2=""
ip_destino_2=""

user_destino_3=""
ip_destino_3=""

opcion=$device

    case $opcion in
        1)
            # Conectarse al equipo remoto y ejecutar el script
            #ssh $user_destino@$ip_destino < $destino
            ssh $user_destino_1@$ip_destino_1 $destino
            ;;
        2)
            ssh $user_destino_2@$ip_destino_2 $destino
            ;;
        3)
            ssh $user_destino_3@$ip_destino_3 $destino
            ;;
        *)
            echo "Opción inválida. Por favor, seleccione 1, 2 o 3."
            ;;
    esac

# Comprobar el código de salida de rsync
if [ $? -eq 0 ]; then
    echo "La ejecucion se ha completado correctamente."
else
    echo "Hubo un error durante la ejecucion."
fi