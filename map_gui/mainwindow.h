#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class SimMapWindow;
}

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class SimMapWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimMapWindow(QWidget *parent = 0);
    ~SimMapWindow();

private slots:
    void openMjpegUrl();

private:
    Ui::SimMapWindow *ui;

    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;
};

#endif // MAINWINDOW_H
