#include <db_gen.h>

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << " Usage: db_gen dirPath" << endl;
        return -1;
    }

    // 1st argument - path to the dir containing all the tiles
    auto vecImageDatabase = generateDB(argv[1]);

    return 0;
}