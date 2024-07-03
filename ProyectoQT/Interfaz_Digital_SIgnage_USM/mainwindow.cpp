#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "variables_globales.h"
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //ui->Logo_USM->setPixmap(QPixmap("/home/seba/Desktop/Contenido/Imagenes/Logos/logo_usm.png"));
    ui->Logo_ELO->setPixmap(QPixmap("/home/seba/Desktop/Contenido/Imagenes/Logos/elo.jpg"));
    QString video_simbolo = "/home/seba/Desktop/Digital_Signage_USM/Material_Interfaz/video_simbolo.jpg";
    ui->preview_plantilla->setPixmap(QPixmap(video_simbolo));



    QString path = "/home/seba/Desktop/Contenido/Videos/Material_Prueba";
    QDir dir(path);
    QStringList files = dir.entryList(QDir::Files);
    ui->Video_lista->addItems(files);

    QString path2 = "/home/seba/Desktop/Contenido/Titulares/Small";
    QDir dir2(path2);
    QStringList files2 = dir2.entryList(QDir::Files);
    ui->Titulares_Lista->addItems(files2);

    QString path3 = "/home/seba/Desktop/Contenido/Titulares/Large";
    QDir dir3(path3);
    QStringList files3 = dir3.entryList(QDir::Files);
    ui->Titulares_Large_Lista->addItems(files3);


    QString filename_ubicaciones = "/home/seba/Desktop/Contenido_ELO308/videos";
    QDir dir5(filename_ubicaciones);
    QStringList files_lista_ubicaciones = dir5.entryList(QDir::Files);
    ui->Gestion_Contenido_Disponible->addItems(files_lista_ubicaciones);


    QString filename;
    QString filename2;
    QString filename3;



    filename = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/1/video_lista_1.txt";
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        ui->ResumenVideos->setPlainText(stream.readAll()); // Cargar el contenido del archivo en el QTextEdit
        file.close();
    }

    filename2 = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/1/video_titulares_1.txt";
    QFile file2(filename2);
    if (file2.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file2);
        ui->ResumenTitulares->setPlainText(stream.readAll()); // Cargar el contenido del archivo en el QTextEdit
        file2.close();
    }

    filename3 = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/1/video_titulares_large_1.txt";
    QFile file3(filename3);
    if (file3.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file3);
        ui->ResumenTitularesLarge->setPlainText(stream.readAll()); // Cargar el contenido del archivo en el QTextEdit
        file3.close();
    }

    QString filename_dispositivos_registrados = "/home/seba/Desktop/Digital_Signage_USM/Dispositivos_Registrados.txt";
    QFile file_dispositivos_registrados(filename_dispositivos_registrados);
    if (file_dispositivos_registrados.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream_dispositivos_registrados(&file_dispositivos_registrados);
        while (!stream_dispositivos_registrados.atEnd()) {
            QString line_dispositivos_registrados = stream_dispositivos_registrados.readLine();
            if (!line_dispositivos_registrados.isEmpty()){
                ui->asignacion_dispositivos->addItem(line_dispositivos_registrados); // Cargar el contenido del archivo en el QTextEdit
            }
        }
        file_dispositivos_registrados.close();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Start_clicked()
{

    int indice_actual = ui->Lista_plantillas->currentIndex();
    int indice_actual_device = ui->home_dispositivo->currentIndex();
    QString scriptPath;
    QString indice_string = QString::number(indice_actual+1);
    QString indice_device_string = QString::number(indice_actual_device+1);
    scriptPath = "/home/seba/Desktop/Digital_Signage_USM/play.sh";
    QString Dispositivo_seleccionado = ui->home_dispositivo->currentText();

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;

    QString name_video= ui->Lista_plantillas->currentText();

    arguments << indice_device_string << indice_string << "1" << Dispositivo_seleccionado << name_video;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    //process->start("bash", QStringList() << scriptPath);
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath << arguments;


}

void MainWindow::on_sincronizar_clicked()
{
    int indice_actual_device = ui->home_dispositivo->currentIndex();
    QString scriptPath;
    QString indice_device_string = QString::number(indice_actual_device+1);
    scriptPath = "/home/seba/Desktop/Digital_Signage_USM/transferir.sh";
    QString Dispositivo_seleccionado = ui->home_dispositivo->currentText();

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << Dispositivo_seleccionado;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    //process->start("bash", QStringList() << scriptPath);
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath << arguments;
}

void MainWindow::on_Lista_plantillas_activated(int index)
{
    //Recuperamos video seleccionado
    QString name_video= ui->Lista_plantillas->currentText();
    QChar delimiter = '.';
    QStringList tokens = name_video.split(delimiter);

    int i = 0;
    qDebug() << "Video:" << tokens[i] << "\n";

    //Miniatura plantilla
    QString path_miniaturas = global_path+"Contenido_ELO308/miniaturas/"+tokens[i]+".jpg";
    ui->preview_plantilla->setPixmap(QPixmap(path_miniaturas));
}

void MainWindow::on_Lista_plantillas_2_activated(int index)
{
    // Dependiendo del índice seleccionado, cambias la imagen mostrada en el QLabel
    QString indice = QString::number(index+1);
    QString filename;
    QString filename2;
    QString filename3;
    filename = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/"+indice+"/video_lista_"+indice+".txt";
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        ui->ResumenVideos->setPlainText(stream.readAll()); // Cargar el contenido del archivo en el QTextEdit
        file.close();
    }

    filename2 = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/"+indice+"/video_titulares_"+indice+".txt";
    QFile file2(filename2);
    if (file2.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file2);
        ui->ResumenTitulares->setPlainText(stream.readAll()); // Cargar el contenido del archivo en el QTextEdit
        file2.close();
    }

    filename3 = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/"+indice+"/video_titulares_large_"+indice+".txt";
    QFile file3(filename3);
    if (file3.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file3);
        ui->ResumenTitularesLarge->setPlainText(stream.readAll()); // Cargar el contenido del archivo en el QTextEdit
        file3.close();
    }

}


void MainWindow::on_Agregar_video_clicked()
{
    // Obtener el elemento seleccionado del primer QListWidget
    QList<QListWidgetItem *> selectedItems = ui->Video_lista->selectedItems();
    if (!selectedItems.isEmpty()) {
        QListWidgetItem *selectedItem = selectedItems.first();
        // Clonar el elemento seleccionado y agregarlo al segundo QListWidget
        QListWidgetItem *newItem = new QListWidgetItem(selectedItem->text());
        ui->Video_lista_escogida->addItem(newItem);
    }
}


void MainWindow::on_Borrar_video_clicked()
{
    // Obtener el elemento seleccionado del QListWidget
    QList<QListWidgetItem *> selectedItems = ui->Video_lista_escogida->selectedItems();
    if (!selectedItems.isEmpty()) {
        QListWidgetItem *selectedItem = selectedItems.first();
        // Eliminar el elemento seleccionado del QListWidget
        delete ui->Video_lista_escogida->takeItem(ui->Video_lista_escogida->row(selectedItem));
    }
}


void MainWindow::on_Generar_lista_clicked()
{
    int indice_actual = ui->Lista_plantillas_2->currentIndex();
    QString textfile;
    QString indice = QString::number(indice_actual+1);
    textfile = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/"+indice+"/video_lista_"+indice+".txt";

    QFile file(textfile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para escritura";
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < ui->Video_lista_escogida->count(); ++i) {
        out << "file '/home/seba/Desktop/Contenido/Videos/Material_Prueba/" << ui->Video_lista_escogida->item(i)->text()<< "'" << "\n";
    }

    file.close();
    qDebug() << "Elementos de la lista guardados en lista.txt";
}


void MainWindow::on_Limpiar_lista_clicked()
{
    ui->Video_lista_escogida->clear();
}


void MainWindow::on_Titulares_Lista_itemClicked(QListWidgetItem *item)
{
    QString titular_texto;
    QString file_content;
    titular_texto=item->text();
    file_content = "/home/seba/Desktop/Contenido/Titulares/Small/"+titular_texto;
    QFile file(file_content);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        ui->Titulares_Contenido->setPlainText(stream.readAll()); // Cargar el contenido del archivo en el QTextEdit
        file.close();
    }

}


void MainWindow::on_Titulares_Large_Lista_itemClicked(QListWidgetItem *item)
{
    QString titular_texto;
    QString file_content;
    titular_texto=item->text();
    file_content = "/home/seba/Desktop/Contenido/Titulares/Large/"+titular_texto;
    QFile file(file_content);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        ui->Titulares_Large_Contenido->setPlainText(stream.readAll()); // Cargar el contenido del archivo en el QTextEdit
        file.close();
    }
}


void MainWindow::on_Agregar_titular_small_clicked()
{
    QString inicio = ui->Inicio_titular_small->toPlainText();
    QString final = ui->Final_titular_small->toPlainText();
    // Obtener el elemento seleccionado del primer QListWidget
    QList<QListWidgetItem *> selectedItems = ui->Titulares_Lista->selectedItems();
    if (!selectedItems.isEmpty()) {
        QListWidgetItem *selectedItem = selectedItems.first();
        // Clonar el elemento seleccionado y agregarlo al segundo QListWidget
        QListWidgetItem *newItem = new QListWidgetItem(selectedItem->text()+"|"+inicio+"|"+final);
        ui->Titulares_small_lista_escogida->addItem(newItem);
    }
}


void MainWindow::on_Quitar_titular_small_clicked()
{
    // Obtener el elemento seleccionado del QListWidget
    QList<QListWidgetItem *> selectedItems = ui->Titulares_small_lista_escogida->selectedItems();
    if (!selectedItems.isEmpty()) {
        QListWidgetItem *selectedItem = selectedItems.first();
        // Eliminar el elemento seleccionado del QListWidget
        delete ui->Titulares_small_lista_escogida->takeItem(ui->Titulares_small_lista_escogida->row(selectedItem));
    }
}


void MainWindow::on_Limpiar_lista_titulares_small_clicked()
{
    ui->Titulares_small_lista_escogida->clear();
}

//titulares="/home/$user_name/Desktop/Digital_Signage_USM/Plantillas/$plantilla_number/video_titulares_$plantilla_number.txt"

    //titulares_large="/home/$user_name/Desktop/Digital_Signage_USM/Plantillas/$plantilla_number/video_titulares_large_$plantilla_number.txt"

void MainWindow::on_Generar_lista_titulares_small_clicked()
{
    int indice_actual = ui->Lista_plantillas_2->currentIndex();
    QString textfile;
    QString indice = QString::number(indice_actual+1);
    textfile = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/"+indice+"/video_titulares_"+indice+".txt";

    QFile file(textfile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para escritura";
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < ui->Titulares_small_lista_escogida->count(); ++i) {
        out << "/home/seba/Desktop/Contenido/Titulares/Small/" << ui->Titulares_small_lista_escogida->item(i)->text()<< "|48|150|830" << "\n";
    }

    file.close();
    qDebug() << "Elementos de la lista guardados en lista.txt";
}


void MainWindow::on_Agregar_titular_large_clicked()
{
    QString inicio = ui->Inicio_titular_large->toPlainText();
    QString final = ui->FInal_titular_large->toPlainText();
    // Obtener el elemento seleccionado del primer QListWidget
    QList<QListWidgetItem *> selectedItems = ui->Titulares_Large_Lista->selectedItems();
    if (!selectedItems.isEmpty()) {
        QListWidgetItem *selectedItem = selectedItems.first();
        // Clonar el elemento seleccionado y agregarlo al segundo QListWidget
        QListWidgetItem *newItem = new QListWidgetItem(selectedItem->text()+"|"+inicio+"|"+final);
        ui->Titulares_large_lista_escogida->addItem(newItem);
    }
}


void MainWindow::on_Borrar_titular_large_clicked()
{
    // Obtener el elemento seleccionado del QListWidget
    QList<QListWidgetItem *> selectedItems = ui->Titulares_large_lista_escogida->selectedItems();
    if (!selectedItems.isEmpty()) {
        QListWidgetItem *selectedItem = selectedItems.first();
        // Eliminar el elemento seleccionado del QListWidget
        delete ui->Titulares_large_lista_escogida->takeItem(ui->Titulares_large_lista_escogida->row(selectedItem));
    }
}


void MainWindow::on_Limpiar_lista_titulares_large_clicked()
{
    ui->Titulares_large_lista_escogida->clear();
}


void MainWindow::on_Generar_lista_titulares_large_clicked()
{
    int indice_actual = ui->Lista_plantillas_2->currentIndex();
    QString textfile;
    QString indice = QString::number(indice_actual+1);
    textfile = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/"+indice+"/video_titulares_large_"+indice+".txt";

    QFile file(textfile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para escritura";
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < ui->Titulares_large_lista_escogida->count(); ++i) {
        out << "/home/seba/Desktop/Contenido/Titulares/Large/" << ui->Titulares_large_lista_escogida->item(i)->text()<< "|40|1380|280" << "\n";
    }

    file.close();
    qDebug() << "Elementos de la lista guardados en lista.txt";
}








void MainWindow::on_Generar_Video_clicked()
{
    int indice_actual = ui->Lista_plantillas->currentIndex();
    int indice_actual_device = ui->home_dispositivo->currentIndex();
    QString scriptPath;
    QString indice_plantilla = QString::number(indice_actual+1);
    QString indice_device_string = QString::number(indice_actual_device+1);
    scriptPath = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/"+indice_plantilla+"/video_creacion_"+indice_plantilla+".sh";

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << "2";
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    //process->start("bash", QStringList() << scriptPath);
    process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath;
}


void MainWindow::on_Pause_clicked()
{
    int indice_actual = ui->Lista_plantillas->currentIndex();
    int indice_actual_device = ui->home_dispositivo->currentIndex();
    QString scriptPath;
    QString indice_string = QString::number(indice_actual+1);
    QString indice_device_string = QString::number(indice_actual_device+1);
    scriptPath = "/home/seba/Desktop/Digital_Signage_USM/play.sh";
    QString Dispositivo_seleccionado = ui->home_dispositivo->currentText();

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << indice_string << "2"<<Dispositivo_seleccionado;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    //process->start("bash", QStringList() << scriptPath);
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath;
}


void MainWindow::on_Detener_clicked()
{
    int indice_actual = ui->Lista_plantillas->currentIndex();
    int indice_actual_device = ui->home_dispositivo->currentIndex();
    QString scriptPath;
    QString indice_string = QString::number(indice_actual+1);
    QString indice_device_string = QString::number(indice_actual_device+1);
    scriptPath = "/home/seba/Desktop/Digital_Signage_USM/play.sh";
    QString Dispositivo_seleccionado = ui->home_dispositivo->currentText();

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << indice_string << "3" << Dispositivo_seleccionado;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    //process->start("bash", QStringList() << scriptPath);
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath;
}


void MainWindow::on_Mute_video_clicked()
{
    int indice_actual = ui->Lista_plantillas->currentIndex();
    int indice_actual_device = ui->home_dispositivo->currentIndex();
    QString scriptPath;
    QString indice_string = QString::number(indice_actual+1);
    QString indice_device_string = QString::number(indice_actual_device+1);
    scriptPath = "/home/seba/Desktop/Digital_Signage_USM/play.sh";
    QString Dispositivo_seleccionado = ui->home_dispositivo->currentText();

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << indice_string << "8" << Dispositivo_seleccionado;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    //process->start("bash", QStringList() << scriptPath);
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath;
}


void MainWindow::on_Retroceder_clicked()
{
    int indice_actual = ui->Lista_plantillas->currentIndex();
    int indice_actual_device = ui->home_dispositivo->currentIndex();
    QString scriptPath;
    QString indice_string = QString::number(indice_actual+1);
    QString indice_device_string = QString::number(indice_actual_device+1);
    scriptPath = "/home/seba/Desktop/Digital_Signage_USM/play.sh";
    QString Dispositivo_seleccionado = ui->home_dispositivo->currentText();

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << indice_string << "5"<<Dispositivo_seleccionado;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    //process->start("bash", QStringList() << scriptPath);
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath;
}


void MainWindow::on_Adelantar_clicked()
{
    int indice_actual = ui->Lista_plantillas->currentIndex();
    int indice_actual_device = ui->home_dispositivo->currentIndex();
    QString scriptPath;
    QString indice_string = QString::number(indice_actual+1);
    QString indice_device_string = QString::number(indice_actual_device+1);
    scriptPath = "/home/seba/Desktop/Digital_Signage_USM/play.sh";
    QString Dispositivo_seleccionado = ui->home_dispositivo->currentText();

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << indice_string << "4"<<Dispositivo_seleccionado;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    //process->start("bash", QStringList() << scriptPath);
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath;
}


void MainWindow::on_Bajar_Volumen_clicked()
{
    int indice_actual = ui->Lista_plantillas->currentIndex();
    int indice_actual_device = ui->home_dispositivo->currentIndex();
    QString scriptPath;
    QString indice_string = QString::number(indice_actual+1);
    QString indice_device_string = QString::number(indice_actual_device+1);
    scriptPath = "/home/seba/Desktop/Digital_Signage_USM/play.sh";
    QString Dispositivo_seleccionado = ui->home_dispositivo->currentText();

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << indice_string << "7" << Dispositivo_seleccionado;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    //process->start("bash", QStringList() << scriptPath);
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath;
}


void MainWindow::on_Subir_Volumen_clicked()
{
    int indice_actual = ui->Lista_plantillas->currentIndex();
    int indice_actual_device = ui->home_dispositivo->currentIndex();
    QString scriptPath;
    QString indice_string = QString::number(indice_actual+1);
    QString indice_device_string = QString::number(indice_actual_device+1);
    scriptPath = "/home/seba/Desktop/Digital_Signage_USM/play.sh";
    QString Dispositivo_seleccionado = ui->home_dispositivo->currentText();

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << indice_string << "6" << Dispositivo_seleccionado;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    //process->start("bash", QStringList() << scriptPath);
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath;
}


void MainWindow::on_lista_ubicaciones_activated(int index)
{
    ui->Dispositivos_Ubicacion->clear();
    QString Ubicacion = ui->lista_ubicaciones->currentText();
    QString filename_ubicaciones = global_path+"Digital_Signage_USM/Ubicaciones/"+Ubicacion+".txt";
    QFile file(filename_ubicaciones);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if (!line.isEmpty()){
                ui->Dispositivos_Ubicacion->addItem(line); // Cargar el contenido del archivo en el QTextEdit
            }
        }
        file.close();
    }
}


void MainWindow::on_Agregar_dispositivo_boton_clicked()
{
    //Agregamos usuario registrado
    QString user_and_ip = ui->Formato_ip_dispositivo->toPlainText();
    QListWidgetItem *newItem = new QListWidgetItem(user_and_ip);
    ui->Dispositivos_Ubicacion->addItem(newItem);

    qDebug() << "Pre-registrado usuario: "<< user_and_ip <<"\n";
}

void MainWindow::on_Quitar_titular_small_2_clicked(){}

void MainWindow::on_Guardar_cambios_dispositivos_agregados_clicked()
{
    QString textfile;
    QString ubicacion;
    ubicacion =ui->lista_ubicaciones->currentText();
    textfile = global_path+"Digital_Signage_USM/Ubicaciones/"+ubicacion+".txt";

    QFile file(textfile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para escritura";
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < ui->Dispositivos_Ubicacion->count(); ++i) {
        out << ui->Dispositivos_Ubicacion->item(i)->text()+"\n";
    }

    file.close();

    //Crear archivos para dispositivo
    QString ruta_file_contenido_dispositivos = global_path+"Contenido_Dispositivos/";
    QDir dir(ruta_file_contenido_dispositivos);
    if (!dir.exists()) {
        qDebug() << "El directorio no existe:" << ruta_file_contenido_dispositivos;
        return;
    }

    QChar delimiter = '@';

    for (int i = 0; i < ui->Dispositivos_Ubicacion->count(); ++i) {
        QListWidgetItem *item = ui->Dispositivos_Ubicacion->item(i);
        if (item){
            QStringList item_name = item->text().split(delimiter);
            QString tmp_string = ruta_file_contenido_dispositivos + item_name[0] +".txt";
            qDebug() << tmp_string <<"\n";
            QFile archivo(tmp_string);
            // Comprobar si el archivo ya existe
            if (archivo.exists()) {
                qDebug() << "El archivo ya existe, se saltará:" << tmp_string;
                continue;
            }
            // Intentar abrir el archivo en modo de escritura
            if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
                qDebug() << "No se puede abrir el archivo para escritura:" << archivo.errorString();
                file.close();
                continue;
            }
        }
    }

    qDebug() << "Cambios Guardados Satisfactoriamente\n";
    ui->Lista_plantillas->clear();
    ui->home_dispositivo->clear();
}




void MainWindow::on_Quitar_dispositivo_lista_ubicacion_clicked()
{
    // Obtener el elemento seleccionado del QListWidget
    QList<QListWidgetItem *> selectedItems = ui->Dispositivos_Ubicacion->selectedItems();
    if (!selectedItems.isEmpty()) {
        QListWidgetItem *selectedItem = selectedItems.first();
        qDebug() << "Pre-eliminado usuario: "<< selectedItem->text() <<"\n";
        // Eliminar el elemento seleccionado del QListWidget
        delete ui->Dispositivos_Ubicacion->takeItem(ui->Dispositivos_Ubicacion->row(selectedItem));
    }

}


void MainWindow::on_home_ubicacion_activated(int index)
{
    //Limpieza
    QString video_simbolo = global_path+"Digital_Signage_USM/Material_Interfaz/video_simbolo.jpg";
    ui->preview_plantilla->setPixmap(QPixmap(video_simbolo));
    ui->home_dispositivo->clear();
    ui->Lista_plantillas->clear();

    //Delimitador
    QChar delimiter = '@';

    //Leemos texto
    QString Ubicacion = ui->home_ubicacion->currentText()+".txt";
    qDebug() << "Ubication:" << ui->home_ubicacion->currentText() << "\n";
    QString filename_ubicaciones = global_path+"Digital_Signage_USM/Ubicaciones/"+Ubicacion;
    QFile file(filename_ubicaciones);

    //Agregamos Dispositivos Asociados a la ubicacion Seleccionada
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList tmp_string = line.split(delimiter);
            if (!line.isEmpty()){
                ui->home_dispositivo->addItem(tmp_string[0],tmp_string[1]); // user@ip
            }
        }
        file.close();
    }

}


void MainWindow::on_home_dispositivo_activated(int index)
{
    ui->Lista_plantillas->clear();
    QString video_simbolo = global_path +"Digital_Signage_USM/Material_Interfaz/video_simbolo.jpg";
    ui->preview_plantilla->setPixmap(QPixmap(video_simbolo));

    //Feedback
    QString ip = ui->home_dispositivo->itemData(index).toString();
    QString user = ui->home_dispositivo->currentText();
    qDebug() << "User:" << user << ", IP:" << ip << "\n";

    // Divide la cadena usando el delimitador
    QString name_device= ui->home_dispositivo->currentText();
    QChar delimiter = '@';
    QStringList tokens = name_device.split(delimiter);

    //Agregar videos asignados
    int i=0;
    QString path_lista_lugares = global_path+"Contenido_Dispositivos/"+tokens[i]+"/videos";
    QDir dir4(path_lista_lugares);
    QStringList files_lista_lugares = dir4.entryList(QDir::Files);
    QStringList modified_files_lista_lugares = removeExtensions(files_lista_lugares, '.');
    ui->Lista_plantillas->addItems(modified_files_lista_lugares);
}


void MainWindow::on_asignacion_dispositivos_activated(int index)
{
    //ui->asignacion_dispositivos->clear();
    QString device = ui->asignacion_dispositivos->currentText();

    // Imprimir todas las subcadenas
    qDebug() << "Dispositivo seleccionado: "<<device << " \n";

    ui->Gestion_Contenido_Disponible->clear();


    //Delimitador
    QChar delimiter = '.';

    //Leemos texto
    QString device_file = device+".txt";
    qDebug() << "Ubication:" << ui->home_ubicacion->currentText() << "\n";
    QString path_device_file = global_path+"Contenido_Dispositivos/"+device_file;
    QFile file(path_device_file);

    //Agregamos Dispositivos Asociados a la ubicacion Seleccionada
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList tmp_string = line.split(delimiter);
            if (!line.isEmpty()){
                ui->Gestion_Contenido_Disponible->addItem(new QListWidgetItem(tmp_string[0])); // user@ip
            }
        }
        file.close();
    }

    ui->lista_asignar_contenido->clear();
    QString path_lista_lugares = global_path+"Contenido_ELO308/videos";
    QDir dir4(path_lista_lugares);
    QStringList files_lista_lugares = dir4.entryList(QDir::Files);
    // Quitar las extensiones de los archivos usando la función, especificando el carácter de corte
    QStringList modified_files_lista_lugares = removeExtensions(files_lista_lugares, '.');
    ui->lista_asignar_contenido->addItems(modified_files_lista_lugares);


}

// Función para quitar la parte del string después del carácter de corte en una QStringList
QStringList MainWindow::removeExtensions(const QStringList &fileList, const QChar &cutChar) {
    QStringList modifiedList;

    for (const QString &filename : fileList) {
        int cutPosition = filename.lastIndexOf(cutChar);
        QString modifiedFilename = filename;

        if (cutPosition != -1) {
            modifiedFilename = filename.left(cutPosition);
        }

        modifiedList.append(modifiedFilename);
    }

    return modifiedList;
}

void MainWindow::on_boton_admin_clicked()
{
    ui->lista_ubicaciones->clear();
    ui->home_ubicacion->clear();
    ui->home_dispositivo->clear();
    ui->Lista_plantillas->clear();

    QString admin = ui->texto_admin->toPlainText();
    qDebug() << "Admin:" << admin << "\n";
    global_path = "/home/"+admin+"/Desktop/";

    QString path_lista_lugares = global_path+"Digital_Signage_USM/Ubicaciones";
    QDir dir4(path_lista_lugares);
    QStringList files_lista_lugares = dir4.entryList(QDir::Files);
    // Quitar las extensiones de los archivos usando la función, especificando el carácter de corte
    QStringList modified_files_lista_lugares = removeExtensions(files_lista_lugares, '.');
    ui->lista_ubicaciones->addItems(modified_files_lista_lugares);
    ui->home_ubicacion->addItems(modified_files_lista_lugares);


}


void MainWindow::on_Actualizar_Lista_Dispositivos_clicked()
{
    //llenar lista con contenido disponible
    QString path_asignacion_dispositivos = global_path+"Contenido_Dispositivos";
    QDir dir4(path_asignacion_dispositivos);
    QStringList files_asignacion_dispositivos = dir4.entryList(QDir::Files);
    // Quitar las extensiones de los archivos usando la función, especificando el carácter de corte
    QStringList modified_files_asignacion_dispositivos = removeExtensions(files_asignacion_dispositivos, '.');
    ui->asignacion_dispositivos->addItems(modified_files_asignacion_dispositivos);
}


void MainWindow::on_lista_asignar_contenido_activated(int index)
{
    //Recuperamos video seleccionado
    QString name_video= ui->lista_asignar_contenido->currentText();

    qDebug() << "Video:" << name_video << "\n";

    //Miniatura plantilla
    QString path_miniaturas = global_path+"Contenido_ELO308/miniaturas/"+name_video+".jpg";
    ui->miniatura_asignar_contenido->setPixmap(QPixmap(path_miniaturas));
}

