#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <string>
#include <exiv2/exiv2.hpp>
#include <gps_coord.h>

using namespace cv;
using namespace std;

// NOT NEEDED, leave here just in case...
// defining IMAGE_POINT_TUPLE
// 1st variable: int        -> pixel x-coordinate 
// 2nd variable: int        -> pixel y-coordinate
// 3rd variable: GPS_COORD  -> GPS coordinates correspoding to the pixel 
//typedef tuple < int, int, GPS_COORD > IMAGE_POINT_TUPLE;
// usage:
// IMAGE_POINT_TUPLE c(0, 0, GPS_COORD(0.0, 0.0));
// auto a = IMAGE_POINT_TUPLE(0, 0, GPS_COORD(0.0, 0.0));
// IMAGE_POINT_TUPLE b = make_tuple(0, 0, GPS_COORD(0.0, 0.0));

int checkDimensions(Mat& imageToCheck, int iTileSize)
{
	int iReturnValue = 0;

	if (imageToCheck.rows % iTileSize != 0) iReturnValue += 1;
	if (imageToCheck.cols % iTileSize != 0) iReturnValue += 1;

	return iReturnValue;
}

void cropImage(Mat& imageToCrop, GPS_COORD topLeft, GPS_COORD& bottomRight, int iTileSize)
{
    // calculate new rows and columns numbers
	int iNewRowsNumber = (imageToCrop.rows / iTileSize) * iTileSize;
	int iNewColsNumber = (imageToCrop.cols / iTileSize) * iTileSize;

    // calculate new bottom right corner's GPS coordinates
    // top left corner's GPS coordinates remain unchanged
    GPS_COORD scaleFactor((double)iNewColsNumber / (double)imageToCrop.cols, (double)iNewRowsNumber / (double)imageToCrop.rows);
    GPS_COORD cornersDiff = bottomRight - topLeft; // imagine it as a vector (in a mathematical way)
    cornersDiff *= scaleFactor;
    bottomRight = topLeft + cornersDiff;

	imageToCrop = Mat(imageToCrop, Rect(0, 0, iNewColsNumber, iNewRowsNumber));
}

void prepareTiles(Mat& imageToCut, GPS_COORD topLeft, GPS_COORD bottomRight, int iTileSize)
{
    // NEED ANOTHER IMAGE TO TEST
    GPS_COORD cornersDiff = bottomRight - topLeft;
    GPS_COORD gpsTileLongitudeSpacingVector(((double)iTileSize / (double)imageToCut.cols) * cornersDiff.GetLongitudeDouble(), 0.0);
    GPS_COORD gpsTileLatitudeSpacingVector(0.0, ((double)iTileSize / (double)imageToCut.rows) * cornersDiff.GetLatitudeDouble());
    GPS_COORD gpsTileDiameterVector(gpsTileLongitudeSpacingVector + gpsTileLatitudeSpacingVector);

    //// testing GPS_COORD overloaded operators
    //GPS_COORD gpsTileDiameterVector2 = gpsTileLongitudeSpacingVector + gpsTileLatitudeSpacingVector;
    //GPS_COORD gpsTileDiameterVector3(((double)iTileSize / (double)imageToCut.cols) * cornersDiff.GetLongitudeDouble(), ((double)iTileSize / (double)imageToCut.rows) * cornersDiff.GetLatitudeDouble());
    //GPS_COORD gpsTileDiameterVector4 = cornersDiff * ((double)iTileSize / (double)imageToCut.rows);
    //GPS_COORD gpsTileDiameterVector5 = ((double)iTileSize / (double)imageToCut.cols)* cornersDiff;
    //cout << gpsTileDiameterVector << gpsTileDiameterVector2 << gpsTileDiameterVector3 << gpsTileDiameterVector4 << gpsTileDiameterVector5;

    for (int i = 0; i < imageToCut.rows; i = i + iTileSize)
    {
        for (int j = 0; j < imageToCut.cols; j = j + iTileSize)
        {
            // crop tile from the source image and save it
            Mat tile(imageToCut, Rect(i, j, iTileSize, iTileSize));
            string filepath = "./../resources/tiles/" + to_string(i) + "_" + to_string(j) + ".jpg";
            // TODO to arguments?

            try
            {
                imwrite(filepath, tile);
                cout << "Writing " << filepath << endl;
            }
            catch (runtime_error& ex)
            {
                cout << "ERROR! Exception while saving tile image! Check output folder!" << ex.what() << endl;
            }
            
            // calculate GPS coordinates of the tile's corners
            GPS_COORD gpsTileTopLeft = topLeft + ((j / iTileSize) * gpsTileLongitudeSpacingVector) + ((i / iTileSize) * gpsTileLatitudeSpacingVector);
            GPS_COORD gpsTileBotRght = gpsTileTopLeft + gpsTileDiameterVector;

            // reopen the tile using Exiv2 method (needed for writing tags)
            Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filepath);
            assert(image.get() != 0);
            image->readMetadata();
            Exiv2::ExifData &exifData = image->exifData();

            
            // top left corner uses "Exif.GPSInfo.GPS_____" tags, bottom right uses "Exif.GPSInfo.GPSDest_____" tags
            // top left corner:
            exifData["Exif.GPSInfo.GPSLatitudeRef"]     = gpsTileTopLeft.GetLatitudeRef();
            exifData["Exif.GPSInfo.GPSLatitude"]        = gpsTileTopLeft.GetLatitudeString();
            exifData["Exif.GPSInfo.GPSLongitudeRef"]    = gpsTileTopLeft.GetLongitudeRef();
            exifData["Exif.GPSInfo.GPSLongitude"]       = gpsTileTopLeft.GetLongitudeString();
            // bottom right corner:
            exifData["Exif.GPSInfo.GPSDestLatitudeRef"] = gpsTileBotRght.GetLatitudeRef();
            exifData["Exif.GPSInfo.GPSDestLatitude"]    = gpsTileBotRght.GetLatitudeString();
            exifData["Exif.GPSInfo.GPSDestLongitudeRef"]= gpsTileBotRght.GetLongitudeRef();
            exifData["Exif.GPSInfo.GPSDestLongitude"]   = gpsTileBotRght.GetLongitudeString();

            // another method of adding tags (for future reference
            //Exiv2::RationalValue::AutoPtr rv(new Exiv2::RationalValue);
            //rv->read("51/1 12/1 2/1");
            //key = Exiv2::ExifKey("Exif.GPSInfo.GPSLongitude");
            //exifData.add(key, rv.get());

            // write tags to file
            image->setExifData(exifData);
            image->writeMetadata();

        }
    }
}

int main(int argc, char** argv)
{
    if (argc != 6 && argc != 7)
	{
		cout << " Usage: bmp_splitter pathToImage topLeftCornerGPSLongitude topLeftCornerGPSLatitude bottomRightCornerGPSLongitude bottomRightCornerGPSLatitude [iTileSize = 100]" << endl;
		waitKey(0); 
		return -1;
	}
    
    // 1st argument - filepath to source image file
	string sourceFilepath(argv[1]);
    
    // 2nd to 5th argument - GPS coordinates
    // no need to try,catch, because strtod handles it internally!
    GPS_COORD gpsTopLeftCorner(strtod(argv[2], NULL), strtod(argv[3], NULL));
    GPS_COORD gpsBotRghtCorner(strtod(argv[4], NULL), strtod(argv[5], NULL));

    if (gpsTopLeftCorner.GetLatitudeDouble() > gpsBotRghtCorner.GetLatitudeDouble() &&
        gpsTopLeftCorner.GetLongitudeDouble() < gpsBotRghtCorner.GetLongitudeDouble())
    {
        cout << "Parsing parameters OK!" << endl;
    }
    else
    {
        cout << "Wrong GPS parameters! Quitting..." << endl;
        waitKey(0);
        return -1;
    }

    // optional 6th argument - iTileSize (default = 100 [px])
    int iTileSize;
    if (argc == 6)
    {
        iTileSize = 100;
    }
    else // argc == 7
        iTileSize = strtol(argv[6], NULL, 10);

    // read source image from the given path
	Mat sourceBmp;
	sourceBmp = imread(sourceFilepath, CV_LOAD_IMAGE_UNCHANGED);
    
    // Check for invalid input
	if (!sourceBmp.data)                              
	{
		cout << "Could not open or find the image" << std::endl;
		waitKey(0);
		return -1;
	}

    // check if the source image's dimensions are dividable by iTileSize and crop the source image if needed
    if (checkDimensions(sourceBmp, iTileSize) > 0)
    {
        cropImage(sourceBmp, gpsTopLeftCorner, gpsBotRghtCorner, iTileSize);
    }

    // prepare tiles with GPS info
    prepareTiles(sourceBmp, gpsTopLeftCorner, gpsBotRghtCorner, iTileSize);
	

	//namedWindow("Display window", CV_GUI_EXPANDED);        // Create a window for display.
	//imshow("Display window", sourceBmp);                   // Show our image inside it.

	waitKey(0);                                          // Wait for a keystroke in the window // DO NOT USE cin.get();!!!!
	return 0;
}