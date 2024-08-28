#!/bin/bash

# ======================================================================
# Script para verificar la conexión SSH sin contraseña con un servidor remoto.
#
# Sintaxis de uso:
# ./check_dispo.sh <user_ip>
#
# Parámetros:
# 1. user_ip - Dirección IP o nombre de host del servidor remoto.
#
# Ejemplo:
# ./check_dispo.sh 192.168.1.100
#
# Firma: Sebastian Castillo
# ======================================================================

# Asignar el primer parámetro como la dirección IP o nombre de host del usuario
user_ip=$1

# Intentar establecer una conexión SSH sin contraseña
# -o BatchMode=yes: No solicita contraseña ni ninguna otra entrada interactiva
# -o ConnectTimeout=5: Establece un tiempo de espera de 5 segundos para la conexión
ssh -o BatchMode=yes -o ConnectTimeout=5 "$user_ip" 'echo SSH connection established' &>/dev/null

# Verificar si la conexión SSH fue exitosa
if [ $? -ne 0 ]; then
    # Si la conexión falla, se muestra un mensaje de error y el script termina con un código de salida 1
    echo "No se pudo establecer conexión SSH sin contraseña con $user_ip. Verifica la configuración de la clave SSH."
    exit 1
fi

# Si la conexión es exitosa, no se muestra ningún mensaje adicional
