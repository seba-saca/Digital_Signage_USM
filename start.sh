#!/bin/bash

# User name
user_name=seba

# Función para mostrar las opciones
mostrar_menu() {
    clear
    echo "Seleccione una plantilla:"
    echo "1. Ejecutar plantilla 1"
    echo "2. Ejecutar plantilla 2"
    echo "3. Ejecutar plantilla 3"
    echo "4. Ejecutar plantilla 4"
    echo "5. Ejecutar plantilla 5"
    echo "6. Salir"
}


# Función para ejecutar el script seleccionado
ejecutar_opcion() {
    case $1 in
        1)
            /home/$user_name/Desktop/Digital_Signage_USM/Plantillas/1/video_creacion_1.sh
            ;;
        2)
            /home/$user_name/Desktop/Digital_Signage_USM/Plantillas/2/video_creacion_2.sh
            ;;
        3)
            /home/$user_name/Desktop/Digital_Signage_USM/Plantillas/3/video_creacion_3.sh
            ;;
        4)
            /home/$user_name/Desktop/Digital_Signage_USM/Plantillas/4/video_creacion_4.sh
            ;;
        5)
            /home/$user_name/Desktop/Digital_Signage_USM/Plantillas/5/video_creacion_5.sh
            ;;
        6)
            echo "Saliendo..."
            exit 0
            ;;
        *)
            echo "Opción inválida. Por favor, seleccione una opción válida."
            ;;
    esac
}

# Ciclo principal del programa
while true; do
    mostrar_menu
    read -p "Ingrese su opción: " opcion
    ejecutar_opcion $opcion
    read -p "Presione Enter para continuar..."
done
