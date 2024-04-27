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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Start_clicked()
{
    int indice_actual = ui->Lista_plantillas->currentIndex();
    QString scriptPath;

    switch (indice_actual) {
    case 0:
        scriptPath = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/1/video_creacion_1.sh";
        break;
    case 1:
        scriptPath = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/2/video_creacion_2.sh";
        break;
    case 2:
        scriptPath = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/3/video_creacion_3.sh";
        break;
    case 3:
        scriptPath = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/4/video_creacion_4.sh";
        break;
    case 4:
        scriptPath = "/home/seba/Desktop/Digital_Signage_USM/Plantillas/5/video_creacion_5.sh";
        break;
    // Agrega más casos según sea necesario para otros índices
    default:
        break;
    }

    QProcess *process = new QProcess(this);
    process->start("bash", QStringList() << scriptPath);
    process->waitForFinished(); // Espera a que el proceso termine antes de continuar
}


void MainWindow::on_Lista_plantillas_activated(int index)
{
    // Dependiendo del índice seleccionado, cambias la imagen mostrada en el QLabel
    switch (index) {
    case 0:
        ui->preview_plantilla->setPixmap(QPixmap("/home/seba/Desktop/Contenido/Imagenes/Overlays/fondo_usm_1.jpg"));
        break;
    case 1:
        ui->preview_plantilla->setPixmap(QPixmap("/home/seba/Desktop/Contenido/Imagenes/Overlays/fondo_usm_2.jpg"));
        break;
    case 2:
        ui->preview_plantilla->setPixmap(QPixmap("/home/seba/Desktop/Contenido/Imagenes/Overlays/fondo_usm_3.jpg"));
        break;
    case 3:
        ui->preview_plantilla->setPixmap(QPixmap("/home/seba/Desktop/Contenido/Imagenes/Overlays/fondo_usm_4.jpg"));
        break;
    case 4:
        ui->preview_plantilla->setPixmap(QPixmap("/home/seba/Desktop/Contenido/Imagenes/Overlays/fondo_usm_5.jpg"));
        break;
    default:
        break;
    }
}





