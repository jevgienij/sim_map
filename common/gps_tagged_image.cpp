#include "gps_tagged_image.h"

// constructors
GPS_TAGGED_IMAGE::GPS_TAGGED_IMAGE(std::string filepath, GPS_COORD tl, GPS_COORD br) : sFilepath(filepath), topLeft(tl), botRght(br)
{
    if ((topLeft < botRght) == false)
    {
        std::cerr << "ERROR! GPS_TAGGED_IMAGE::GPS_TAGGED_IMAGE(std::string filepath, GPS_COORD topLeft, GPS_COORD botRght) constructor:" << std::endl;
        std::cerr << "       topLeft > botRght = true, invalid GPS_COORD parameters given! Creating empty instance of GPS_TAGGED_IMAGE!" << std::endl;
        sFilepath.clear();
        topLeft = GPS_COORD();
        botRght = GPS_COORD();
    }
}

GPS_TAGGED_IMAGE::GPS_TAGGED_IMAGE(std::string filepath) : sFilepath(filepath)
{
    Exiv2::Image::AutoPtr image;

    try 
    {
        image = Exiv2::ImageFactory::open(sFilepath);
    }
    catch (Exiv2::Error e) 
    {
        std::cerr << "Failed to open file " << sFilepath <<std::endl;
    }

    image->readMetadata();
    
    if (image.get() == NULL) 
    {
        std::cerr << "Failed to read file " << sFilepath << std::endl;
    }

    Exiv2::ExifData &exifData = image->exifData();
    topLeft = GPS_COORD(exifData["Exif.GPSInfo.GPSLongitude"].toString(), exifData["Exif.GPSInfo.GPSLongitudeRef"].toString(), exifData["Exif.GPSInfo.GPSLatitude"].toString(), exifData["Exif.GPSInfo.GPSLatitudeRef"].toString());
    botRght = GPS_COORD(exifData["Exif.GPSInfo.GPSDestLongitude"].toString(), exifData["Exif.GPSInfo.GPSDestLongitudeRef"].toString(), exifData["Exif.GPSInfo.GPSDestLatitude"].toString(), exifData["Exif.GPSInfo.GPSDestLatitudeRef"].toString());
    

    if ((topLeft < botRght) ==  false)
    {
        std::cerr << "ERROR! GPS_TAGGED_IMAGE::GPS_TAGGED_IMAGE(std::string filepath) constructor:" << std::endl;
        std::cerr << "       topLeft > botRght = true, invalid GPS_COORD parameters read from image! Creating empty instance of GPS_TAGGED_IMAGE!" << std::endl;
        sFilepath.clear();
        topLeft = GPS_COORD();
        botRght = GPS_COORD();
    }
}


// member operators
bool GPS_TAGGED_IMAGE::operator< (const GPS_TAGGED_IMAGE& rhs) const
{
    return (this->topLeft < rhs.topLeft);
}

// non-member operators
std::ostream& operator<< (std::ostream& wyjscie, const GPS_TAGGED_IMAGE& img)
{
    return wyjscie << "Filepath: " << img.GetFilepath() << std::endl << "Top Left: " << img.GetTopLeft() << std::endl << "Bot Rght: " << img.GetBotRght() << std::endl;
}
