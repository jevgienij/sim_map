#include <mapwidget.h>


MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent)
{
	QString temp = QCoreApplication::applicationDirPath() + "/" + ".." + "/" + ".." + + "/" + "resources" + "/" + "tiles" + "/";
	
	//std::ofstream Cfg("./log1.txt");
	//Cfg << "# BEGIN #" << std::endl;
	//Cfg << QCoreApplication::applicationDirPath().toStdString() << std::endl;
	//Cfg << temp.toStdString() << std::endl;
	//Cfg << "# END #" << std::endl;
	
	imageDatabase = generateDB(temp.toStdString());
    //imageDatabase = generateDB("C:\\Users\\Carlos\\Documents\\mgr\\moja\\sim_map\\resources\\tiles\\");
    // TODO iTileSize checking while creating DB

    QString filename =  QString::fromStdString(imageDatabase[0][0].GetFilepath());
    QImage image(filename);
    if (image.isNull()) 
	{
		QMessageBox::critical(this, tr("DB gen for Sim map"), tr("Cannot load %1.").arg(filename)); 
		return;
	}
    iTileSize = image.width();
    //~image(); // OK?

	// create and start timer used in UAV path drawing	
	timer.start();
}


void MapWidget::paintEvent(QPaintEvent *)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);

    //a simple line
    painter.drawLine(1,1,this->width()+500,this->height()+500);

    //
    double dUavLongitude = 21.05;   //[deg]
    double dUavLatitude  = 51.95;    //[deg]
    double dUavAltitude = 0.0;    //[m]
	GPS_COORD uavPosition(dUavLongitude, dUavLatitude);
    // something else?

	int iUAVcol;
	int iUAVrow;

    // select the tile in which is the UAV as [UAVcol][UAVrow]

	for (int i_row = 0; i_row < imageDatabase.size(); ++i_row)
	{
		for (int i_col = 0; i_col < imageDatabase.at(i_row).size(); ++i_col)
		{
			// TODO zmien na porownanie GPS_COORD!!!
			if(dUavLatitude  <= imageDatabase[i_row][i_col].GetTopLeft().GetLatitudeDouble()  && dUavLatitude  > imageDatabase[i_row][i_col].GetBotRght().GetLatitudeDouble() &&
			   dUavLongitude >= imageDatabase[i_row][i_col].GetTopLeft().GetLongitudeDouble() && dUavLongitude < imageDatabase[i_row][i_col].GetBotRght().GetLongitudeDouble() )
			{
				iUAVcol = i_col;
				iUAVrow = i_row;
			}
		}
	}

    // load and draw tiles in THE FOLLOWING AREA:
    // top left corner: tile[UAVcol-tileColsInView][UAVrow+tileRowsInView]
    // bot rght corner: tile[UAVcol+tileColsInView][UAVrow-tileRowsInView]


	// setting desired painted size to 900
	
	int tileColsInView = (this->width()/iTileSize  + 1) / 2; //parentWidget()->width() / image.width();
    int tileRowsInView = (this->height()/iTileSize + 1) / 2; //parentWidget()->height() / image.height();

	//std::ofstream Cfg("./log.txt");
	//Cfg << "# BEGIN #" << std::endl;

	int iUAVcolInVisibleMap;
	int iUAVrowInVisibleMap;

    for (int it_row=iUAVrow+tileRowsInView-1;it_row>=iUAVrow-tileRowsInView;it_row--)
    {
        for (int it_col=iUAVcol-tileColsInView;it_col<iUAVcol+tileColsInView;it_col++)
        {

            QString filename =  QString::fromStdString(imageDatabase[it_col][it_row].GetFilepath()); //[it_col][it_row] dobrze czy na odwrot?????
            QImage image(filename);

            painter.drawImage( (it_col-(iUAVcol-tileColsInView))*iTileSize, (it_row-(iUAVrow-tileRowsInView))*iTileSize, image);

			//Cfg << "it_col "<<it_col <<std::endl;	
			//Cfg << "it_row "<<it_row <<std::endl;	
			//Cfg << "tileColsInView "<<tileColsInView <<std::endl;	
			//Cfg << "tileRowsInView "<<tileRowsInView <<std::endl;	
			//Cfg << "iUAVcol "<<iUAVcol <<std::endl;	
			//Cfg << "iUAVrow "<<iUAVrow <<std::endl;	
			//Cfg << "iTileSize "<<iTileSize <<std::endl;	
		    //Cfg << (it_col-(iUAVcol-tileColsInView))*iTileSize << " " <<(it_row-(iUAVrow-tileRowsInView))*iTileSize << std::endl;
			//Cfg << std::endl;

			// better solution?
			iUAVcolInVisibleMap = tileColsInView;
			iUAVrowInVisibleMap = tileRowsInView;
        }
    }
	
	//Cfg << "# END #"    << std::endl;
    //Cfg.close();
	
	// draw UAV icon
	QString filename = QCoreApplication::applicationDirPath() + "/" + ".." + "/" + ".." + + "/" + "resources" + "/" + "input" + "/" + "uav32.png";
    //QString filename =  QString::fromStdString("C:\\Users\\Carlos\\Documents\\mgr\\moja\\sim_map\\resources\\input\\uav32.png");
	QImage uavIcon(filename);

	GPS_COORD topLeftPixel(iUAVcolInVisibleMap*iTileSize, iUAVrowInVisibleMap*iTileSize);
	GPS_COORD botRghtPixel((iUAVcolInVisibleMap+1)*iTileSize, (iUAVrowInVisibleMap+1)*iTileSize);

	// calculate UAV position in pixel coordinates (using template function normalize() and overloaded operator*)
	auto uavPixelPosition = normalize(uavPosition, imageDatabase[iUAVrow][iUAVcol].GetTopLeft(), imageDatabase[iUAVrow][iUAVcol].GetBotRght(), botRghtPixel, topLeftPixel);

	int iUavPxPosition = (int)(uavPixelPosition.GetLongitudeDouble()); 
    int iUavPyPosition = (int)(uavPixelPosition.GetLatitudeDouble());

	painter.drawImage(iUavPxPosition, iUavPyPosition, uavIcon); 
	if (uavIcon.isNull())
	{
		QMessageBox::warning(this, tr("Map widget error"), tr("Cannot load UAV icon from %1.").arg(filename)); 
		return;
	}
    
	// draw UAV info label
	painter.setBackgroundMode(Qt::OpaqueMode);
	painter.setBackground(QBrush(QColor(255, 255, 225), Qt::SolidPattern));
	
	int iUavLabelSpacing = 25;
    QString sUavLabel =  QString::fromStdString(" Latitude: " + std::to_string(dUavLatitude) + " Longitude: " + std::to_string(dUavLongitude) + " Altitude: " + std::to_string(dUavAltitude) + " ");
	painter.drawText(iUavPxPosition + iUavLabelSpacing, iUavPyPosition, sUavLabel);

	// draw path TODO move to receiveData() method?
	if (timer.elapsed() > 500)	//add current UAV position to vector every 0.5s
	{
		timer.start();   		// restart the timer 
		UavPath.push_back(uavPosition);
	}

	// draw line!!
	//QPen myPen(Qt::black, 2, Qt::SolidLine);
	//painter.setPen(myPen);
	//painter.drawPolyline ( const QPolygon & points )

	// need a method to recalc vector<UAV_COORD> to vector<pxPoint>
	// create QPolygon from pxPoints 
    //QPolygon polygon;
    //polygon << QPoint(130, 140) << QPoint(180, 170)
    //        << QPoint(180, 140) << QPoint(220, 110)
    //        << QPoint(140, 100);


    ////painter.drawImage(300, 600, image);
    ////create a black pen that has solid line
    ////and the width is 2.
    //QPen myPen(Qt::black, 2, Qt::SolidLine);
    //painter.setPen(myPen);
    //painter.drawLine(1,1,this->width()+500,this->height()+500);

    ////draw a point
    //myPen.setColor(Qt::red);
    //painter.drawPoint(110,110);

    ////draw a polygon
    //QPolygon polygon;
    //polygon << QPoint(130, 140) << QPoint(180, 170)
    //         << QPoint(180, 140) << QPoint(220, 110)
    //         << QPoint(140, 100);
    // painter.drawPolygon(polygon);

     //draw an ellipse
     //The setRenderHint() call enables antialiasing, telling QPainter to use different
     //color intensities on the edges to reduce the visual distortion that normally
     //occurs when the edges of a shape are converted into pixels
//     painter.setRenderHint(QPainter::Antialiasing, true);
//     painter.setPen(QPen(Qt::black, 3, Qt::DashDotLine, Qt::RoundCap));
//     painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
//     painter.drawEllipse(200, 80, 400, 240);

}

void MapWidget::receiveData(int x, int y) 
{
        /*QPainter painter(p);
        painter.setBrush(Qt::black);
        QPoint point(x, y);
        painter.drawPoint(point);
        data.append(point);*/
        repaint();
}