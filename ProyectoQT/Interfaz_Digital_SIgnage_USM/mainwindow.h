#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QMainWindow>
#include <QDir>
#include <QFileInfo>
#include <QListWidgetItem>
#include <QFile>

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
