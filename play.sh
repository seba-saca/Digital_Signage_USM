#!/bin/bash
device=$1
plantilla=$2
accion=$3
user_ip=$4

destino="/home/berry/Desktop/Dispositivo_$device/dispo_play.sh $device $plantilla $accion"

ssh $user_ip $destino

# Comprobar el código de salida de rsync
if [ $? -eq 0 ]; then
    echo "La ejecucion se ha completado correctamente."
else
    echo "Hubo un error durante la ejecucion."
fi