#include <mapwidget.h>


MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent)
{	
	QString temp = QCoreApplication::applicationDirPath() + "/" + ".." + "/" + ".." + "/" + "resources" + "/" + "tiles" + "/";
	
	// debug log
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
    

	// create and start timer used in UAV path drawing	
	timer.start();
}


void MapWidget::paintEvent(QPaintEvent *)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);
	
	GPS_COORD uavPosition(dUavLongitude, dUavLatitude);

	int iUAVcol = -1;
	int iUAVrow = -1;

    // select the tile in which is the UAV as [UAVcol][UAVrow]
	for (int i_row = 0; i_row < imageDatabase.size(); ++i_row)
	{
		if(dUavLatitude  <= imageDatabase[i_row][0].GetTopLeft().GetLatitudeDouble() && 
		   dUavLatitude  >  imageDatabase[i_row][0].GetBotRght().GetLatitudeDouble() )
		{
			for (int i_col = 0; i_col < imageDatabase.at(i_row).size(); ++i_col)
			{
				if(dUavLatitude  <= imageDatabase[i_row][i_col].GetTopLeft().GetLatitudeDouble()  &&
				   dUavLatitude  >  imageDatabase[i_row][i_col].GetBotRght().GetLatitudeDouble()  &&
				   dUavLongitude >= imageDatabase[i_row][i_col].GetTopLeft().GetLongitudeDouble() && 
				   dUavLongitude <  imageDatabase[i_row][i_col].GetBotRght().GetLongitudeDouble() )
				{
					iUAVcol = i_col;
					iUAVrow = i_row;
					break;
				}
			}
			break;
		}
	}


	bPrevPaintWasError = bCurrPaintIsError;

	if (iUAVcol < 0 || iUAVrow < 0)
	{
		bCurrPaintIsError = true;

		if (!bPrevPaintWasError)
		{
			QMessageBox::critical(this, tr("Error!"), tr("UAV position out of map boundaries")); 
			return;
		}
	}
	else
		bCurrPaintIsError = false;


    // load and draw tiles in THE FOLLOWING AREA:
    // top left corner: tile[UAVcol-tileColsInView][UAVrow+tileRowsInView]
    // bot rght corner: tile[UAVcol+tileColsInView][UAVrow-tileRowsInView]

	// load and prepare "No data available" tile	
	QString filename = QCoreApplication::applicationDirPath() + "/" + ".." + "/" + ".." + + "/" + "resources" + "/" + "input" + "/" + "no_data.png";
	QImage noDataTile(filename);
	if (noDataTile.width() != iTileSize)
		noDataTile = noDataTile.scaled(iTileSize, iTileSize);


	//std::ofstream Cfg("./log.txt");
	//Cfg << "# BEGIN #" << std::endl;	
	
	//int tileColsInView = (this->parentWidget()->width()/iTileSize  + 1) / 2; //parentWidget()->width() / image.width();
    //int tileRowsInView = (this->parentWidget()->height()/iTileSize + 1) / 2; //parentWidget()->height() / image.height();
	
	this->resize((this->parentWidget()->width() / iTileSize + 1)*(iTileSize), (this->parentWidget()->height() / iTileSize + 1)*(iTileSize));
	//this->resize(tileColsInView * iTileSize, tileRowsInView * iTileSize);
	
	int tileColsInView = (this->width()/iTileSize  + 1) / 2; //parentWidget()->width() / image.width();
    int tileRowsInView = (this->height()/iTileSize + 1) / 2; //parentWidget()->height() / image.height();

	int iUAVcolInVisibleMap;
	int iUAVrowInVisibleMap;

	
	if (bCurrPaintIsError)
		painter.drawImage( 0, 0, noDataTile);
	else
	{
		for (int it_row=iUAVrow+tileRowsInView-1;it_row>=iUAVrow-tileRowsInView;it_row--)
		{

			for (int it_col=iUAVcol-tileColsInView;it_col<iUAVcol+tileColsInView;it_col++)
			{
				if (it_row < 0 || it_row >= imageDatabase.size())
				{
					painter.drawImage( (it_col-(iUAVcol-tileColsInView))*iTileSize, (it_row-(iUAVrow-tileRowsInView))*iTileSize, noDataTile);
				}
				else if (it_col < 0 || it_col >= imageDatabase.at(it_row).size()) 
					// nie mozna wrzucic razem do poprzedniego ifa, gdyz jest ryzyko przekroczenia zakresu wolajac imageDatabase.at(it_row).size()
					// sprawdzenie zakresu it_row w poprzednim ifie powoduje, ze ta galaz warunku zostanie wywolana tylko dla prawidlowych wartosci it_row
				{
					painter.drawImage( (it_col-(iUAVcol-tileColsInView))*iTileSize, (it_row-(iUAVrow-tileRowsInView))*iTileSize, noDataTile);
				}
				else
				{
					QString filename =  QString::fromStdString(imageDatabase[it_col][it_row].GetFilepath()); //[it_col][it_row] dobrze czy na odwrot?????
					QImage image(filename);
				
					painter.drawImage( (it_col-(iUAVcol-tileColsInView))*iTileSize, (it_row-(iUAVrow-tileRowsInView))*iTileSize, image);
				}			
				//Cfg << "it_col "<<it_col <<std::endl;	
				//Cfg << "it_row "<<it_row <<std::endl;	
				//Cfg << "tileColsInView "<<tileColsInView <<std::endl;	
				//Cfg << "tileRowsInView "<<tileRowsInView <<std::endl;	
				//Cfg << "iUAVcol "<<iUAVcol <<std::endl;	
				//Cfg << "iUAVrow "<<iUAVrow <<std::endl;	
				//Cfg << "iTileSize "<<iTileSize <<std::endl;	
				//Cfg << (it_col-(iUAVcol-tileColsInView))*iTileSize << " " <<(it_row-(iUAVrow-tileRowsInView))*iTileSize << std::endl;
				//Cfg << std::endl;
			}
		}
	

		// better solution?
		iUAVcolInVisibleMap = tileColsInView;
		iUAVrowInVisibleMap = tileRowsInView;
	
		//Cfg << "# END #"    << std::endl;
		//Cfg.close();
	
		// draw UAV icon
		filename = QCoreApplication::applicationDirPath() + "/" + ".." + "/" + ".." + + "/" + "resources" + "/" + "input" + "/" + "uav32.png";
		//QString filename =  QString::fromStdString("C:\\Users\\Carlos\\Documents\\mgr\\moja\\sim_map\\resources\\input\\uav32.png");
		QImage uavIcon(filename);

		GPS_COORD topLeftPixel(iUAVcolInVisibleMap*iTileSize, iUAVrowInVisibleMap*iTileSize);
		GPS_COORD botRghtPixel((iUAVcolInVisibleMap+1)*iTileSize, (iUAVrowInVisibleMap+1)*iTileSize);

		// calculate UAV position in pixel coordinates (using template function normalize() and overloaded operator*)
		auto uavPixelPosition = normalize(uavPosition, imageDatabase[iUAVrow][iUAVcol].GetTopLeft(), imageDatabase[iUAVrow][iUAVcol].GetBotRght(), botRghtPixel, topLeftPixel);

		int iUavPxPosition = (int)(uavPixelPosition.GetLongitudeDouble()); 
		int iUavPyPosition = (int)(uavPixelPosition.GetLatitudeDouble());

		painter.drawImage(iUavPxPosition - uavIcon.width() / 2, iUavPyPosition - uavIcon.height() / 2, uavIcon); 
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

	}

	// draw path TODO
	//if (timer.elapsed() > 500)	//add current UAV position to vector every 0.5s
	//{
	//	timer.start();   		// restart the timer 
	// UavPath.push_back(uavPosition);
	//}
}

void MapWidget::receiveData(XXX_MAPGUI_STRUCT receivedData) 
{	
	// debug log
	//std::ofstream Cfg("./log_map_widget_new_data.txt");
	//Cfg << "# BEGIN #" << std::endl;	

	// copy received data to private members of this widget class
	dUavLatitude  = receivedData.latitude;
	dUavLongitude = receivedData.longitude;
	dUavAltitude  = receivedData.altitude;
	
	//Cfg << "dUavLatitude "<<dUavLatitude <<std::endl;	
	//Cfg << "dUavLongitude "<<dUavLongitude <<std::endl;	
	//Cfg << "dUavAltitude "<<dUavAltitude <<std::endl;	
	//Cfg << std::endl;
	//Cfg << "# END #"    << std::endl;
	//Cfg.close();

	// force widget to be repainted
    repaint();
}