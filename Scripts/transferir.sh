#!/bin/bash

# ======================================================================
# Script para transferir archivos a un dispositivo remoto y sincronizar un archivo de control
#
# Sintaxis de uso:
# ./transferir.sh <source_dir> <dest_dir> <dest_user> <dest_host> <files_list> <path_control_device> <path_destino_sincro> <feedback_file>
#
# Parámetros:
# 1. source_dir          - Directorio fuente donde están los archivos.
# 2. dest_dir            - Directorio destino en el dispositivo remoto.
# 3. dest_user           - Usuario en el dispositivo remoto.
# 4. dest_host           - Dirección IP o nombre de host del dispositivo remoto.
# 5. files_list          - Ruta del archivo con la lista de archivos a transferir.
# 6. path_control_device - Ruta del archivo de control en el dispositivo local.
# 7. path_destino_sincro - Ruta destino para la sincronización del archivo de control.
# 8. feedback_file       - Archivo donde se escribirá el feedback de las operaciones.
#
# Ejemplo:
# ./transferir.sh /fuente /destino remoto_user 192.168.1.10 /lista_archivos.txt /control_device.txt /remoto/sincro/control_device.txt /logs/feedback.txt
#
# Firma: Sebastian Castillo
# ======================================================================

# Variables
SOURCE_DIR=$1             # Directorio fuente donde están los archivos
DEST_DIR=$2               # Directorio destino en el dispositivo remoto
DEST_USER=$3              # Usuario en el dispositivo remoto
DEST_HOST=$4              # Dirección IP o nombre de host del dispositivo remoto
FILES_LIST=$5             # Ruta del archivo con la lista de archivos a transferir
PATH_CONTROL_DEVICE=$6    # Ruta del archivo de control en el dispositivo local
PATH_DESTINO_SINCRO=$7    # Ruta destino para la sincronización del archivo de control
FEEDBACK_FILE=$8          # Archivo donde se escribirá el feedback de las operaciones
usuario=$USER             # Nombre de usuario en la máquina local

# Inicializar el archivo de feedback (sobrescribir el contenido)
echo "Iniciando transferencia de archivos - $(date '+%Y-%m-%d %H:%M:%S')" > "$FEEDBACK_FILE"

# Función para registrar mensajes en el archivo de feedback
log_feedback() {
    local MESSAGE=$1
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $MESSAGE" >> "$FEEDBACK_FILE"
}

# Validar que el archivo de lista de archivos existe
if [[ ! -f "$FILES_LIST" ]]; then
    log_feedback "Error: El archivo de lista $FILES_LIST no existe."
    exit 1
fi

# Verificar y crear el directorio de destino en el dispositivo remoto
ssh "$DEST_USER@$DEST_HOST" "mkdir -p '$DEST_DIR'"
if [[ $? -ne 0 ]]; then
    log_feedback "Error: No se pudo crear el directorio de destino $DEST_DIR en el dispositivo remoto."
    exit 1
else
    log_feedback "Éxito: El directorio de destino $DEST_DIR se verificó/creó correctamente en el dispositivo remoto."
fi

# Leer el archivo de lista y transferir cada archivo
while IFS= read -r FILE; do
    if [[ -f "$SOURCE_DIR/$FILE.mkv" ]]; then
        # Transferir el archivo usando rsync
        rsync -avz --progress --ignore-existing "$SOURCE_DIR/$FILE.mkv" "$DEST_USER@$DEST_HOST:$DEST_DIR"
        if [[ $? -eq 0 ]]; then
            log_feedback "Éxito: $FILE transferido correctamente."
        else
            log_feedback "Error: Falló la transferencia de $FILE."
        fi
    else
        log_feedback "Advertencia: El archivo $FILE no se encontró en $SOURCE_DIR."
    fi
done < "$FILES_LIST"

# Verificar y crear el directorio de sincronización del archivo de control en el dispositivo remoto
ssh "$DEST_USER@$DEST_HOST" "mkdir -p '$(dirname "$PATH_DESTINO_SINCRO")'"
if [[ $? -ne 0 ]]; then
    log_feedback "Error: No se pudo crear el directorio de sincronización $(dirname "$PATH_DESTINO_SINCRO") en el dispositivo remoto."
    exit 1
else
    log_feedback "Éxito: El directorio de sincronización $(dirname "$PATH_DESTINO_SINCRO") se verificó/creó correctamente en el dispositivo remoto."
fi

# Sincronizar el archivo de control
rsync -avz --progress "$PATH_CONTROL_DEVICE" "$DEST_USER@$DEST_HOST:$PATH_DESTINO_SINCRO"
if [[ $? -eq 0 ]]; then
    log_feedback "Éxito: El archivo de control se ha sincronizado correctamente."
else
    log_feedback "Error: Falló la sincronización del archivo de control."
fi

# Finalizar el proceso y registrar en el archivo de feedback
log_feedback "Transferencia de archivos finalizada"
