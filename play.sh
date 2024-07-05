#!/bin/bash
user_ip=$1
path_script=$2
path_videos=$3
name_video=$4
accion=$5

ssh $user_ip $path_script $path_videos $name_video $accion

# Comprobar el código de salida de rsync
if [ $? -eq 0 ]; then
    echo "La ejecucion se ha completado correctamente."
else
    echo "Hubo un error durante la ejecucion."
fi