#!/bin/bash

# Ruta del video original
video_original=$1

# Ruta de destino para copiar el video
ruta_destino=$2

# Verificar si el video existe en la ruta de destino
if [ -f "$ruta_destino" ]; then
    echo "El video ya existe en la ruta de destino."
else
    # Copiar el video a la ruta de destino
    cp "$video_original" "$ruta_destino"

    # Verificar si la copia fue exitosa
    if [ $? -eq 0 ]; then
        echo "El video se ha copiado exitosamente a $ruta_destino."
    else
        echo "Ha ocurrido un error al copiar el video."
    fi
fi