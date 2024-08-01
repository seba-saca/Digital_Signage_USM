#!/bin/bash
user_ip=$1
path_script=$2
path_videos=$3
name_video=$4
accion=$5
path_txt_file=$6

# Comando para verificar la conexión SSH sin contraseña
ssh -o BatchMode=yes -o ConnectTimeout=5 "$user_ip" 'echo SSH connection established' &>/dev/null

# Verificar si la conexión SSH sin contraseña fue exitosa
if [ $? -ne 0 ]; then
    echo "No se pudo establecer conexión SSH sin contraseña con $user_ip. Verifica la configuración de la clave SSH."
    exit 1
fi


ssh $user_ip $path_script $path_videos $name_video $accion $path_txt_file

# Comprobar el código de salida de rsync
if [ $? -eq 0 ]; then
    echo "La ejecucion se ha completado correctamente."
else
    echo "Hubo un error durante la ejecucion."
fi