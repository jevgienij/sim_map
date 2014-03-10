#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QtGui>
#include <QWidget>
#include <QMessageBox>
#include <QElapsedTimer>

#include <fstream>
#include <vector>

#include <db_gen.h>
#include <gps_coord.h>
#include <listener.h>

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *); // override Qt default painter method 
    IMAGE_DB imageDatabase;			// image database vector of vectors, see full definition in db_gen.h
    int iTileSize;					// map's tile width/height in pixels
    QElapsedTimer timer;			// timer used in UAV path drawing	
	std::vector<GPS_COORD> UavPath;	// vector of UAV position points for UAV path drawing

	double dUavLatitude;
	double dUavLongitude;
	double dUavAltitude; // data received from simulator
	
	bool bPrevPaintWasError; 
	bool bCurrPaintIsError;  // painting error flags


signals:

public slots:
	void receiveData(XXX_MAPGUI_STRUCT receivedData);
};

template <typename Source, typename Target>
Target normalize(Source s, Source max, Source min, Target floor, Target ceiling) {
	return ((ceiling - floor) * (s - min) / (max - min) + floor);}

#endif // MAPWIDGET_H
