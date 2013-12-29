#include "mapwidget.h"
#include <QMessageBox>


MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent)
{
    imageDatabase = generateDB("C:\\Users\\Carlos\\Documents\\mgr\\moja\\sim_map\\resources\\tiles\\");
    // TODO iTileSize checking while creating DB

    QString filename =  QString::fromStdString(imageDatabase[0][0].GetFilepath());
    QImage image(filename);
    if (image.isNull()) {
                 QMessageBox::information(this, tr("DB gen for Sim map"),
                                          tr("Cannot load %1.").arg(filename)); return;}
    iTileSize = image.width();
    //~image(); // OK?
}

void MapWidget::paintEvent(QPaintEvent *)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);

    //a simple line
    painter.drawLine(1,1,this->width()-100,this->height()-100);

    //
    double dUavLongitude = 21.05;   //[deg]
    double dUavLatitude  = 51.95;    //[deg]
    double dUavAltitude = 0.0;    //[m]
    // something else?

	int iUAVcol;
	int iUAVrow;

    // select the tile in which is the UAV as [UAVcol][UAVrow]

	for (int i_row = 0; i_row < imageDatabase.size(); ++i_row)
	{
		for (int i_col = 0; i_col < imageDatabase.at(i_row).size(); ++i_col)
		{
			if(dUavLatitude  <= imageDatabase[i_row][i_col].GetTopLeft().GetLatitudeDouble()  && dUavLatitude  > imageDatabase[i_row][i_col].GetBotRght().GetLatitudeDouble() &&
			   dUavLongitude >= imageDatabase[i_row][i_col].GetTopLeft().GetLongitudeDouble() && dUavLongitude < imageDatabase[i_row][i_col].GetBotRght().GetLongitudeDouble() )
			{
				iUAVcol = i_col;
				iUAVrow = i_row;
			}
		}
	}

    // load the tiles in THE FOLLOWING AREA:
    // top left corner: tile[UAVcol-tileColsInView][UAVrow+tileRowsInView]
    // bot rght corner: tile[UAVcol+tileColsInView][UAVrow-tileRowsInView]

	// setting desired painted size to 1100
    int tileColsInView = 5; //parentWidget()->width() / image.width();
    int tileRowsInView = 5; //parentWidget()->height() / image.height();

     //petle na odwrot? czy to ma znaczenie?
    for (auto it_row=iUAVrow+tileRowsInView;it_row>iUAVrow-tileRowsInView;it_row--)
    {
        for (auto it_col=iUAVcol-tileColsInView;it_col<iUAVcol+tileColsInView;it_col++)
        {

            QString filename =  QString::fromStdString(imageDatabase[it_col][it_row].GetFilepath()); //[it_col][it_row] dobrze czy na odwrot?????
            QImage image(filename);
            //if (image.isNull()) //zaladuj resource mapa niedostepna! NIEEEE! Tlo zawsze jako "Mapa niedostepna", painter najwyzej przykryje!

            painter.drawImage( (it_col-(iUAVcol-tileColsInView))*iTileSize, ((iUAVrow+tileRowsInView)-it_row)*iTileSize, image);

			//rysuje w odwrotnej kolejnosci!
        }
    }

    // narysuj UAV
    // narysuj Etykiete z info



    //painter.drawImage(300, 600, image);
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
