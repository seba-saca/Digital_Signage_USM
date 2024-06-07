#!/bin/bash
device=$1
plantilla=$2
accion=$3
user_ip=$4
video=$5

# Usando cut para extraer cada parte del string
username=$(echo "$user_ip" | cut -d '@' -f 1)

destino="/home/$username/Desktop/$username/control_device.sh $device $plantilla $accion $username $video"

ssh $user_ip $destino

# Comprobar el código de salida de rsync
if [ $? -eq 0 ]; then
    echo "La ejecucion se ha completado correctamente."
else
    echo "Hubo un error durante la ejecucion."
fi