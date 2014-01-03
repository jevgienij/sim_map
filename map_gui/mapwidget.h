#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QtGui>
#include <QWidget>
#include <QMessageBox>

#include <fstream>

#include <db_gen.h>

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);
    IMAGE_DB imageDatabase;
    int iTileSize;

signals:

public slots:

};

#endif // MAPWIDGET_H
