#!/bin/bash
# Definir variables para directorios de origen y destino
device=$1
destino=$2

# Usando cut para extraer cada parte del string
username=$(echo "$destino" | cut -d '@' -f 1)

origen="/home/seba/Desktop/Contenido_Dispositivos/$username"

destino="$destino:/home/$username/Desktop"

rsync -avr --delete "$origen" "$destino"

# Comprobar el código de salida de rsync
if [ $? -eq 0 ]; then
    echo "La sincronización se ha completado correctamente."
else
    echo "Hubo un error durante la sincronización."
fi
