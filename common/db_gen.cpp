#include "db_gen.h"

using namespace boost::filesystem;
using namespace std;

IMAGE_DB generateDB(string inputDirPath) // creates IMAGE_DB for images in given inputDirPath, @see IMAGE_DB typedef in db_gen.h
{
    // 1st argument - path to the dir containing all the tiles
    path dirPath(inputDirPath);
    // default directory_iterator construction yields past-the-end
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
    IMAGE_DB vecImageDb;                                        // create empty vector for storing final version of the image DB (sorted and 2D), @see IMAGE_DB typedef in db_gen.h

    vecRowDb.push_back(vecImageInterimDb.front());              // push back the FIRST image to the row DB vector

    for (auto it = vecImageInterimDb.cbegin() + 1; it != vecImageInterimDb.cend(); ++it)                // start iterating from the 2nd element as the 1st one has already been pushed back
    {
        if (fabs(vecRowDb.back().GetTopLeft().GetLatitudeDouble() - it->GetTopLeft().GetLatitudeDouble()) < numeric_limits<double>::epsilon())   
            vecRowDb.push_back(*it);                                                                    // image belongs to the same row in DB if the latitude is the same as in previous element
        else                                                                                            // image belongs to the next row in DB if the latitude is different to the previous element's latitude
        {
            vecImageDb.push_back(vecRowDb);                                                             // push back the previous row 
            vecRowDb.clear();                                                                           // clear the row DB vector 
            vecRowDb.push_back(*it);                                                                    // and fill with the first element of new row
        }
    }
    vecImageDb.push_back(vecRowDb);                                                                     // push back the LAST row 

    //// DEBUG
    //std::copy(vecRowDb.begin(), vecRowDb.end(), std::ostream_iterator<GPS_TAGGED_IMAGE>(std::cout, " "));
    //cout << vecImageDb.size() << endl;
    //for (auto n : vecImageDb) {
    //    cout << n.size() << " ";
    //}

    return vecImageDb;
}