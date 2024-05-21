#!/bin/bash

# Definir variables para directorios de origen y destino
device=$1

origen="/home/seba/Desktop/Contenido_Compartir/Dispositivo_$device"

user_destino_1="berry"
ip_destino_1="192.168.43.221"

user_destino_2=""
ip_destino_2=""

user_destino_3=""
ip_destino_3=""

opcion=$device

    case $opcion in
        1)
            destino="$user_destino_1@$ip_destino_1:/home/$user_destino_1/Desktop"
            rsync -avr --delete "$origen" "$destino"
            ;;
        2)
            destino="$user_destino_2@$ip_destino_2:/home/$user_destino_2/Desktop"
            rsync -avr --delete "$origen" "$destino"
            ;;
        3)
            destino="$user_destino_3@$ip_destino_3:/home/$user_destino_3/Desktop"
            rsync -avr --delete "$origen" "$destino"
            ;;
        *)
            echo "Opción inválida. Por favor, seleccione 1, 2 o 3."
            ;;
    esac

# Comprobar el código de salida de rsync
if [ $? -eq 0 ]; then
    echo "La sincronización se ha completado correctamente."
else
    echo "Hubo un error durante la sincronización."
fi
