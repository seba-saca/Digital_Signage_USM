#!/bin/bash

# Variables
SOURCE_DIR=$1             # Directorio fuente donde están los archivos
DEST_DIR=$2               # Directorio destino en el dispositivo remoto
DEST_USER=$3              # Usuario en el dispositivo remoto
DEST_HOST=$4              # Dirección IP o nombre de host del dispositivo remoto
FILES_LIST=$5             # Ruta del archivo con la lista de archivos
PATH_CONTROL_DEVICE=$6    # Ruta del archivo de control en el dispositivo local
PATH_DESTINO_SINCRO=$7    # Ruta destino para la sincronización del archivo de control
FEEDBACK_FILE=$8          # Archivo donde se escribirá el feedback de las operaciones
usuario=$USER



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

# Leer el archivo de lista y transferir cada archivo
while IFS= read -r FILE; do
    if [[ -f "$SOURCE_DIR/$FILE.mkv" ]]; then
        # Transferir el archivo usando rsync
        #rsync -avz --progress --ignore-existing "$SOURCE_DIR/$FILE.mkv" "$DEST_USER@$DEST_HOST:$DEST_DIR" >> "$FEEDBACK_FILE" 2>&1
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

# Sincronizar el archivo de control
#rsync -avz --progress "$PATH_CONTROL_DEVICE" "$DEST_USER@$DEST_HOST:$PATH_DESTINO_SINCRO" >> "$FEEDBACK_FILE" 2>&1
rsync -avz --progress "$PATH_CONTROL_DEVICE" "$DEST_USER@$DEST_HOST:$PATH_DESTINO_SINCRO"
if [[ $? -eq 0 ]]; then
    log_feedback "Éxito: El archivo de control se ha sincronizado correctamente."
else
    log_feedback "Error: Falló la sincronización del archivo de control."
fi

log_feedback "Transferencia de archivos finalizada"