#include <string>
#include <exiv2/exiv2.hpp>
#include <gps_tagged_image.h>

#include <boost/filesystem.hpp>   // includes all needed Boost.Filesystem declarations
#include <iostream>               

using namespace boost::filesystem;         
using namespace std;


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << " Usage: db_gen dirPath" << endl;
        return -1;
    }
    
    // 1st argument - path to the dir containing all the tiles
    path dirPath(argv[1]);
    directory_iterator end_itr;

    vector<GPS_TAGGED_IMAGE> vecImageInterimDb; // create empty vector for storing interim version of the image DB (unsorted and 1D)

    try
    {
        if (exists(dirPath))                    // does dirPath actually exist?
        {
            if (is_directory(dirPath))          // is dirPath a directory?
            {

                for (directory_iterator i(dirPath); i != end_itr; ++i)                      // for each file in dirPath
                {
                    if (is_regular_file(i->status()))                                       // is file a regular file?
                    {
                        vecImageInterimDb.push_back(GPS_TAGGED_IMAGE(i->path().string()));  // push back GPS_TAGGED_IMAGE created using data from file (images will be unsorted)
                    }
                }
            }
            else
                cerr << " ERROR: " << dirPath << " is not a valid directory!" << endl;
        }
        else
            cerr << " ERROR: " << dirPath << " does not exist!" << endl;
    }

    catch (const filesystem_error& ex)
    {
        cout << ex.what() << '\n';
    }

    sort(vecImageInterimDb.begin(), vecImageInterimDb.end());   // sort the interim DB vector (by means of overloaded operator<)

    vector <GPS_TAGGED_IMAGE> vecRowDb;                         // create empty vector for storing one row of image DB
    vector <vector <GPS_TAGGED_IMAGE>> vecImageDb;              // create empty vector for storing final version of the image DB (sorted and 2D)

    vecRowDb.push_back(vecImageInterimDb.front());              // push back the FIRST image to the row DB vector
    
    for (auto it = vecImageInterimDb.cbegin() + 1; it != vecImageInterimDb.cend(); ++it)                // start iterating from the 2nd element as the 1st one has already been pushed back
    {
        if (vecRowDb.back().GetTopLeft().GetLatitudeDouble() == it->GetTopLeft().GetLatitudeDouble())   // image belongs to the same row in DB if the latitude is the same as in previous element
            vecRowDb.push_back(*it);
        else                                                                                            // image belongs to the next row in DB if the latitude is different to the previous element's latitude
        {
            vecImageDb.push_back(vecRowDb);                                                             // push back the previous row 
            vecRowDb.clear();                                                                           // clear the row DB vector 
            vecRowDb.push_back(*it);                                                                    // and fill with the first element of new row
        }
    }
    vecImageDb.push_back(vecRowDb);                                                                     // push back the LAST row 

    //std::cout << vecImageDb.size() << endl;
    //for (auto n : vecImageDb) {
    //    std::cout << n.size() << " ";
    //}
    
    
    return 0;
}