#!/bin/bash

# ======================================================================
# Script para verificar la conexión SSH sin contraseña y transferir un archivo desde un host remoto.
#
# Sintaxis de uso:
# ./copy_text.sh <REMOTE_HOST> <REMOTE_FILE_PATH> <LOCAL_FILE_PATH>
#
# Parámetros:
# 1. REMOTE_HOST       - Dirección IP o nombre de host del servidor remoto.
# 2. REMOTE_FILE_PATH  - Ruta completa del archivo en el servidor remoto.
# 3. LOCAL_FILE_PATH   - Ruta donde se guardará el archivo en la máquina local.
#
# Ejemplo:
# ./copy_text.sh 192.168.1.100 /remote/path/to/file.txt /local/path/to/destination/
#
# Firma: Sebastian Castillo
# ======================================================================

# Variables
REMOTE_HOST=$1           # Dirección IP o nombre de host del servidor remoto
REMOTE_FILE_PATH=$2      # Ruta del archivo en el servidor remoto
LOCAL_FILE_PATH=$3       # Ruta donde se guardará el archivo en la máquina local

# Verificar la conexión SSH sin contraseña
ssh -o BatchMode=yes -o ConnectTimeout=5 "$REMOTE_HOST" 'echo SSH connection established' &>/dev/null

# Verificar si la conexión SSH fue exitosa
if [ $? -ne 0 ]; then
    # Si la conexión falla, se muestra un mensaje de error y el script termina con un código de salida 1
    echo "No se pudo establecer conexión SSH sin contraseña con $REMOTE_HOST. Verifica la configuración de la clave SSH."
    exit 1
fi

# Transferir el archivo desde el servidor remoto a la máquina local usando SCP
scp "${REMOTE_HOST}:${REMOTE_FILE_PATH}" "${LOCAL_FILE_PATH}"

# Comprobar el estado de la transferencia
if [ $? -ne 0 ]; then
    # Si la transferencia falla, se muestra un mensaje de error y el script termina con un código de salida 1
    echo "Error: No se pudo transferir el archivo remoto." > ${LOCAL_FILE_PATH}
    exit 1
fi
