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
    QString video_simbolo = "/home/saca/Desktop/Digital_Signage_USM/Material_Interfaz/video_player.png";
    ui->preview_plantilla->setPixmap(QPixmap(video_simbolo));

    QString filename_ubicaciones = "/home/seba/Desktop/Contenido_ELO308/Videos";
    QDir dir5(filename_ubicaciones);
    QStringList files_lista_ubicaciones = dir5.entryList(QDir::Files);
    ui->Gestion_Contenido_Disponible->addItems(files_lista_ubicaciones);

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



void MainWindow::on_sincronizar_clicked()
{
    // Miniatura plantilla
    QString path_miniaturas = global_path+"Digital_Signage_USM/Material_Interfaz/";
    QString loading_imagen = path_miniaturas+"loading.png";
    QPixmap mapeo(loading_imagen);
    ui->label_sync_logo->setPixmap(mapeo);
    QApplication::processEvents();

    QString error_imagen = path_miniaturas+"error.png";
    QString ready_imagen = path_miniaturas+"ready.png";

    QString scriptPath = global_path+"Digital_Signage_USM/transferir.sh";

    QStringList arguments;
    QString path_origen = global_path+"Contenido_ELO308/Videos";
    QString path_destino = "/home/"+user_sincro+"/Desktop/"+user_sincro+"/videos";
    QString path_file_contenido = global_path+"Contenido_Dispositivos/"+user_sincro+".txt";

    QString path_file_sincro = global_path+"Digital_Signage_USM/control_device.sh";
    QString path_destino_sincro = "/home/"+user_sincro+"/Desktop/"+user_sincro;
    QString path_file_log = global_path+"Contenido_Dispositivos/Logs/"+user_sincro+"_log.txt";

    arguments << path_origen << path_destino << user_sincro << ip_sincro << path_file_contenido << path_file_sincro << path_destino_sincro << path_file_log;
    qDebug() << scriptPath << arguments;

    QProcess *process = new QProcess(this);

    // Conectar señales a las funciones correspondientes
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus){
        if (exitCode == 0 && exitStatus == QProcess::NormalExit){
            ui->label_sync_logo->setPixmap(QPixmap(ready_imagen));
        } else {
            ui->label_sync_logo->setPixmap(QPixmap(error_imagen));
        }

        QFile file = global_path+"Contenido_Dispositivos/Logs/"+user_sincro+"_log.txt";
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            ui->Estado_Disponibilida_Contenido->setPlainText(stream.readAll());
            file.close();
        }
    });

    connect(process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error){
        ui->label_sync_logo->setPixmap(QPixmap(error_imagen));
        qDebug() << "Error en la ejecución del proceso:" << error;
    });

    // Iniciar el proceso de manera asíncrona
    process->start(scriptPath, arguments);
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

void MainWindow::on_Start_clicked()
{

    int indice_actual = ui->Lista_plantillas->currentIndex();
    int indice_actual_device = ui->home_dispositivo->currentIndex();
    ui->Inicio_feedback->setText("Mandando instrucción"+ui->Start->text());
    QString scriptPath;
    scriptPath = global_path+"Digital_Signage_USM/play.sh";

    QString indice_string = QString::number(indice_actual+1);
    QString indice_device_string = QString::number(indice_actual_device+1);
    QString Dispositivo_seleccionado = ui->home_dispositivo->currentText();

    QString scriptPath_destino = "/home/"+Dispositivo_seleccionado+"/Desktop/"+Dispositivo_seleccionado+"/control_device.sh";
    QString scriptPath_videos = "/home/"+Dispositivo_seleccionado+"/Desktop/"+Dispositivo_seleccionado+"/videos";
    QString ruta_status = "/home/"+Dispositivo_seleccionado+"/Desktop/"+Dispositivo_seleccionado;
    scriptPath = global_path+"Digital_Signage_USM/play.sh";

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;

    QString name_video= ui->Lista_plantillas->currentText();

    arguments << user_ip_dispositivo << scriptPath_destino << scriptPath_videos << name_video << "1" << ruta_status;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    int exitCode = process->exitCode();
    if (exitCode == 0){
        //Exito
        ui->Inicio_feedback->setText("Instrucción *"+ui->Start->text()+"* enviada satisfactoriamente");
    }
    else {
        ui->Inicio_feedback->setText("No se envió instrucción *"+ui->Start->text()+"* solicitada");
    }
    qDebug() << scriptPath << arguments;
}

void MainWindow::on_Pause_clicked()
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
    QString ruta_status = "/home/"+Dispositivo_seleccionado+"/Desktop/"+Dispositivo_seleccionado;
    scriptPath = global_path+"Digital_Signage_USM/play.sh";

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;

    QString name_video= ui->Lista_plantillas->currentText();

    arguments << user_ip_dispositivo << scriptPath_destino << scriptPath_videos << name_video << "2" << ruta_status;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    int exitCode = process->exitCode();
    if (exitCode == 0){
        //Exito
        ui->Inicio_feedback->setText("Instrucción *"+ui->Pause->text()+"* enviada satisfactoriamente");
    }
    else {
        ui->Inicio_feedback->setText("No se envió instrucción *"+ui->Pause->text()+"* solicitada");
    }
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath << arguments;
}

void MainWindow::on_Detener_clicked()
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
    QString ruta_status = "/home/"+Dispositivo_seleccionado+"/Desktop/"+Dispositivo_seleccionado;

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;

    QString name_video= ui->Lista_plantillas->currentText();

    arguments << user_ip_dispositivo << scriptPath_destino << scriptPath_videos << name_video << "3" << ruta_status;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    int exitCode = process->exitCode();
    if (exitCode == 0){
        //Exito
        ui->Inicio_feedback->setText("Instrucción *"+ui->Detener->text()+"* enviada satisfactoriamente");
    }
    else {
        ui->Inicio_feedback->setText("No se envió instrucción *"+ui->Detener->text()+"* solicitada");
    }
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath << arguments;
}

void MainWindow::on_Mute_video_clicked()
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
    QString ruta_status = "/home/"+Dispositivo_seleccionado+"/Desktop/"+Dispositivo_seleccionado;
    scriptPath = global_path+"Digital_Signage_USM/play.sh";

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;

    QString name_video= ui->Lista_plantillas->currentText();

    arguments << user_ip_dispositivo << scriptPath_destino << scriptPath_videos << name_video << "8" << ruta_status;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    int exitCode = process->exitCode();
    if (exitCode == 0){
        //Exito
        ui->Inicio_feedback->setText("Instrucción *"+ui->Mute_video->text()+"* enviada satisfactoriamente");
    }
    else {
        ui->Inicio_feedback->setText("No se envió instrucción *"+ui->Mute_video->text()+"* solicitada");
    }
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath << arguments;
}

void MainWindow::on_Retroceder_clicked()
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
    QString ruta_status = "/home/"+Dispositivo_seleccionado+"/Desktop/"+Dispositivo_seleccionado;
    scriptPath = global_path+"Digital_Signage_USM/play.sh";

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;

    QString name_video= ui->Lista_plantillas->currentText();

    arguments << user_ip_dispositivo << scriptPath_destino << scriptPath_videos << name_video << "5" << ruta_status;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    int exitCode = process->exitCode();
    if (exitCode == 0){
        //Exito
        ui->Inicio_feedback->setText("Instrucción *"+ui->Retroceder->text()+"* enviada satisfactoriamente");
    }
    else {
        ui->Inicio_feedback->setText("No se envió instrucción *"+ui->Retroceder->text()+"* solicitada");
    }
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath << arguments;
}

void MainWindow::on_Adelantar_clicked()
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
    QString ruta_status = "/home/"+Dispositivo_seleccionado+"/Desktop/"+Dispositivo_seleccionado;
    scriptPath = global_path+"Digital_Signage_USM/play.sh";

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;

    QString name_video= ui->Lista_plantillas->currentText();

    arguments << user_ip_dispositivo << scriptPath_destino << scriptPath_videos << name_video << "4" << ruta_status;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    int exitCode = process->exitCode();
    if (exitCode == 0){
        //Exito
        ui->Inicio_feedback->setText("Instrucción *"+ui->Adelantar->text()+"* enviada satisfactoriamente");
    }
    else {
        ui->Inicio_feedback->setText("No se envió instrucción *"+ui->Adelantar->text()+"* solicitada");
    }
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath << arguments;
}

void MainWindow::on_Bajar_Volumen_clicked()
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
    QString ruta_status = "/home/"+Dispositivo_seleccionado+"/Desktop/"+Dispositivo_seleccionado;
    scriptPath = global_path+"Digital_Signage_USM/play.sh";

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;

    QString name_video= ui->Lista_plantillas->currentText();

    arguments << user_ip_dispositivo << scriptPath_destino << scriptPath_videos << name_video << "7" << ruta_status;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    int exitCode = process->exitCode();
    if (exitCode == 0){
        //Exito
        ui->Inicio_feedback->setText("Instrucción *"+ui->Bajar_Volumen->text()+"* enviada satisfactoriamente");
    }
    else {
        ui->Inicio_feedback->setText("No se envió instrucción *"+ui->Bajar_Volumen->text()+"* solicitada");
    }
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath << arguments;
}

void MainWindow::on_Subir_Volumen_clicked()
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
    QString ruta_status = "/home/"+Dispositivo_seleccionado+"/Desktop/"+Dispositivo_seleccionado;
    scriptPath = global_path+"Digital_Signage_USM/play.sh";

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;

    QString name_video= ui->Lista_plantillas->currentText();

    arguments << user_ip_dispositivo << scriptPath_destino << scriptPath_videos << name_video << "6" << ruta_status;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    int exitCode = process->exitCode();
    if (exitCode == 0){
        //Exito
        ui->Inicio_feedback->setText("Instrucción *"+ui->Subir_Volumen->text()+"* enviada satisfactoriamente");
    }
    else {
        ui->Inicio_feedback->setText("No se envió instrucción *"+ui->Subir_Volumen->text()+"* solicitada");
    }
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath << arguments;
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
    QString video_simbolo = global_path+"Digital_Signage_USM/Material_Interfaz/video_player.png";
    ui->preview_plantilla->setPixmap(QPixmap(video_simbolo));
    ui->home_dispositivo->clear();
    ui->Lista_plantillas->clear();
    ui->Inicio_feedback->clear();

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
    ui->Inicio_Status_Contenido->clear();
    ui->Lista_plantillas->clear();
    ui->Inicio_feedback->clear();
    QString video_simbolo = global_path +"Digital_Signage_USM/Material_Interfaz/video_player.png";
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
    ui->miniatura_asignar_contenido->setPixmap(QPixmap(global_path+"Digital_Signage_USM/Material_Interfaz/video_player.png"));
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
    admin_nombre = admin;
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

    QString video_simbolo = global_path+"Digital_Signage_USM/Material_Interfaz/video_player.png";
    ui->preview_plantilla->setPixmap(QPixmap(video_simbolo));

    ui->Logo_ELO->setPixmap(QPixmap(global_path+"Digital_Signage_USM/Plantillas/Imagenes/Logos/elo.jpg"));

    ui->Imagen_Centro_Edicion->setPixmap(QPixmap(global_path+"Digital_Signage_USM/Material_Interfaz/layout.png"));

    ui->miniatura_asignar_contenido->setPixmap(QPixmap(global_path+"Digital_Signage_USM/Material_Interfaz/video_player.png"));

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

    if (!fileName.isEmpty()) {
        path_subir_video = fileName;
        QString fileBaseName;
        QStringList tmp_string;

        QFileInfo fileInfo(fileName);
        fileBaseName = fileInfo.fileName(); // Solo el nombre del archivo
        tmp_string = fileBaseName.split('.');
        name_subir_video = fileBaseName;

        QString imagenPath = global_path + "Contenido_ELO308/miniaturas/" + tmp_string[0] + ".jpg";
        QString scriptPath = global_path + "Digital_Signage_USM/miniaturas_gen.sh";

        QStringList arguments;
        arguments << fileName << imagenPath;
        QProcess *process = new QProcess(this);
        // Asignamos el script y los argumentos al proceso
        process->start(scriptPath, arguments);
        process->waitForFinished(); // Espera a que el proceso termine antes de continuar

        // Miniatura plantilla
        ui->label_subir_video->setPixmap(QPixmap(imagenPath));
        qDebug() << scriptPath << arguments;
    } else {
        qDebug() << "No se seleccionó ningún archivo.";
    }


}

void MainWindow::on_Guardar_subir_video_clicked()
{
    ui->lista_asignar_contenido->clear();

    QString scriptPath = global_path+"Digital_Signage_USM/copy_video.sh";
    QString Path_video_destino = global_path+"Contenido_ELO308/Videos/"+name_subir_video;

    // Miniatura plantilla
    QString path_miniaturas = global_path+"Digital_Signage_USM/Material_Interfaz/";
    QString error_imagen = path_miniaturas+"loading.png";
    QString ready_imagen = path_miniaturas+"ready.png";
    ui->label_estado_subir_video->setPixmap(QPixmap(error_imagen));

    // Forzar la actualización de la interfaz de usuario
    QApplication::processEvents();

    QStringList video_name = Path_video_destino.split(".");

    QStringList arguments;
    arguments << path_subir_video << Path_video_destino << video_name[0];
    qDebug() << arguments[0] << arguments[1] << arguments[2] << "\n";

    QProcess *process = new QProcess(this);

    // Conectar señales a las funciones correspondientes
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus){
        if (exitCode == 0 && exitStatus == QProcess::NormalExit){
            ui->label_estado_subir_video->setPixmap(QPixmap(ready_imagen));
        } else {
            ui->label_estado_subir_video->setPixmap(QPixmap(error_imagen));
            qDebug() << "Error al subir el video. Código de salida:" << exitCode;
        }
    });

    connect(process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error){
        ui->label_estado_subir_video->setPixmap(QPixmap(error_imagen));
        qDebug() << "Error en la ejecución del proceso:" << error;
    });

    // Iniciar el proceso de manera asíncrona
    process->start(scriptPath, arguments);
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
    ui->Estado_Disponibilida_Contenido->clear();
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
    ui->Estado_Disponibilidad->clear();
    ui->Estado_Disponibilidad->setPixmap(QPixmap(global_path+"Digital_Signage_USM/Material_Interfaz/loading.png"));

    // Miniatura plantilla
    QString path_miniaturas = global_path+"Digital_Signage_USM/Material_Interfaz/";
    QString loading_imagen = path_miniaturas+"loading.png";
    QPixmap mapeo(loading_imagen);
    ui->Estado_Disponibilidad->setPixmap(mapeo);

    QString error_imagen = path_miniaturas+"error.png";
    QString ready_imagen = path_miniaturas+"ready.png";

    // Forzar la actualización de la interfaz de usuario
    QApplication::processEvents();

    // Parámetros
    QString Dispositivo_seleccionado = ui->home_dispositivo->currentText();

    // Path de scripts
    QString scriptPath = global_path+"Digital_Signage_USM/check_dispo.sh";

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << user_sincro+"@"+ip_sincro;
    qDebug() << arguments << "\n";

    // Consultar
    QProcess *process = new QProcess(this);

    // Conectar señales a las funciones correspondientes
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus){
        if (exitCode == 0 && exitStatus == QProcess::NormalExit){
            ui->Estado_Disponibilidad->setPixmap(QPixmap(ready_imagen));
        } else {
            ui->Estado_Disponibilidad->setPixmap(QPixmap(error_imagen));
        }
    });

    connect(process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error){
        ui->Estado_Disponibilidad->setPixmap(QPixmap(error_imagen));
        qDebug() << "Error en la ejecución del proceso:" << error;
    });

    // Iniciar el proceso de manera asíncrona
    process->start(scriptPath, arguments);
}



void MainWindow::on_lista_sincronizacion_devices_activated(int index)
{
    ui->Estado_Disponibilidad->clear();
    ui->Estado_Disponibilida_Contenido->clear();
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
    ///home/saca/Desktop/Contenido_ELO308/Plantillas/Plantilla_1
    QString filePath = global_path+"Contenido_ELO308/Plantillas/"+name_plantilla+"/temp/"+name_sector_selected+".txt";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo \n";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        ui->Contenido_Asignado_Centro_Edicion->addItem(line);
    }
    file.close();

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
    QString filePath = global_path+"Contenido_ELO308/Plantillas/"+Plantilla+"/temp/"+name_sector_selected+".txt";
    qDebug() << "Path archivo txt lista: " << path_lista_contenido << "\n";


    QFile file(path_lista_contenido);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para escritura";
        return;
    }

    QTextStream out(&file);

    QFile file2(filePath);
    if (!file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para escritura";
        return;
    }

    QTextStream out2(&file2);

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
            out2 <<ui->Contenido_Asignado_Centro_Edicion->item(i)->text()<< "\n";
        }
    }

    else {
        qDebug() << "Es video \n";

        for (int i = 0; i < ui->Contenido_Asignado_Centro_Edicion->count(); ++i) {
            out << "file '"<<global_path<<"Contenido_ELO308/Videos/" << ui->Contenido_Asignado_Centro_Edicion->item(i)->text()<< "'" << "\n";
            out2 <<ui->Contenido_Asignado_Centro_Edicion->item(i)->text()<< "\n";
        }
    }

    file.close();
    file2.close();
    qDebug() << "Elementos de la lista guardados en lista.txt";
    ////////////////////////////////////////////////

}

void MainWindow::on_boton_generar_video_editor_clicked()
{
    QString path_miniaturas = global_path + "Digital_Signage_USM/Material_Interfaz/";
    QString loading_imagen = path_miniaturas + "loading.png";
    QString error_imagen = path_miniaturas + "error.png";
    QString ready_imagen = path_miniaturas + "ready.png";

    // Establecer la imagen de carga
    QPixmap mapeo(loading_imagen);
    ui->label_status_editor->setPixmap(mapeo);

    // Forzar la actualización de la interfaz de usuario
    QApplication::processEvents();

    QString name_video = ui->nombre_video->toPlainText();
    QString plantilla = ui->Lista_plantillas_Centro_Edicion->currentText();

    qDebug() << "Nombre asignado: " << name_video;
    QString scriptPath = global_path + "Contenido_ELO308/Plantillas/" + plantilla + "/video.sh";

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << admin_nombre << name_video;

    QProcess *process = new QProcess(this);

    // Conectar señales a las funciones correspondientes
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus){
        if (exitCode == 0 && exitStatus == QProcess::NormalExit){
            ui->label_status_editor->setPixmap(QPixmap(ready_imagen));
        } else {
            ui->label_status_editor->setPixmap(QPixmap(error_imagen));
            qDebug() << "Error al generar el video. Código de salida:" << exitCode;
        }
    });

    connect(process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error){
        ui->label_status_editor->setPixmap(QPixmap(error_imagen));
        qDebug() << "Error en la ejecución del proceso:" << error;
    });

    // Iniciar el proceso de manera asíncrona
    process->start(scriptPath, arguments);

    qDebug() << scriptPath << arguments;
}



void MainWindow::on_Bucle_clicked()
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
    QString ruta_status = "/home/"+Dispositivo_seleccionado+"/Desktop/"+Dispositivo_seleccionado;
    scriptPath = global_path+"Digital_Signage_USM/play.sh";

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;

    QString name_video= ui->Lista_plantillas->currentText();

    arguments << user_ip_dispositivo << scriptPath_destino << scriptPath_videos << name_video << "9" <<ruta_status;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    int exitCode = process->exitCode();
    if (exitCode == 0){
        //Exito
        ui->Inicio_feedback->setText("Instrucción *"+ui->Bucle->text()+"* enviada satisfactoriamente");
    }
    else {
        ui->Inicio_feedback->setText("No se envió instrucción *"+ui->Bucle->text()+"* solicitada");
    }
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath << arguments;
}


void MainWindow::on_Consultar_clicked()
{
    ui->Inicio_Status_Contenido->clear();
    ui->Inicio_status->setPixmap(QPixmap(global_path + "Digital_Signage_USM/Material_Interfaz/loading.png"));

    // Forzar la actualización de la interfaz de usuario
    QApplication::processEvents();

    // Parámetros
    QString Dispositivo_seleccionado = ui->home_dispositivo->currentText();
    QString scriptPath_destino = "/home/" + Dispositivo_seleccionado + "/Desktop/" + Dispositivo_seleccionado + "/control_device.sh";
    QString scriptPath_videos = "/home/" + Dispositivo_seleccionado + "/Desktop/" + Dispositivo_seleccionado + "/videos";
    QString ruta_status = "/home/" + Dispositivo_seleccionado + "/Desktop/" + Dispositivo_seleccionado;
    QString name_video = ui->Lista_plantillas->currentText();
    QString device = ui->home_dispositivo->currentText();
    QString textfile = global_path + "Contenido_Dispositivos/Estado/" + device + "_status.txt";
    QString textfile_remoto = "/home/" + device + "/Desktop/" + device + "/mplayer_status.txt";

    // Path de scripts
    QString scriptPath = global_path + "Digital_Signage_USM/play.sh";
    QString script_copytext = global_path + "Digital_Signage_USM/copy_text.sh";

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << user_ip_dispositivo << scriptPath_destino << scriptPath_videos << name_video << "10" << ruta_status;
    QStringList arguments2;
    arguments2 << user_ip_dispositivo << textfile_remoto << textfile;

    // Crear archivo
    QFile file(textfile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para escritura";
        return;
    }
    file.close();

    // Consultar de forma asíncrona
    QProcess *process = new QProcess(this);

    // Conectar señales a las funciones correspondientes
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus){
        ui->Inicio_status->clear();
        if (exitCode == 0 && exitStatus == QProcess::NormalExit) {
            qDebug() << script_copytext << arguments2 << "\n";
            QFile file(textfile);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream stream(&file);
                ui->Inicio_Status_Contenido->setPlainText(stream.readAll()); // Cargar el contenido del archivo en el QTextEdit
                file.close();
            }
        } else {
            QFile file(textfile);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                ui->Inicio_Status_Contenido->setPlainText("Comunicación fallida con dispositivo: " + device); // Cargar el contenido del archivo en el QTextEdit
                file.close();
                qDebug() << "Cambios Guardados Satisfactoriamente: " << device << "\n";
            }
        }
    });

    connect(process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error){
        ui->Inicio_status->clear();
        ui->Inicio_Status_Contenido->setPlainText("Error en la ejecución del proceso.");
        qDebug() << "Error en la ejecución del proceso:" << error;
    });

    // Iniciar el proceso de manera asíncrona
    process->start(script_copytext, arguments2);
}



void MainWindow::on_Reanudar_clicked()
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
    QString ruta_status = "/home/"+Dispositivo_seleccionado+"/Desktop/"+Dispositivo_seleccionado;
    scriptPath = global_path+"Digital_Signage_USM/play.sh";

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;

    QString name_video= ui->Lista_plantillas->currentText();

    arguments << user_ip_dispositivo << scriptPath_destino << scriptPath_videos << name_video << "10" <<ruta_status;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    int exitCode = process->exitCode();
    if (exitCode == 0){
        //Exito
        ui->Inicio_feedback->setText("Instrucción *"+ui->Reanudar->text()+"* enviada satisfactoriamente");
    }
    else {
        ui->Inicio_feedback->setText("No se envió instrucción *"+ui->Reanudar->text()+"* solicitada");
    }
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath << arguments;
}

