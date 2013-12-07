#ifndef _GPS_TAGGED_IMAGE_CLASS_H_
#define _GPS_TAGGED_IMAGE_CLASS_H_

//#include <exiv2/exiv2.hpp>
#include <string>
#include <iostream>

#include <gps_coord.h>

class GPS_TAGGED_IMAGE
{
public:
    // constructors
    GPS_TAGGED_IMAGE(std::string sFilepath, GPS_COORD topLeft, GPS_COORD botRght);
    GPS_TAGGED_IMAGE(std::string filepath);
    
    // destructor not needed

    // member operators
    bool operator< (const GPS_TAGGED_IMAGE& rhs) const;

    // methods
    std::string				GetFilepath()   const { return sFilepath; };
    GPS_COORD				GetTopLeft()    const { return topLeft; };
    GPS_COORD				GetBotRght()    const { return botRght; };
        
    void                    SetFilepath();
    void                    SetTopLeft(GPS_COORD newTopLeft)  { topLeft = newTopLeft; }
    void                    SetBotRght(GPS_COORD newBotRght)  { botRght = newBotRght; }

private:
    std::string             sFilepath;
    GPS_COORD               topLeft, botRght; 
    
};

// non-member operators
std::ostream& operator<< (std::ostream& wyjscie, const GPS_TAGGED_IMAGE& gps);

#endif


