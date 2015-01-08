/*
	Marc-Antoine Desbiens
	September 2012
*/

#ifndef TOPOLOGICALMARKERDETECTION_H_
#define TOPOLOGICALMARKERDETECTION_H_

#include <string>
#include <vector>
#include <stack>

#include "MarkerData.h"

// Detects topological markers within an image. The image is located at the
// aTargetImageFileName location and the marker dictionary at 
// aDictionaryFileName.
//

//Dictionary struc
struct Dict{
	int id;
	std::string data;
};

//Pixel structure
struct Pixel
{
	int X;
	int Y;

	Pixel(){}
	Pixel(int mX, int mY){X = mX; Y = mY;}
};

//Region structure
struct Region
{
	static const int ROOT = 0;

	int index;
	int parentIndex;
	bool color; //Black or white
	int minX, minY, maxX, maxY;
	std::vector<int> childIndex;
	std::string nodeString;

	Region(){index=ROOT; parentIndex=ROOT;}
	Region(int i, int parent){index=i; parentIndex=parent;}
};

struct ChildPixel
{
	int parentIndex;
	Pixel pixel;

	ChildPixel(int parent, Pixel p){parentIndex = parent; pixel = p;}
};

// The function returns a vector containing marker data for all the markers
// found in the image aTargetImageFile. See "MarkerData.h" to see which
// information has to be returned.
std::vector<MarkerData> DetectMarkers(const std::string& aDictionaryFileName,
                                      const std::string& aTargetImageFileName);

#endif //TOPOLOGICALMARKERDETECTION_H_
