#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mapwidget.h"

#include <QInputDialog>

#include <vlc-qt/Common.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>

SimMapWindow::SimMapWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimMapWindow),
    _media(0)
{
    ui->setupUi(this);

    _instance = new VlcInstance(VlcCommon::args(), this);
    _player = new VlcMediaPlayer(_instance);
    _player->setVideoWidget(ui->video);

    ui->video->setMediaPlayer(_player);

    connect(ui->setStreamUrl, SIGNAL(clicked()), this, SLOT(openMjpegUrl()));
}

SimMapWindow::~SimMapWindow()
{
    delete _player;
    delete _media;
    delete _instance;
    delete ui;
}

void SimMapWindow::openMjpegUrl()
{
    QString url =
            QInputDialog::getText(this, tr("Set URL"), tr("Enter the stream URL you want to play"), QLineEdit::Normal, tr("http://127.0.0.1:8080"));

    if (url.isEmpty())
        return;

    _media = new VlcMedia(url, _instance);

    _player->open(_media);
}
