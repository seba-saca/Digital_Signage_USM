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

    void on_sincronizar_clicked();

    void on_Pause_clicked();

    void on_Detener_clicked();

    void on_Mute_video_clicked();

    void on_Retroceder_clicked();

    void on_Adelantar_clicked();

    void on_Bajar_Volumen_clicked();

    void on_Subir_Volumen_clicked();

    void on_lista_ubicaciones_activated(int index);

    void on_Agregar_dispositivo_boton_clicked();

    void on_Guardar_cambios_dispositivos_agregados_clicked();

    void on_Quitar_dispositivo_lista_ubicacion_clicked();

    void on_home_ubicacion_activated(int index);

    void on_home_dispositivo_activated(int index);

    void on_asignacion_dispositivos_activated(int index);

    QStringList removeExtensions(const QStringList &fileList, const QChar &cutChar);

    void on_boton_admin_clicked();

    void on_Actualizar_Lista_Dispositivos_clicked();

    void on_lista_asignar_contenido_activated(int index);

    void on_boton_asignar_contenido_clicked();

    void on_Quitar_contenido_asignado_clicked();

    void on_Guardar_cambios_contenido_asignado_clicked();

    void on_Subir_video_clicked();

    void on_Guardar_subir_video_clicked();

    void on_actualizar_sincronizacion_lista_devices_clicked();

    void on_lista_sincronizacion_ubicacion_activated(int index);

    void on_sincronizar_check_dispo_clicked();

    void on_lista_sincronizacion_devices_activated(int index);

    void on_Lista_plantillas_Centro_Edicion_activated(int index);

    void on_Lista_plantillas_Centro_Edicion_Sector_activated(int index);

    void on_Lista_Asignar_Contenido_Centro_Edicion_activated(int index);

    void on_Boton_asignar_contenido_Centro_Edicion_clicked();

    void on_Quitar_Contenido_Asignado_Centro_Edicion_clicked();

    void on_Guardar_contenido_Centro_EDICION_clicked();

    void on_boton_generar_video_editor_clicked();

    void on_Bucle_clicked();

    void on_Consultar_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
