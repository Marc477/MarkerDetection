#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "MarkerData.h"
#include "TopologicalMarkerDetection.h"

using namespace std;

int main(int argc, char *argv[])
{
    string DictionaryFileName;
    string TargetImageFileName;

	if(argc > 2){
		DictionaryFileName = argv[1];
		TargetImageFileName = argv[2];
		cout << DictionaryFileName << endl;
		cout << TargetImageFileName << endl;
		cout << endl;
	}
	else
	{
		cout    << "Please enter the name of the marker dictionary file :" << endl;
		cin     >> DictionaryFileName;

		cout    << "Please enter the name of the target image file :" << endl;
		cin     >> TargetImageFileName;
	}

    vector<MarkerData> Results = DetectMarkers( DictionaryFileName,
                                                TargetImageFileName);

    for(unsigned int i = 0; i < Results.size(); ++i)
    {
		float sizeX = (Results[i].BoundingBox.Right-Results[i].BoundingBox.Left);
		float sizeY = (Results[i].BoundingBox.Top-Results[i].BoundingBox.Bottom);

        cout << "Marker #" << i << endl;
        cout << "-----------------------" << endl;
        cout << "ID: " << Results[i].Id << endl;
        cout << "Centroid: (" <<    Results[i].Centroid.X << ", ";
        cout <<                     Results[i].Centroid.Y << ")" << endl;
		cout << "Size: " << sizeX << "x" << sizeY << endl;
        cout << "Bounding Box: " << endl;
		cout << "  Left-Right: " << Results[i].BoundingBox.Left << ", " << Results[i].BoundingBox.Right   << endl;
        cout << "  Bottom-Top: " << Results[i].BoundingBox.Bottom << ", " << Results[i].BoundingBox.Top  << endl;
		cout << endl;
    }

	system("pause");

    return 0;
}
