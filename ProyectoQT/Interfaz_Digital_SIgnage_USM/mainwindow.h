#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QMainWindow>
#include <QDir>
#include <QFileInfo>
#include <QListWidgetItem>
#include <QFile>
#include <QListWidget>
#include <QFile>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Start_clicked();

    void on_Lista_plantillas_activated(int index);


    void on_Lista_plantillas_2_activated(int index);

    void on_Agregar_video_clicked();

    void on_Borrar_video_clicked();



    void on_Generar_lista_clicked();

    void on_Limpiar_lista_clicked();

    void on_Titulares_Lista_itemClicked(QListWidgetItem *item);

    void on_Titulares_Large_Lista_itemClicked(QListWidgetItem *item);

    void on_Agregar_titular_small_clicked();

    void on_Quitar_titular_small_clicked();

    void on_Limpiar_lista_titulares_small_clicked();

    void on_Generar_lista_titulares_small_clicked();

    void on_Agregar_titular_large_clicked();

    void on_Borrar_titular_large_clicked();

    void on_Limpiar_lista_titulares_large_clicked();

    void on_Generar_lista_titulares_large_clicked();

    void on_sincronizar_clicked();

    void on_Generar_Video_clicked();

    void on_Pause_clicked();

    void on_Detener_clicked();

    void on_Mute_video_clicked();

    void on_Retroceder_clicked();

    void on_Adelantar_clicked();

    void on_Bajar_Volumen_clicked();

    void on_Subir_Volumen_clicked();

    void on_lista_ubicaciones_activated(int index);

    void on_Agregar_dispositivo_boton_clicked();

    void on_Agregar_lista_dispositivos_clicked();

    void on_Quitar_titular_small_2_clicked();

    void on_Quitar_lista_dispositivos_clicked();

    void on_Limpiar_lista_dispositivos_clicked();

    void on_Agregar_dispositivo_ubicacion_boton_clicked();

    void on_Guardar_cambios_dispositivos_agregados_clicked();


    void on_Quitar_dispositivo_lista_ubicacion_clicked();

    void on_home_ubicacion_activated(int index);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
