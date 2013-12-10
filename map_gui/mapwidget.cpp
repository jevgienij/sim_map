#include "mapwidget.h"
 #include <QMessageBox>
MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent)
{

}

void MapWidget::paintEvent(QPaintEvent *event)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);

    //a simple line
    painter.drawLine(1,1,this->width()-100,this->height()-100);
QString filename("C:\\Users\\Carlos\\Documents\\mgr\\moja\\sim_map\\resources\\tiles\\0_0.jpg");
QImage image(filename);
if (image.isNull()) {
             QMessageBox::information(this, tr("Image Viewer"),
                                      tr("Cannot load %1.").arg(filename)); return;}

    painter.drawImage(300, 600, image);
    //create a black pen that has solid line
    //and the width is 2.
    QPen myPen(Qt::black, 2, Qt::SolidLine);
    painter.setPen(myPen);
    painter.drawLine(100,100,100,1);

    //draw a point
    myPen.setColor(Qt::red);
    painter.drawPoint(110,110);

    //draw a polygon
    QPolygon polygon;
    polygon << QPoint(130, 140) << QPoint(180, 170)
             << QPoint(180, 140) << QPoint(220, 110)
             << QPoint(140, 100);
     painter.drawPolygon(polygon);

     //draw an ellipse
     //The setRenderHint() call enables antialiasing, telling QPainter to use different
     //color intensities on the edges to reduce the visual distortion that normally
     //occurs when the edges of a shape are converted into pixels
//     painter.setRenderHint(QPainter::Antialiasing, true);
//     painter.setPen(QPen(Qt::black, 3, Qt::DashDotLine, Qt::RoundCap));
//     painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
//     painter.drawEllipse(200, 80, 400, 240);

}
