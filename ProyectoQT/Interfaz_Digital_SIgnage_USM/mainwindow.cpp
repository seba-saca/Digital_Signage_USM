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

    QString filename_Dispositivos_Registrados;
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
    QString name_video= ui->Lista_plantillas->currentText();
    QChar delimiter = '.';

    // Divide la cadena usando el delimitador
    QStringList tokens = name_video.split(delimiter);

    // Imprimir todas las subcadenas
    qDebug() << "Todas las subcadenas:";
    for (const auto& token : tokens) {
        qDebug() << token;
    }

    // Imprimir una subcadena específica (por ejemplo, la tercera)
    int i = 0; // Índice de la subcadena que queremos imprimir (comienza en 0)
    if (i < tokens.size()) {
        qDebug() << "Subcadena específica (0ndice 0):" << tokens[i];
    } else {
        qDebug() << "Índice fuera de rango.";
    }





    QString path_miniaturas = "/home/seba/Desktop/Contenido_ELO308/miniaturas/"+tokens[i]+".jpg";
    qDebug() << path_miniaturas;
    ui->preview_plantilla->setPixmap(QPixmap(path_miniaturas));
    //ui->video_prev->setPixmap(QPixmap("/home/seba/Desktop/Contenido_Dispositivos/berry/cut_ticket.jpg"));



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
    QString textfile_dispositivos_registrados;
    QString ubicacion;
    ubicacion =ui->lista_ubicaciones->currentText();
    textfile = "/home/seba/Desktop/Digital_Signage_USM/Ubicaciones/"+ubicacion;
    textfile_dispositivos_registrados = "/home/seba/Desktop/Digital_Signage_USM/Dispositivos_Registrados.txt";

    QFile file(textfile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para escritura";
        return;
    }

    QFile file2(textfile_dispositivos_registrados);
    if (!file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para escritura";
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < ui->Dispositivos_Ubicacion->count(); ++i) {
        out << ui->Dispositivos_Ubicacion->item(i)->text()+"\n";
    }

    QTextStream out2(&file2);
    for (int i = 0; i < ui->Dispositivos_Registrados->count(); ++i) {
        out2 << ui->Dispositivos_Registrados->item(i)->text()+"\n";
    }


    file.close();
    file2.close();
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
    //Limpieza
    QString video_simbolo = "/home/seba/Desktop/Digital_Signage_USM/Material_Interfaz/video_simbolo.jpg";
    ui->preview_plantilla->setPixmap(QPixmap(video_simbolo));
    ui->home_dispositivo->clear();
    ui->Lista_plantillas->clear();

    //Leemos texto
    QString Ubicacion = ui->home_ubicacion->currentText()+".txt";
    QString filename_ubicaciones = global_path+"Digital_Signage_USM/Ubicaciones/"+Ubicacion;
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


    ui->Lista_plantillas->clear();
    QString name_device= ui->home_dispositivo->currentText();
    QChar delimiter = '@';

    // Divide la cadena usando el delimitador
    QStringList tokens = name_device.split(delimiter);

    // Imprimir todas las subcadenas
    qDebug() << "Todas las subcadenas:";
    for (const auto& token : tokens) {
        qDebug() << token;
    }

    // Imprimir una subcadena específica (por ejemplo, la tercera)
    int i = 0; // Índice de la subcadena que queremos imprimir (comienza en 0)
    if (i < tokens.size()) {
        qDebug() << "Subcadena específica (0ndice 0):" << tokens[i];
    } else {
        qDebug() << "Índice fuera de rango.";
    }



    QString path_lista_lugares = "/home/seba/Desktop/Contenido_Dispositivos/"+tokens[i]+"/videos";
    QDir dir4(path_lista_lugares);
    QStringList files_lista_lugares = dir4.entryList(QDir::Files);
    ui->Lista_plantillas->addItems(files_lista_lugares);


}


void MainWindow::on_home_dispositivo_activated(int index)
{
    QString video_simbolo = "/home/seba/Desktop/Digital_Signage_USM/Material_Interfaz/video_simbolo.jpg";
    ui->preview_plantilla->setPixmap(QPixmap(video_simbolo));
    ui->Lista_plantillas->clear();
    QString name_device= ui->home_dispositivo->currentText();
    QChar delimiter = '@';

    // Divide la cadena usando el delimitador
    QStringList tokens = name_device.split(delimiter);

    // Imprimir todas las subcadenas
    qDebug() << "Todas las subcadenas:";
    for (const auto& token : tokens) {
        qDebug() << token;
    }

    // Imprimir una subcadena específica (por ejemplo, la tercera)
    int i = 0; // Índice de la subcadena que queremos imprimir (comienza en 0)
    if (i < tokens.size()) {
        qDebug() << "Subcadena específica (0ndice 0):" << tokens[i];
    } else {
        qDebug() << "Índice fuera de rango.";
    }



    QString path_lista_lugares = "/home/seba/Desktop/Contenido_Dispositivos/"+tokens[i]+"/videos";
    QDir dir4(path_lista_lugares);
    QStringList files_lista_lugares = dir4.entryList(QDir::Files);
    ui->Lista_plantillas->addItems(files_lista_lugares);
}


void MainWindow::on_asignacion_dispositivos_activated(int index)
{
    //ui->asignacion_dispositivos->clear();
    QString device = ui->asignacion_dispositivos->currentText();

    QChar delimiter = '@';

    // Divide la cadena usando el delimitador
    QStringList tokens = device.split(delimiter);

    // Imprimir todas las subcadenas
    qDebug() << "Todas las subcadenas:";
    for (const auto& token : tokens) {
        qDebug() << token;
    }

    // Imprimir una subcadena específica (por ejemplo, la tercera)
    int i = 0; // Índice de la subcadena que queremos imprimir (comienza en 0)
    if (i < tokens.size()) {
        qDebug() << "Subcadena específica (0ndice 0):" << tokens[i];
    } else {
        qDebug() << "Índice fuera de rango.";
    }


    ui->Gestion_Contenido_Disponible->clear();
    ui->Gestion_Contenido_Disponible_2->clear();
    QString filename_ubicaciones = "/home/seba/Desktop/Contenido_ELO308/videos";
    QDir dir4(filename_ubicaciones);
    QStringList files_lista_lugares = dir4.entryList(QDir::Files);
    ui->Gestion_Contenido_Disponible->addItems(files_lista_lugares);

    QString filename_ubicaciones2 = "/home/seba/Desktop/Contenido_Dispositivos/"+ tokens[i] +"/videos";
    QDir dir5(filename_ubicaciones2);
    QStringList files_lista_lugares2 = dir5.entryList(QDir::Files);
    ui->Gestion_Contenido_Disponible_2->addItems(files_lista_lugares2);

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
    QString admin = ui->texto_admin->toPlainText();
    global_path = "/home/"+admin+"/Desktop/";

    QString path_lista_lugares = global_path+"Digital_Signage_USM/Ubicaciones";
    QDir dir4(path_lista_lugares);
    QStringList files_lista_lugares = dir4.entryList(QDir::Files);
    // Quitar las extensiones de los archivos usando la función, especificando el carácter de corte
    QStringList modified_files_lista_lugares = removeExtensions(files_lista_lugares, '.');
    ui->lista_ubicaciones->addItems(modified_files_lista_lugares);
    ui->home_ubicacion->addItems(modified_files_lista_lugares);


}

