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





# Crear archivo de feedback
echo "Creando archivo de feedback: $FEEDBACK_FILE"
echo "Inicio de la transferencia: $(date)" > "$FEEDBACK_FILE"

# Comando para verificar la conexión SSH sin contraseña
ssh -o BatchMode=yes -o ConnectTimeout=5 "$DEST_USER@$DEST_HOST" 'echo SSH connection established' &>/dev/null

# Verificar si la conexión SSH sin contraseña fue exitosa
if [ $? -ne 0 ]; then
    echo "No se pudo establecer conexión SSH sin contraseña con $DEST_USER. Verifica la configuración de la clave SSH." | tee -a "$FEEDBACK_FILE"
    exit 1
fi

# Verificar si el archivo de lista existe
echo "Ruta del archivo de lista: $FILES_LIST"
if [ ! -f "$FILES_LIST" ]; then
    echo "Error: El archivo $FILES_LIST no existe." | tee -a "$FEEDBACK_FILE"
    exit 1
else
    continue
    #echo "El archivo $FILES_LIST existe." | tee -a "$FEEDBACK_FILE"
fi

# Verificar si el directorio de destino existe en el dispositivo remoto y crearlo si no existe
#echo "Verificando o creando el directorio de destino en el dispositivo remoto..." | tee -a "$FEEDBACK_FILE"
ssh "$DEST_USER@$DEST_HOST" "mkdir -p $DEST_DIR"
if [ $? -ne 0 ]; then
    echo "Error: No se pudo crear el directorio de destino $DEST_DIR en el dispositivo remoto." | tee -a "$FEEDBACK_FILE"
    exit 1
else
    continue
    #echo "El directorio de destino $DEST_DIR está listo en el dispositivo remoto." | tee -a "$FEEDBACK_FILE"
fi

# Imprimir el contenido del archivo de lista
#echo "Contenido del archivo de lista:" | tee -a "$FEEDBACK_FILE"
#cat "$FILES_LIST" | tee -a "$FEEDBACK_FILE"

# Leer el archivo de lista de archivos y buscar cada archivo en el directorio fuente
while IFS= read -r filename; do
    #echo "Buscando archivos para: $filename" | tee -a "$FEEDBACK_FILE"

    # Buscar archivos que coincidan con el nombre en el directorio fuente
    found_files=$(find "$SOURCE_DIR" -maxdepth 1 -type f -name "$filename.*" -o -name "$filename")

    # Si se encuentran archivos, sincronizar cada uno de ellos
    if [ -n "$found_files" ]; then
        for file in $found_files; do
            #echo "Sincronizando archivo: $file" | tee -a "$FEEDBACK_FILE"
            rsync -avr --delete "$file" "$DEST_USER@$DEST_HOST:$DEST_DIR"
            if [ $? -ne 0 ]; then
                echo "Error: No se pudo sincronizar el archivo $filename." | tee -a "$FEEDBACK_FILE"
                exit 1
            else
                echo "$filename sincronizado correctamente." | tee -a "$FEEDBACK_FILE"
            fi
        done
    else
        echo "No se encontraron archivos para: $filename" | tee -a "$FEEDBACK_FILE"
    fi
done < "$FILES_LIST"

# Sincronizar el archivo de control
#echo "Sincronizando archivo de control: $PATH_CONTROL_DEVICE" | tee -a "$FEEDBACK_FILE"
rsync -avr --delete "$PATH_CONTROL_DEVICE" "$DEST_USER@$DEST_HOST:$PATH_DESTINO_SINCRO"
if [ $? -ne 0 ]; then
    echo "Error: No se pudo sincronizar el archivo de control $PATH_CONTROL_DEVICE." | tee -a "$FEEDBACK_FILE"
    exit 1
else
    echo "Archivo de control sincronizado correctamente." | tee -a "$FEEDBACK_FILE"
fi

echo "Transferencia completa." | tee -a "$FEEDBACK_FILE"
echo "Fin de la transferencia: $(date)" | tee -a "$FEEDBACK_FILE"
