#include "gps_coord.h"

// constructors
GPS_COORD::GPS_COORD(double lon, double lat) : dLongitude(lon), dLatitude(lat)
{
	// for the sake of more universal usage of scaling operators the if in constructor is commented out

    //if (fabs(dLatitude) > 90.0 || fabs(dLongitude) > 180.0)
    //{
    //    std::cerr << "ERROR! GPS_COORD::GPS_COORD(double lon, double lat) constructor:"         << std::endl;
    //    std::cerr << "       Constructor out of range! Creating empty instance of GPS_COORD!"   << std::endl;
    //    dLatitude   = 0.0;
    //    dLongitude  = 0.0;
    //}
}

GPS_COORD::GPS_COORD(Exiv2::RationalValue longitudeRational, std::string longitudeRef, Exiv2::RationalValue latitudeRational, std::string latitudeRef)
{
    SetLongitude(longitudeRational, longitudeRef);
    SetLatitude (latitudeRational,  latitudeRef );

    //if (fabs(dLongitude) > 180.0 || fabs(dLatitude) > 90.0)
    //{
    //    std::cerr << "ERROR! GPS_COORD::GPS_COORD(Exiv2::RationalValue longitudeRational, std::string longitudeRef, Exiv2::RationalValue latitudeRational, std::string latitudeRef) constructor out of range! Created empty instance of GPS_COORD!" << std::endl;

    //    dLongitude = 0.0;
    //    dLatitude = 0.0;
    //}
}

GPS_COORD::GPS_COORD(std::string longitudeString, std::string longitudeRef, std::string latitudeString, std::string latitudeRef)
{
    SetLongitude(longitudeString, longitudeRef);
    SetLatitude(latitudeString, latitudeRef);

    //if (fabs(dLongitude) > 180.0 || fabs(dLatitude) > 90.0)
    //{
    //    std::cerr << "ERROR! GPS_COORD::GPS_COORD(Exiv2::RationalValue longitudeRational, std::string longitudeRef, Exiv2::RationalValue latitudeRational, std::string latitudeRef) constructor out of range! Created empty instance of GPS_COORD!" << std::endl;

    //    dLongitude = 0.0;
    //    dLatitude = 0.0;
    //}
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

GPS_COORD& GPS_COORD::operator/= (const GPS_COORD& rhs)
{
    this->dLongitude /= rhs.dLongitude; // rhs is in fact a 2-dimensional scale factor!
    this->dLatitude  /= rhs.dLatitude;  // rhs is in fact a 2-dimensional scale factor!

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

const GPS_COORD operator/(const GPS_COORD& lhs, const GPS_COORD& rhs)
{
    GPS_COORD t(lhs);
    t /= rhs;
    return t;
}

std::ostream& operator<< (std::ostream& wyjscie, const GPS_COORD& gps)
{
    return wyjscie << "Longitude: " << gps.GetLongitudeDouble() << "  Latitude: " << gps.GetLatitudeDouble();
}

