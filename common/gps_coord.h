#ifndef _GPS_COORD_CLASS_H_
#define _GPS_COORD_CLASS_H_

#include <exiv2/exiv2.hpp>
#include <string>
#include <iostream>

class GPS_COORD
{
public:
    // constructors
    GPS_COORD(double lon = 0.0, double lat = 0.0);
    GPS_COORD(Exiv2::RationalValue longitudeRational, std::string longitudeRef, Exiv2::RationalValue latitudeRational, std::string latitudeRef);
    GPS_COORD(std::string longitudeString, std::string longitudeRef, std::string latitudeString, std::string latitudeRef);
    
    // destructor not needed

    // member operators
    GPS_COORD& operator+=(const GPS_COORD& rhs);
    GPS_COORD& operator-=(const GPS_COORD& rhs);
    GPS_COORD& operator*=(const GPS_COORD& rhs);    // SCALING OPERATOR: rhs is in fact a 2-dimensional scale factor!
    GPS_COORD& operator*=(const double& rhs);       // SCALING OPERATOR: rhs is in fact a scale factor keeping aspect ratio!
    GPS_COORD& operator/=(const GPS_COORD& rhs);    // SCALING OPERATOR: rhs is in fact a 2-dimensional scale factor!
    bool       operator< (const  GPS_COORD& rhs) const;

    // methods
    double					GetLongitudeDouble()   const { return dLongitude; };
    double					GetLatitudeDouble()    const { return dLatitude; };
    std::string				GetLongitudeString()   const;
    std::string				GetLatitudeString()    const;
    Exiv2::RationalValue	GetLongitudeRational() const;
    Exiv2::RationalValue	GetLatitudeRational()  const;
    std::string             GetLongitudeRef()      const { return (dLongitude < 0) ? "W" : "E"; }
	std::string             GetLatitudeRef()	   const { return (dLatitude  < 0) ? "S" : "N"; }

    void                    SetLongitude(double	                longitudeDouble) { dLongitude = longitudeDouble; }
    void                    SetLongitude(Exiv2::RationalValue   longitudeRational, std::string longitudeRef);
    void                    SetLongitude(std::string            longitudeString,   std::string longitudeRef);
	void                    SetLatitude (double                 latitudeDouble)  { dLatitude  = latitudeDouble;  }
    void                    SetLatitude (Exiv2::RationalValue   latitudeRational,  std::string latitudeRef);
    void                    SetLatitude (std::string            latitudeString,    std::string latitudeRef);

private:
    double					dLongitude;		// [-180, 180]; >0 = E, east; <0 = W, west
	double					dLatitude;		// [-90, 90]; >0 = N, north; <0 = S, south
};

// non-member operators
const GPS_COORD operator+(const GPS_COORD& lhs, const GPS_COORD& rhs);
const GPS_COORD operator-(const GPS_COORD& lhs, const GPS_COORD& rhs);
const GPS_COORD operator*(const GPS_COORD& lhs, const GPS_COORD& rhs);
const GPS_COORD operator*(const GPS_COORD& lhs, const double& rhs);
const GPS_COORD operator*(const double& lhs, const GPS_COORD& rhs);
const GPS_COORD operator/(const GPS_COORD& lhs, const GPS_COORD& rhs);
std::ostream& operator<< (std::ostream& wyjscie, const GPS_COORD& gps);

#endif


