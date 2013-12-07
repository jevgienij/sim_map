#ifndef _DB_GEN_H_
#define _DB_GEN_H_

#include <limits>
#include <iostream>          
#include <string>
#include <exiv2/exiv2.hpp>
#include <boost/filesystem.hpp>   // includes all needed Boost.Filesystem declarations    
#include <gps_tagged_image.h> 

typedef std::vector <std::vector <GPS_TAGGED_IMAGE>> IMAGE_DB;

//std::vector <std::vector <GPS_TAGGED_IMAGE>> generateDB(std::string inputDirPath);
IMAGE_DB generateDB(std::string inputDirPath);

#endif




