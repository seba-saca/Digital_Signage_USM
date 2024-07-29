#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "variables_globales.h"
#include <QCoreApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

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


    QString filename_ubicaciones = "/home/seba/Desktop/Contenido_ELO308/Videos";
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
    scriptPath = global_path+"Digital_Signage_USM/play.sh";

    QString indice_string = QString::number(indice_actual+1);
    QString indice_device_string = QString::number(indice_actual_device+1);
    QString Dispositivo_seleccionado = ui->home_dispositivo->currentText();

    QString scriptPath_destino = "/home/"+Dispositivo_seleccionado+"/Desktop/"+Dispositivo_seleccionado+"/control_device.sh";
    QString scriptPath_videos = "/home/"+Dispositivo_seleccionado+"/Desktop/"+Dispositivo_seleccionado+"/videos";
    scriptPath = global_path+"Digital_Signage_USM/play.sh";

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;

    QString name_video= ui->Lista_plantillas->currentText();

    arguments << user_ip_dispositivo << scriptPath_destino << scriptPath_videos << name_video << "1";
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);

    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath << arguments;


}

void MainWindow::on_sincronizar_clicked()
{
    // Miniatura plantilla
    QString path_miniaturas = global_path+"Digital_Signage_USM/Material_Interfaz/";
    QString loading_imagen = path_miniaturas+"loading.png";
    QPixmap mapeo(loading_imagen);
    ui->label_sync_logo->setPixmap(mapeo);

    QString error_imagen = path_miniaturas+"error.png";
    QString ready_imagen = path_miniaturas+"ready.png";



    // Inicia un temporizador para ejecutar el script después de actualizar la interfaz
    QTimer::singleShot(100, this, [this, error_imagen, ready_imagen]() {
        QString scriptPath;
        scriptPath = global_path+"Digital_Signage_USM/transferir.sh";

        // Lista de argumentos que deseas pasar al script
        QStringList arguments;
        QString path_origen = global_path+"Contenido_ELO308/Videos";
        QString path_destino = "/home/"+user_sincro+"/Desktop/"+user_sincro+"/videos";
        QString path_file_contenido = global_path+"Contenido_Dispositivos/"+user_sincro+".txt";

        QString path_file_sincro = global_path+"Digital_Signage_USM/control_device.sh";
        QString path_destino_sincro = "/home/"+user_sincro+"/Desktop/"+user_sincro;

        arguments << path_origen << path_destino << user_sincro << ip_sincro << path_file_contenido << path_file_sincro << path_destino_sincro;
        qDebug() << scriptPath << arguments;

        QProcess *process = new QProcess(this);
        // Asignamos el script y los argumentos al proceso
        process->start(scriptPath, arguments);
        process->waitForFinished(); // Espera a que el proceso termine antes de continuar


        int exitCode = process->exitCode();
        bool isConnected = (exitCode == 0);

        QString imagePath = isConnected ? ready_imagen : error_imagen;
        QPixmap pixmap(imagePath);
        ui->label_sync_logo->setPixmap(pixmap);
    });
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
    user_ip_dispositivo = user+"@"+ip;
    qDebug() << "User:" << user << ", IP:" << ip << "\n";

    // Divide la cadena usando el delimitador
    QString name_device= ui->home_dispositivo->currentText();

    //Agregar videos asignados
    QString path_lista_lugares = global_path+"Contenido_Dispositivos/"+name_device+".txt";
    qDebug() << "Path lugar: " << path_lista_lugares << "\n";

    QFile file(path_lista_lugares);

    //Agregamos Dispositivos Asociados a la ubicacion Seleccionada
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if (!line.isEmpty()){
                ui->Lista_plantillas->addItem(line); // videos
            }
        }
        file.close();
    }

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
    QString path_lista_lugares = global_path+"Contenido_ELO308/Videos";
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
    ui->lista_sincronizacion_ubicacion->addItems(modified_files_lista_lugares);

    //Miniatura plantilla
    QString path_miniaturas = global_path+"Digital_Signage_USM/Material_Interfaz/";
    QString film_imagen = path_miniaturas+"film.png";
    QString sync = path_miniaturas+"sync.png";
    ui->label_subir_video->setPixmap(QPixmap(film_imagen));
    ui->label_sync_logo->setPixmap(QPixmap(sync));
}


void MainWindow::on_Actualizar_Lista_Dispositivos_clicked()
{
    ui->asignacion_dispositivos->clear();
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


void MainWindow::on_boton_asignar_contenido_clicked()
{
    //Agregamos usuario registrado
    QString video_selected = ui->lista_asignar_contenido->currentText();
    QListWidgetItem *newItem = new QListWidgetItem(video_selected);
    ui->Gestion_Contenido_Disponible->addItem(newItem);

    qDebug() << "Pre-registrado video: "<< video_selected <<"\n";
}


void MainWindow::on_Quitar_contenido_asignado_clicked()
{
    // Obtener el elemento seleccionado del QListWidget
    QList<QListWidgetItem *> selectedItems = ui->Gestion_Contenido_Disponible->selectedItems();
    if (!selectedItems.isEmpty()) {
        QListWidgetItem *selectedItem = selectedItems.first();
        qDebug() << "Pre-eliminado video: "<< selectedItem->text() <<"\n";
        // Eliminar el elemento seleccionado del QListWidget
        delete ui->Gestion_Contenido_Disponible->takeItem(ui->Gestion_Contenido_Disponible->row(selectedItem));
    }
}


void MainWindow::on_Guardar_cambios_contenido_asignado_clicked()
{
    QString textfile;
    QString device;
    device =ui->asignacion_dispositivos->currentText();
    textfile = global_path+"Contenido_Dispositivos/"+device+".txt";

    QFile file(textfile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para escritura";
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < ui->Gestion_Contenido_Disponible->count(); ++i) {
        out << ui->Gestion_Contenido_Disponible->item(i)->text()+"\n";
    }

    qDebug() << "Cambios Guardados Satisfactoriamente: " << device<<"\n";

    file.close();
}


void MainWindow::on_Subir_video_clicked()
{
    ui->label_estado_subir_video->clear();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir archivo"), global_path, tr("Todos los archivos (*)"));
    path_subir_video = fileName;
    QString fileBaseName;
    QStringList tmp_string;

    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        fileBaseName = fileInfo.fileName(); // Solo el nombre del archivo
        tmp_string = fileBaseName.split('.');
        name_subir_video = fileBaseName;
    }

    QString scriptPath;
    QString imagenPath = global_path+"Contenido_ELO308/miniaturas/"+tmp_string[0]+".jpg";
    scriptPath = global_path+"Digital_Signage_USM/miniaturas_gen.sh";

    QStringList arguments;
    arguments << fileName << imagenPath;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    //Miniatura plantilla
    QString path_miniaturas = imagenPath;
    ui->label_subir_video->setPixmap(QPixmap(path_miniaturas));
    qDebug() << scriptPath << arguments;


}


void MainWindow::on_Guardar_subir_video_clicked()
{

    ui->lista_asignar_contenido->clear();

    QString scriptPath;
    scriptPath = global_path+"Digital_Signage_USM/copy_video.sh";
    QString Path_video_destino = global_path+"Contenido_ELO308/Videos/"+name_subir_video;

    //Miniatura plantilla
    QString path_miniaturas = global_path+"Digital_Signage_USM/Material_Interfaz/";
    QString error_imagen = path_miniaturas+"loading.png";
    QString ready_imagen = path_miniaturas+"ready.png";
    ui->label_estado_subir_video->setPixmap(QPixmap(error_imagen));

    QStringList arguments;
    arguments << path_subir_video << Path_video_destino;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    process->waitForFinished(); // Espera a que el proceso termine antes de continuar

    ui->label_estado_subir_video->setPixmap(QPixmap(ready_imagen));

}


void MainWindow::on_actualizar_sincronizacion_lista_devices_clicked()
{
    ui->lista_sincronizacion_devices->clear();
    //Delimitador
    QChar delimiter = '@';

    //Leemos texto
    QString Ubicacion = ui->lista_sincronizacion_ubicacion->currentText()+".txt";
    qDebug() << "Ubication:" << ui->lista_sincronizacion_ubicacion->currentText() << "\n";
    QString filename_ubicaciones = global_path+"Digital_Signage_USM/Ubicaciones/"+Ubicacion;
    QFile file(filename_ubicaciones);

    //Agregamos Dispositivos Asociados a la ubicacion Seleccionada
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList tmp_string = line.split(delimiter);
            if (!line.isEmpty()){
                ui->lista_sincronizacion_devices->addItem(tmp_string[0],tmp_string[1]); // user@ip
            }
        }
        file.close();
    }
}


void MainWindow::on_lista_sincronizacion_ubicacion_activated(int index)
{
    ui->lista_sincronizacion_devices->clear();
    //Delimitador
    QChar delimiter = '@';
    //Leemos texto
    QString Ubicacion = ui->lista_sincronizacion_ubicacion->currentText()+".txt";
    qDebug() << "Ubication:" << ui->lista_sincronizacion_ubicacion->currentText() << "\n";
    QString filename_ubicaciones = global_path+"Digital_Signage_USM/Ubicaciones/"+Ubicacion;
    QFile file(filename_ubicaciones);

    //Agregamos Dispositivos Asociados a la ubicacion Seleccionada
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList tmp_string = line.split(delimiter);
            if (!line.isEmpty()){
                ui->lista_sincronizacion_devices->addItem(tmp_string[0],tmp_string[1]); // user@ip
            }
        }
        file.close();
    }
}


void MainWindow::on_sincronizar_check_dispo_clicked()
{

    // Miniatura plantilla
    QString path_miniaturas = global_path+"Digital_Signage_USM/Material_Interfaz/";
    QString loading_imagen = path_miniaturas+"loading.png";
    QPixmap mapeo(loading_imagen);
    ui->Estado_Disponibilidad->setPixmap(mapeo);

    QString error_imagen = path_miniaturas+"error.png";
    QString ready_imagen = path_miniaturas+"ready.png";



    // Inicia un temporizador para ejecutar el script después de actualizar la interfaz
    QTimer::singleShot(100, this, [this, error_imagen, ready_imagen]() {
        QProcess process;
        QString script = "ping -c 1 "+ip_sincro; // Reemplaza con la IP de tu dispositivo
        qDebug() << script << "\n";

        process.start("bash", QStringList() << "-c" << script);
        process.waitForFinished();

        int exitCode = process.exitCode();
        bool isConnected = (exitCode == 0);

        QString imagePath = isConnected ? ready_imagen : error_imagen;
        QPixmap pixmap(imagePath);
        ui->Estado_Disponibilidad->setPixmap(pixmap);
    });
}


void MainWindow::on_lista_sincronizacion_devices_activated(int index)
{
    ui->Estado_Disponibilidad->clear();
    //Feedback
    ip_sincro = ui->lista_sincronizacion_devices->itemData(index).toString();
    user_sincro = ui->lista_sincronizacion_devices->currentText();
    qDebug() << "User:" << user_sincro << ", IP:" << ip_sincro << "\n";
}


void MainWindow::on_Lista_plantillas_Centro_Edicion_activated(int index)
{
    ui->Lista_plantillas_Centro_Edicion_Sector->clear();

    QChar delimiter = '.';

    //Recuperamos video seleccionado
    QString name_plantilla= ui->Lista_plantillas_Centro_Edicion->currentText();
    qDebug() << "Seleccionada:" << name_plantilla << "\n";

    //Miniatura plantilla
    QString path_miniaturas = global_path+"Digital_Signage_USM/Plantillas/Imagenes/Overlays/"+name_plantilla+"/fondo.jpg";
    qDebug() << "Path:" << path_miniaturas << "\n";

    //Agregar videos asignados
    QString path_lista_sectores = global_path+"Digital_Signage_USM/Plantillas/Imagenes/Overlays/"+name_plantilla+"/Sectores";
    qDebug() << "Path sectores: " << path_lista_sectores << "\n";

    QDir directory(path_lista_sectores);

    // Obtener la lista de archivos en el directorio
    QStringList fileList = directory.entryList(QDir::Files);

    // Llenar el QComboBox con los nombres de los archivos
    for (const QString &filename : fileList) {
        QStringList tmp_string = filename.split(delimiter);
        ui->Lista_plantillas_Centro_Edicion_Sector->addItem(tmp_string[0]);
    }

    ui->Imagen_Centro_Edicion->setPixmap(QPixmap(path_miniaturas));
}


void MainWindow::on_Lista_plantillas_Centro_Edicion_Sector_activated(int index)
{
    ui->Contenido_Asignado_Centro_Edicion->clear();
    // Divide la cadena usando el delimitador

    QString name_plantilla= ui->Lista_plantillas_Centro_Edicion->currentText();
    QString name_sector_selected= ui->Lista_plantillas_Centro_Edicion_Sector->currentText();
    //Miniatura plantilla
    QString path_miniaturas = global_path+"Digital_Signage_USM/Plantillas/Imagenes/Overlays/"+name_plantilla+"/Sectores/"+name_sector_selected+".jpg";
    qDebug() << "Path:" << path_miniaturas << "\n";

    ui->Imagen_Centro_Edicion->setPixmap(QPixmap(path_miniaturas));


    ui->Lista_Asignar_Contenido_Centro_Edicion->clear();
    QChar delimiter = '.';
    QChar delimiter2 = '-';
    //PONER UN ARCHIVO APARTE EN LA CREACION DE CONTENIDO
    //Agregar videos asignados
    QStringList name_sector = name_sector_selected.split(delimiter2);
    QString path_lista_contenido = global_path+"Contenido_ELO308/"+name_sector[0];
    qDebug() << "Path contenido: " << path_lista_contenido << "\n";

    QDir directory(path_lista_contenido);

    // Obtener la lista de archivos en el directorio
    QStringList fileList = directory.entryList(QDir::Files);

    // Llenar el QComboBox con los nombres de los archivos
    for (const QString &filename : fileList) {
        QStringList tmp_string = filename.split(delimiter);
        qDebug() << "tmp_string[0]: " << tmp_string[0] << "tmp_string[1]: " << tmp_string[1] << "\n";
        ui->Lista_Asignar_Contenido_Centro_Edicion->addItem(tmp_string[0],tmp_string[1]);
    }

}


void MainWindow::on_Lista_Asignar_Contenido_Centro_Edicion_activated(int index)
{
    ui->Miniatura_Asignar_Video_Centro_Edicion->clear();
    ui->CONTENIDO_TITULO_CENTRO_EDICION->clear();

    QString contenido_selected= ui->Lista_Asignar_Contenido_Centro_Edicion->currentText();
    QString name_sector_selected= ui->Lista_plantillas_Centro_Edicion_Sector->currentText();
    QChar delimiter2 = '-';
    QStringList name_sector = name_sector_selected.split(delimiter2);
    QString path_lista_contenido = global_path+"Contenido_ELO308/"+name_sector[0];
    qDebug() << "Path contenido: " << path_lista_contenido << "\n";

    //Feedback
    QString contenido = ui->Lista_Asignar_Contenido_Centro_Edicion->currentText();
    QString extension = ui->Lista_Asignar_Contenido_Centro_Edicion->itemData(index).toString();

    if (extension=="txt"){
        qDebug() << "Es texto \n";
        QString titular_texto;
        QString file_content;
        //titular_texto=item->text();
        file_content = path_lista_contenido+"/"+contenido+"."+extension;
        qDebug() << "Path contenido: " << file_content << "\n";
        QFile file(file_content);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            ui->CONTENIDO_TITULO_CENTRO_EDICION->setPlainText(stream.readAll()); // Cargar el contenido del archivo en el QTextEdit
            file.close();
        }
    }

    else {
        qDebug() << "Es video \n";
        QString miniatura_path = global_path+"Contenido_ELO308/miniaturas/"+contenido+".jpg";
        ui->Miniatura_Asignar_Video_Centro_Edicion->setPixmap(QPixmap(miniatura_path));
    }

}


void MainWindow::on_Boton_asignar_contenido_Centro_Edicion_clicked()
{
    QString Contenido_CE = ui->Lista_Asignar_Contenido_Centro_Edicion->currentText();
    int index = ui->Lista_Asignar_Contenido_Centro_Edicion->currentIndex();
    QString extension = ui->Lista_Asignar_Contenido_Centro_Edicion->itemData(index).toString();
    QListWidgetItem *newItem = new QListWidgetItem(Contenido_CE+"."+extension);
    ui->Contenido_Asignado_Centro_Edicion->addItem(newItem);
}


void MainWindow::on_Quitar_Contenido_Asignado_Centro_Edicion_clicked()
{
    // Obtener el elemento seleccionado del QListWidget
    QList<QListWidgetItem *> selectedItems = ui->Contenido_Asignado_Centro_Edicion->selectedItems();
    if (!selectedItems.isEmpty()) {
        QListWidgetItem *selectedItem = selectedItems.first();
        // Eliminar el elemento seleccionado del QListWidget
        delete ui->Contenido_Asignado_Centro_Edicion->takeItem(ui->Contenido_Asignado_Centro_Edicion->row(selectedItem));
    }
}


void MainWindow::on_Guardar_contenido_Centro_EDICION_clicked()
{
    //ui->Miniatura_Asignar_Video_Centro_Edicion->clear();
    //ui->CONTENIDO_TITULO_CENTRO_EDICION->clear();
    QString Plantilla = ui->Lista_plantillas_Centro_Edicion->currentText();
    QString name_sector_selected= ui->Lista_plantillas_Centro_Edicion_Sector->currentText();
    QString contenido_selected= ui->Lista_Asignar_Contenido_Centro_Edicion->currentText();

    QString path_lista_contenido = global_path+"Contenido_ELO308/Plantillas/"+Plantilla+"/"+name_sector_selected+".txt";
    qDebug() << "Path archivo txt lista: " << path_lista_contenido << "\n";


    QFile file(path_lista_contenido);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para escritura";
        return;
    }

    QTextStream out(&file);

    //Feedback
    int index = ui->Lista_Asignar_Contenido_Centro_Edicion->currentIndex();
    QString contenido = ui->Lista_Asignar_Contenido_Centro_Edicion->currentText();
    QString extension = ui->Lista_Asignar_Contenido_Centro_Edicion->itemData(index).toString();

    if (extension=="txt"){
        qDebug() << "Es texto \n" << name_sector_selected;
        QChar delimiter2 = '-';
        QStringList name_sector = name_sector_selected.split(delimiter2);
        QString path_lista_contenido = global_path+"Contenido_ELO308/"+name_sector[0];
        for (int i = 0; i < ui->Contenido_Asignado_Centro_Edicion->count(); ++i) {
            out <<global_path<<"Contenido_ELO308/"+name_sector[0]+"/" << ui->Contenido_Asignado_Centro_Edicion->item(i)->text()<< "\n";
        }
    }

    else {
        qDebug() << "Es video \n";

        for (int i = 0; i < ui->Contenido_Asignado_Centro_Edicion->count(); ++i) {
            out << "file '"<<global_path<<"Contenido_ELO308/Videos/" << ui->Contenido_Asignado_Centro_Edicion->item(i)->text()<< "'" << "\n";
        }
    }

    file.close();
    qDebug() << "Elementos de la lista guardados en lista.txt";
    ////////////////////////////////////////////////

}

