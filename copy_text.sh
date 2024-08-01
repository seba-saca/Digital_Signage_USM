#!/bin/bash

# Variables
remote_host=$1  # Usuario y servidor remoto
remote_file=$2  # Ruta del archivo remoto
local_file=$3   # Ruta del archivo local

# Función para registrar mensajes en el archivo local
log_message() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $1" >> "$local_file"
}

# Comando para verificar la conexión SSH sin contraseña
log_message "Intentando establecer conexión SSH sin contraseña con $remote_host..."
ssh -o BatchMode=yes -o ConnectTimeout=5 "$remote_host" 'echo SSH connection established' &>/dev/null

# Verificar si la conexión SSH sin contraseña fue exitosa
if [ $? -ne 0 ]; then
    log_message "Error: No se pudo establecer conexión SSH sin contraseña con $remote_host. Verifica la configuración de la clave SSH."
    log_message "Estado: No se pudo establecer conexión con el archivo remoto en $remote_host."
    exit 1
fi

log_message "Conexión SSH establecida con éxito."

# Intentar copiar el archivo remoto al archivo local
log_message "Intentando copiar el archivo remoto $remote_file desde $remote_host al archivo local $local_file..."
if ssh "$remote_host" "cat '$remote_file'" > "$local_file.tmp" 2>>"$local_file"; then
    log_message "Archivo copiado exitosamente. Moviendo el archivo temporal al archivo final."
    mv "$local_file.tmp" "$local_file"
else
    log_message "Error: No se pudo copiar el archivo remoto desde $remote_host. Verifica que el archivo remoto existe y que tienes permisos para acceder a él."
    log_message "Estado: No se pudo encontrar o acceder al archivo remoto en $remote_host."
    rm -f "$local_file.tmp"  # Eliminar archivo temporal en caso de error
    exit 1
fi

# Verificar si el archivo local se ha creado y no está vacío
if [ -s "$local_file" ]; then
    log_message "El archivo local $local_file se ha creado y contiene datos."
else
    log_message "Advertencia: El archivo local $local_file está vacío. Hubo un problema al copiar el archivo remoto."
    exit 1
fi

log_message "Proceso completado exitosamente."
