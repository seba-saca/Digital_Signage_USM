#!/bin/bash

# Definir variables para directorios de origen y destino
origen="/home/seba/Desktop/Contenido_Compartir"

user_destino="seba"
ip_destino="192.168.0.48"

destino="$user_destino@$ip_destino:/home/seba/Desktop"

# Ejecutar rsync para copiar los archivos
rsync -avr --delete "$origen" "$destino"

# Comprobar el código de salida de rsync
if [ $? -eq 0 ]; then
    echo "La sincronización se ha completado correctamente."
else
    echo "Hubo un error durante la sincronización."
fi
