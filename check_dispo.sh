#!/bin/bash
user_ip=$1

# Comando para verificar la conexión SSH sin contraseña
ssh -o BatchMode=yes -o ConnectTimeout=5 "$user_ip" 'echo SSH connection established' &>/dev/null

# Verificar si la conexión SSH sin contraseña fue exitosa
if [ $? -ne 0 ]; then
    echo "No se pudo establecer conexión SSH sin contraseña con $user_ip. Verifica la configuración de la clave SSH."
    exit 1
fi
