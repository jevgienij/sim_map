#include "gps_coord.h"

// constructors
GPS_COORD::GPS_COORD(double lon, double lat) : dLongitude(lon), dLatitude(lat)
{
    if (fabs(dLatitude) > 90.0 || fabs(dLongitude) > 180.0)
    {
        std::cerr << "ERROR! GPS_COORD::GPS_COORD(double lon, double lat) constructor:"         << std::endl;
        std::cerr << "       Constructor out of range! Creating empty instance of GPS_COORD!"   << std::endl;
        dLatitude   = 0.0;
        dLongitude  = 0.0;
    }
}

GPS_COORD::GPS_COORD(Exiv2::RationalValue longitudeRational, std::string longitudeRef, Exiv2::RationalValue latitudeRational, std::string latitudeRef)
{
    SetLongitude(longitudeRational, longitudeRef);
    SetLatitude (latitudeRational,  latitudeRef );

    if (fabs(dLongitude) > 180.0 || fabs(dLatitude) > 90.0)
    {
        std::cerr << "ERROR! GPS_COORD::GPS_COORD(Exiv2::RationalValue longitudeRational, std::string longitudeRef, Exiv2::RationalValue latitudeRational, std::string latitudeRef) constructor out of range! Created empty instance of GPS_COORD!" << std::endl;

        dLongitude = 0.0;
        dLatitude = 0.0;
    }
}

GPS_COORD::GPS_COORD(std::string longitudeString, std::string longitudeRef, std::string latitudeString, std::string latitudeRef)
{
    SetLongitude(longitudeString, longitudeRef);
    SetLatitude(latitudeString, latitudeRef);

    if (fabs(dLongitude) > 180.0 || fabs(dLatitude) > 90.0)
    {
        std::cerr << "ERROR! GPS_COORD::GPS_COORD(Exiv2::RationalValue longitudeRational, std::string longitudeRef, Exiv2::RationalValue latitudeRational, std::string latitudeRef) constructor out of range! Created empty instance of GPS_COORD!" << std::endl;

        dLongitude = 0.0;
        dLatitude = 0.0;
    }
}

// member operators
GPS_COORD& GPS_COORD::operator+= (const GPS_COORD& rhs)
{
    this->dLongitude += rhs.dLongitude;
	this->dLatitude  += rhs.dLatitude;

	return *this;
}

GPS_COORD& GPS_COORD::operator-= (const GPS_COORD& rhs)
{
    this->dLongitude -= rhs.dLongitude;
    this->dLatitude  -= rhs.dLatitude;

    return *this;
}

GPS_COORD& GPS_COORD::operator*= (const GPS_COORD& rhs)
{
    this->dLongitude *= rhs.dLongitude; // rhs is in fact a 2-dimensional scale factor!
    this->dLatitude  *= rhs.dLatitude;  // rhs is in fact a 2-dimensional scale factor!

    return *this;
}

GPS_COORD& GPS_COORD::operator*= (const double& rhs)
{
    this->dLongitude *= rhs;  // rhs is in fact a scale factor keeping aspect ratio!
    this->dLatitude  *= rhs;  // rhs is in fact a scale factor keeping aspect ratio!

    return *this;
}

bool GPS_COORD::operator< (const GPS_COORD& rhs) const
{
    if (this->dLatitude > rhs.dLatitude)
        return true;
    else if (this->dLatitude == rhs.dLatitude)
    {
        if (this->dLongitude < rhs.dLongitude)
            return true;
        else
            return false;
    }
    else
        return false;
}


// methods
std::string GPS_COORD::GetLongitudeString() const
{
    double                  dAbs = fabs(dLongitude);
    double                  dDeg = floor(dAbs);
    Exiv2::DoubleValue      dvDeg(dDeg);
    Exiv2::RationalValue    rvDeg(dvDeg.toRational(0));
    std::string             sDeg(rvDeg.toString(0));

    Exiv2::DoubleValue      dvRemainder((dAbs - dDeg) * 60.0);
    Exiv2::RationalValue    rvMin(dvRemainder.toRational(0));
    std::string             sMin(rvMin.toString(0));

    return sDeg + " " + sMin +" 0/1";
}

std::string GPS_COORD::GetLatitudeString() const
{
    double                  dAbs = fabs(dLatitude);
    double                  dDeg = floor(dAbs);
    Exiv2::DoubleValue      dvDeg(dDeg);
    Exiv2::RationalValue    rvDeg(dvDeg.toRational(0));
    std::string             sDeg(rvDeg.toString(0));

    Exiv2::DoubleValue      dvRemainder((dAbs - dDeg) * 60.0);
    Exiv2::RationalValue    rvMin(dvRemainder.toRational(0));
    std::string             sMin(rvMin.toString(0));

    return sDeg + " " + sMin +" 0/1";

    // OLD METHOD:
    //double  dAbs = fabs(dLatitude);
    //int		iDeg = (int)dAbs;
    //int		iDegDenominator = 1;
    //double	dRemainder = dAbs - iDeg;
    //int		iMinDenominator = 1000000;
    //int		iMin = (int)((dRemainder * 60.0 * iMinDenominator) + 0.5);
    //
    //return to_string(iDeg) + "/" + to_string(iDegDenominator) + " " +
    //    to_string(iMin) + "/" + to_string(iMinDenominator) + " 0/1";
}

Exiv2::RationalValue GPS_COORD::GetLongitudeRational() const
{
    Exiv2::RationalValue rv;

    rv.read(GetLongitudeString());

    return rv;
}

Exiv2::RationalValue GPS_COORD::GetLatitudeRational() const
{
	Exiv2::RationalValue rv;
	
	rv.read(GetLatitudeString());

	return rv;
}

void GPS_COORD::SetLongitude(Exiv2::RationalValue longitudeRational, std::string longitudeRef)
{
    double dDeg = longitudeRational.toFloat(0);
    double dMin = longitudeRational.toFloat(1);
    double dSec = longitudeRational.toFloat(2);

    double dLongitudeAbs = dDeg + dMin / 60.0 + dSec / 3600.0;

    dLongitude = longitudeRef.compare("W") == 0 ? -dLongitudeAbs : dLongitudeAbs;
}

void GPS_COORD::SetLongitude(std::string longitudeString, std::string longitudeRef)
{
    Exiv2::RationalValue rv;
    rv.read(longitudeString);

    SetLongitude(rv, longitudeRef);

    //double dDeg = longitudeRational.toFloat(0);
    //double dMin = longitudeRational.toFloat(1);
    //double dSec = longitudeRational.toFloat(2);

    //double dLongitudeAbs = dDeg + dMin / 60.0 + dSec / 3600.0;

    //dLongitude = longitudeRef.compare("W") ? -dLongitudeAbs : dLongitudeAbs;
}

void GPS_COORD::SetLatitude(Exiv2::RationalValue latitudeRational, std::string latitudeRef)
{
    double dDeg = latitudeRational.toFloat(0);
    double dMin = latitudeRational.toFloat(1);
    double dSec = latitudeRational.toFloat(2);

    double dLatitudeAbs = dDeg + dMin / 60.0 + dSec / 3600.0;

    dLatitude = latitudeRef.compare("S") == 0 ? -dLatitudeAbs : dLatitudeAbs;
}

void GPS_COORD::SetLatitude(std::string latitudeString, std::string latitudeRef)
{
    Exiv2::RationalValue rv;
    rv.read(latitudeString);

    SetLatitude(rv, latitudeRef);

    //double dDeg = latitudeRational.toFloat(0);
    //double dMin = latitudeRational.toFloat(1);
    //double dSec = latitudeRational.toFloat(2);

    //double dLatitudeAbs = dDeg + dMin / 60.0 + dSec / 3600.0;

    //dLatitude = latitudeRef.compare("S") ? -dLatitudeAbs : dLatitudeAbs;
}


// non-member operators
const GPS_COORD operator+(const GPS_COORD& lhs, const GPS_COORD& rhs)
{
	GPS_COORD t(lhs);
	t += rhs;
	return t;
}

const GPS_COORD operator-(const GPS_COORD& lhs, const GPS_COORD& rhs)
{
    GPS_COORD t(lhs);
    t -= rhs;
    return t;
}

const GPS_COORD operator*(const GPS_COORD& lhs, const GPS_COORD& rhs)
{
    GPS_COORD t(lhs);
    t *= rhs;
    return t;
}

const GPS_COORD operator*(const GPS_COORD& lhs, const double& rhs)
{
    GPS_COORD t(lhs);
    t *= rhs;
    return t;
}

const GPS_COORD operator*(const double& lhs, const GPS_COORD& rhs)
{
    GPS_COORD t(rhs);
    t *= lhs;
    return t;
}

std::ostream& operator<< (std::ostream& wyjscie, const GPS_COORD& gps)
{
    return wyjscie << "Longitude: " << gps.GetLongitudeDouble() << "  Latitude: " << gps.GetLatitudeDouble();
}


// snippets
/*

https://github.com/mikerussellnz/geolocate/blob/master/src/exif-gps-exiv2.cpp

#include <exiv2/exiv2.hpp>
#include <iostream>
#include <string>
#include <string.h>
#include <cstdio>
#include <cmath>

#include "exif-gps.h"


void dumpAllExif(std::string &file) {
Exiv2::ExifData exifData;
Exiv2::Image::AutoPtr image;

try {
image = Exiv2::ImageFactory::open(file);
} catch(Exiv2::Error e) {
fprintf(stderr, "Failed to open file %s.\n", file.c_str());
return;
}
image->readMetadata();
if (image.get() == NULL) {
fprintf(stderr, "Failed to read file %s.\n", file.c_str());
return;
}
exifData = image->exifData();

Exiv2::ExifData::const_iterator i;
for (i = exifData.begin(); i != exifData.end(); i++) {
std::cout << i->key() << ": " << i->value() << std::endl;
}
}

void readDateTime(std::string &file) {
//     Exiv2::Exifdatum& tag = exifData["Exif.Photo.DateTimeOriginal"];
//std::string date = tag.toString();
//std::cout << "date is " << date << std::endl;
//
}

void writeGPS(std::string &file, ExifGPS gps)  {
	Exiv2::ExifData exifData;
	Exiv2::Image::AutoPtr image;

	try {
		image = Exiv2::ImageFactory::open(file);
	}
	catch (Exiv2::Error e) {
		fprintf(stderr, "Failed to open file %s.\n", file.c_str());
		//               return 0;
	}
	image->readMetadata();
	if (image.get() == NULL) {
		fprintf(stderr, "Failed to read file %s.\n", file.c_str());
		//              return 0;
	}
	exifData = image->exifData();

	char tmp[50];

	snprintf(tmp, 50, "%d/1", (int)gps.altitude);
	exifData["Exif.GPSInfo.GPSAltitude"] = std::string(tmp);
	exifData["Exif.GPSInfo.GPSAltitudeRef"] = (gps.altitude < 0) ? "1" : "0";

	double absLat = fabs(gps.lat);
	int latDegrees = (int)absLat;
	double latRem = absLat - latDegrees;
	int latMinutes = floor((latRem * 60 * 100) + 0.5);
	snprintf(tmp, 50, "%d/1 %d/100 0/1", latDegrees, latMinutes);
	std::cout << "lat " << tmp << std::endl;
	exifData["Exif.GPSInfo.GPSLatitude"] = std::string(tmp);
	exifData["Exif.GPSInfo.GPSLatitudeRef"] = (gps.lat < 0) ? "S" : "N";

	double absLon = fabs(gps.lon);
	int lonDegrees = (int)absLon;
	double lonRem = absLon - lonDegrees;
	int lonMinutes = floor((lonRem * 60 * 100) + 0.5);
	snprintf(tmp, 50, "%d/1 %d/100 0/1", lonDegrees, lonMinutes);
	std::cout << "lon " << tmp << std::endl;
	exifData["Exif.GPSInfo.GPSLongitude"] = std::string(tmp);
	exifData["Exif.GPSInfo.GPSLongitudeRef"] = (gps.lon < 0) ? "W" : "E";

	image->setExifData(exifData);
	image->writeMetadata();
}

ExifGPS readGPS(std::string &file) {
	Exiv2::ExifData exifData;
	Exiv2::Image::AutoPtr image;

	try {
		image = Exiv2::ImageFactory::open(file);
	}
	catch (Exiv2::Error e) {
		fprintf(stderr, "Failed to open file %s.\n", file.c_str());
		//               return 0;
	}
	image->readMetadata();
	if (image.get() == NULL) {
		fprintf(stderr, "Failed to read file %s.\n", file.c_str());
		//              return 0;
	}
	exifData = image->exifData();

	//      Exiv2::Exifdatum& gpsVer = exifData["Exif.GPSInfo.GPSVersionID"];

	Exiv2::Exifdatum& latValue = exifData["Exif.GPSInfo.GPSLatitude"];
	Exiv2::Exifdatum& lonValue = exifData["Exif.GPSInfo.GPSLongitude"];
	Exiv2::Exifdatum& latRef = exifData["Exif.GPSInfo.GPSLatitudeRef"];
	Exiv2::Exifdatum& lonRef = exifData["Exif.GPSInfo.GPSLongitudeRef"];

	double lat;
	double lon;

	Exiv2::Rational degrees;
	Exiv2::Rational minutes;
	Exiv2::Rational seconds;

	degrees = latValue.toRational(0);
	minutes = latValue.toRational(1);
	seconds = latValue.toRational(2);

	lat = ((double)degrees.first / (double)degrees.second) + ((double)minutes.first / (minutes.second * 60)) + ((double)seconds.first / 3600);

	if (strcmp(latRef.toString().c_str(), "S") == 0) {
		lat = -lat;
	}

	degrees = lonValue.toRational(0);
	minutes = lonValue.toRational(1);
	seconds = lonValue.toRational(2);

	lon = ((double)degrees.first / (double)degrees.second) + ((double)minutes.first / (minutes.second * 60)) + ((double)seconds.first / 3600);

	if (strcmp(lonRef.toString().c_str(), "W") == 0) {
		lon = -lon;
	}

	ExifGPS gps;
	gps.lat = lat;
	gps.lon = lon;
	gps.altitude = 0;

	std::cout << "lat " << latValue.toString() << " ref " << latRef.toString() << " lon " << lonValue.toString() << " ref " << lonRef.toString() << std::endl;

	return gps;
}

*/