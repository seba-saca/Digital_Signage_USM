#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->Logo_USM->setPixmap(QPixmap("/home/seba/Desktop/Contenido/Imagenes/Logos/logo_usm.png"));
    ui->Logo_ELO->setPixmap(QPixmap("/home/seba/Desktop/Contenido/Imagenes/Logos/elo.jpg"));
    ui->preview_plantilla->setPixmap(QPixmap("/home/seba/Desktop/Contenido/Imagenes/Overlays/fondo_usm_1.jpg"));

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

    QString path_lista_lugares = "/home/seba/Desktop/Digital_Signage_USM/Ubicaciones";
    QDir dir4(path_lista_lugares);
    QStringList files_lista_lugares = dir4.entryList(QDir::Files);
    ui->lista_ubicaciones->addItems(files_lista_lugares);
    ui->home_ubicacion->addItems(files_lista_lugares);

    QString filename;
    QString filename2;
    QString filename3;
    QString filename_Dispositivos_Registrados;


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

    filename_Dispositivos_Registrados = "/home/seba/Desktop/Digital_Signage_USM/Dispositivos_Registrados.txt";
    QFile file_Dispositivos_Registrados(filename_Dispositivos_Registrados);
    if (file_Dispositivos_Registrados.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file_Dispositivos_Registrados);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if (!line.isEmpty()){
                ui->Dispositivos_Registrados->addItem(line); // Cargar el contenido del archivo en el QTextEdit
            }
        }
        file_Dispositivos_Registrados.close();
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

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << indice_string << "1";
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    //process->start("bash", QStringList() << scriptPath);
    //process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath;
}

void MainWindow::on_sincronizar_clicked()
{
    int indice_actual_device = ui->home_dispositivo->currentIndex();
    QString scriptPath;
    QString indice_device_string = QString::number(indice_actual_device+1);
    scriptPath = "/home/seba/Desktop/Digital_Signage_USM/transferir.sh";

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string;
    QProcess *process = new QProcess(this);
    // Asignamos el script y los argumentos al proceso
    process->start(scriptPath, arguments);
    //process->start("bash", QStringList() << scriptPath);
    process->waitForFinished(); // Espera a que el proceso termine antes de continuar
    qDebug() << scriptPath;
}

void MainWindow::on_Lista_plantillas_activated(int index)
{
    // Dependiendo del índice seleccionado, cambias la imagen mostrada en el QLabel
    QString indice = QString::number(index+1);
    ui->preview_plantilla->setPixmap(QPixmap("/home/seba/Desktop/Contenido/Imagenes/Overlays/fondo_usm_"+indice+".jpg"));
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

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << indice_string << "2";
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

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << indice_string << "3";
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

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << indice_string << "8";
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

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << indice_string << "5";
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

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << indice_string << "4";
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

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << indice_string << "7";
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

    // Lista de argumentos que deseas pasar al script
    QStringList arguments;
    arguments << indice_device_string << indice_string << "6";
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
    QString filename_ubicaciones = "/home/seba/Desktop/Digital_Signage_USM/Ubicaciones/"+Ubicacion;
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
    QString user_and_ip = ui->Formato_ip_dispositivo->toPlainText();
    QListWidgetItem *newItem = new QListWidgetItem(user_and_ip);
    ui->Dispositivos_Registrados->addItem(newItem);
}


void MainWindow::on_Quitar_lista_dispositivos_clicked()
{
    // Obtener el elemento seleccionado del QListWidget
    QList<QListWidgetItem *> selectedItems = ui->Dispositivos_seleccionados->selectedItems();
    if (!selectedItems.isEmpty()) {
        QListWidgetItem *selectedItem = selectedItems.first();
        // Eliminar el elemento seleccionado del QListWidget
        delete ui->Dispositivos_seleccionados->takeItem(ui->Dispositivos_seleccionados->row(selectedItem));
    }
}


void MainWindow::on_Agregar_lista_dispositivos_clicked()
{
    // Obtener el elemento seleccionado del primer QListWidget
    QList<QListWidgetItem *> selectedItems = ui->Dispositivos_Registrados->selectedItems();
    if (!selectedItems.isEmpty()) {
        QListWidgetItem *selectedItem = selectedItems.first();
        // Clonar el elemento seleccionado y agregarlo al segundo QListWidget
        QListWidgetItem *newItem = new QListWidgetItem(selectedItem->text());
        ui->Dispositivos_seleccionados->addItem(newItem);
    }
}

void MainWindow::on_Quitar_titular_small_2_clicked(){}

void MainWindow::on_Limpiar_lista_dispositivos_clicked()
{
    ui->Dispositivos_seleccionados->clear();
}


void MainWindow::on_Agregar_dispositivo_ubicacion_boton_clicked()
{
    for (int i = 0; i < ui->Dispositivos_seleccionados->count(); ++i) {
        QListWidgetItem *item = ui->Dispositivos_seleccionados->item(i)->clone();
        ui->Dispositivos_Ubicacion->addItem(item);
    }
}


void MainWindow::on_Guardar_cambios_dispositivos_agregados_clicked()
{
    QString textfile;
    QString ubicacion;
    ubicacion =ui->lista_ubicaciones->currentText();
    textfile = "/home/seba/Desktop/Digital_Signage_USM/Ubicaciones/"+ubicacion;

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
    qDebug() << "Elementos de la lista guardados en lista.txt";
}




void MainWindow::on_Quitar_dispositivo_lista_ubicacion_clicked()
{
    // Obtener el elemento seleccionado del QListWidget
    QList<QListWidgetItem *> selectedItems = ui->Dispositivos_Ubicacion->selectedItems();
    if (!selectedItems.isEmpty()) {
        QListWidgetItem *selectedItem = selectedItems.first();
        // Eliminar el elemento seleccionado del QListWidget
        delete ui->Dispositivos_Ubicacion->takeItem(ui->Dispositivos_Ubicacion->row(selectedItem));
    }
}


void MainWindow::on_home_ubicacion_activated(int index)
{
    ui->home_dispositivo->clear();
    QString Ubicacion = ui->home_ubicacion->currentText();
    QString filename_ubicaciones = "/home/seba/Desktop/Digital_Signage_USM/Ubicaciones/"+Ubicacion;
    QFile file(filename_ubicaciones);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            if (!line.isEmpty()){
                ui->home_dispositivo->addItem(line); // Cargar el contenido del archivo en el QTextEdit
            }
        }
        file.close();
    }

}

