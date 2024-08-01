#!/bin/bash

# Variables
REMOTE_HOST=$1
REMOTE_FILE_PATH=$2
LOCAL_FILE_PATH=$3

# Comando para verificar la conexión SSH sin contraseña
ssh -o BatchMode=yes -o ConnectTimeout=5 "$REMOTE_HOST" 'echo SSH connection established' &>/dev/null

# Verificar si la conexión SSH sin contraseña fue exitosa
if [ $? -ne 0 ]; then
    echo "No se pudo establecer conexión SSH sin contraseña con $REMOTE_HOST. Verifica la configuración de la clave SSH."
    exit 1
fi

# Comando SCP para transferir el archivo
scp ${REMOTE_HOST}:${REMOTE_FILE_PATH} ${LOCAL_FILE_PATH}

# Comprobar el estado de la transferencia
if [ $? -ne 0 ]; then
  echo "Error: No se pudo transferir el archivo remoto." > ${LOCAL_FILE_PATH}
  exit 1
fi

