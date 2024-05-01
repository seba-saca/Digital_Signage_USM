#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Logo_USM->setPixmap(QPixmap("/home/seba/Desktop/Contenido/Imagenes/Logos/logo_usm.png"));
    ui->Logo_ELO->setPixmap(QPixmap("/home/seba/Desktop/Contenido/Imagenes/Logos/elo.jpg"));
    ui->preview_plantilla->setPixmap(QPixmap("/home/seba/Desktop/Contenido/Imagenes/Overlays/fondo_usm_1.jpg"));

    QString path = "/home/seba/Desktop/Contenido/Videos/Material_Prueba";
    QDir dir(path);
    QStringList files = dir.entryList(QDir::Files);
    ui->Video_lista->addItems(files);

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Start_clicked()
{
    int indice_actual = ui->Lista_plantillas->currentIndex();
    QString scriptPath;
    QString indice = QString::number(indice_actual+1);
    scriptPath = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/"+indice+"/video_creacion_"+indice+".sh";
    QProcess *process = new QProcess(this);
    process->start("bash", QStringList() << scriptPath);
    process->waitForFinished(); // Espera a que el proceso termine antes de continuar
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

