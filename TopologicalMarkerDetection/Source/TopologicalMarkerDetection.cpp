/*
	Marc-Antoine Desbiens
	September 2012
*/

#include "TopologicalMarkerDetection.h"

#include "Foundation\BitmapHandling.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

bool strSortComparaison (string s1, string s2) 
{ 
	//Sort by length first, then sort by alphabetic order
	if (s1.length() == s2.length())
		return ( s1 < s2 );
	else
		return ( s1.length() < s2.length() ); 
}

// Balance the topological tree to always get the same tree for the same marker
void BalanceTree(string& tree)
{
	string srcTree;
	vector<string> subTree;
	int treeBegin, treeEnd, len;
	int cntBracket=0;

	srcTree = tree;
	len = tree.length();

	//Divide srcTree into subTrees
	treeBegin=0;
	while(treeBegin<len)
	{
		treeEnd=treeBegin;
		if(treeBegin+1<len && srcTree.substr(treeBegin+1,1) == "["){
			cntBracket = 1;
			treeEnd++;
			while(cntBracket != 0 && treeEnd!=string::npos){
				treeEnd = srcTree.find_first_of("[]", treeEnd+1);
				if (treeEnd!=string::npos){
					if(srcTree.substr(treeEnd,1) == "[")
						cntBracket++;
					if(srcTree.substr(treeEnd,1) == "]")
						cntBracket--;
				}
			}
		}

		if (treeEnd!=string::npos){
			subTree.push_back(srcTree.substr(treeBegin,treeEnd-treeBegin+1));
		}

		treeBegin = treeEnd+1;
	}

	//Balance each subTree if tree length > 3
	string subTempo;
	for(int i=0; i<subTree.size(); i++)
	{
		if(subTree[i].length() >3)
		{
			subTempo = subTree[i].substr(2, subTree[i].size()-3); //Keep children only   b[...keep this...]
			BalanceTree(subTempo);
			subTree[i].replace(2, subTree[i].size()-3, subTempo); //Replace by balanced children
		}
	}

	//Sort sub tree
	sort(subTree.begin(), subTree.end(), strSortComparaison);

	//Set tree output
	stringstream ss;
	for(int i=0; i<subTree.size(); i++)
	{
		ss << subTree[i];
	}
	tree = ss.str();
	ss.clear();
}

// Segmentation algorithm #1
float KMeans(Image<ColorRGB> &img)
{
	float temp;
	float lastU1, lastU2;
	float sum1, sum2;
	int nb1, nb2;
	float greyPixel;
	float u1,u2;

	u1=u2 = 0;
	lastU1=lastU2 = -1;

	//Loop until u = lastU
	while( abs(u1 - lastU1) > 0.02 || abs(u2 - lastU2) > 0.02 )
	{
		//Set last U
		lastU1 = u1;
		lastU2 = u2;

		//initial random means
		u1=u2=0;
		while( abs(u1 - u2) < 0.1 )
		{
			u1 = (float) rand()/(float)RAND_MAX; //0 to 1
			u2 = (float) rand()/(float)RAND_MAX;
		}

		//Switch u1 u2 to make u1 lower
		if(u2<u1)
		{
			temp = u2;
			u2 = u1;
			u1 = temp;
		}

		//Add all nearest pixels
		sum1=sum2=0;
		nb1=nb2=0;
		for(int x=0; x<img.GetWidth(); x++)
		{
			for(int y=0; y<img.GetHeight(); y++)
			{
				greyPixel = (img.GetPixel(x,y).R() + img.GetPixel(x,y).G() + img.GetPixel(x,y).B())/3;
				if( abs(greyPixel-u1) < abs(greyPixel-u2))
				{
					//Closer to u1
					sum1 += greyPixel;
					nb1++;
				}
				else{
					//Closer to u2
					sum2 += greyPixel;
					nb2++;
				}
			}
		}

		//Set final means u1 u2
		if(nb1!=0 && nb2 !=0){
			u1 = sum1/nb1;
			u2 = sum2/nb2;
		}
	}
	return (u1+u2)/2;
}

// Segmentation algorithm #2
float OtsuSegment(Image<ColorRGB> &img)
{
	float threshold=0.5;

	// Calculate histogram
	int histogram[256];
	float grayscale;
	int index;

	for(int i=0; i<256; i++)
		histogram[i]=0;

	for(int i=0; i<img.GetWidth(); i++)
	{
		for(int j=0; j<img.GetHeight(); j++)
		{
			grayscale = (img.GetPixel(i,j).R()+img.GetPixel(i,j).G()+img.GetPixel(i,j).B())/3;
			index = (int) (grayscale*255.99f); //255.99f to make sure index is < 256 and not <=256
		    histogram[index] ++;
		}
	}

	//Otsu segmentation
	int size;
	float sum = 0;
	
	float value=0, valueMax=0;
	float sum1=0, sum2=0;
	int w1=0, w2=0;
	float u1,u2;

	// Total number of pixels
	size = img.GetWidth()*img.GetHeight();
	for (int i=0 ; i<256 ; i++) 
		sum += i * histogram[i];

	for (int t=0 ; t<256 ; t++) {
	   
       //Number of pixel in each class with threshold t
	   w1 += histogram[t];
	   w2 = size - w1;

	   //Find mean sum
	   sum1 += (float) (t * histogram[t]);
	   sum2 = sum-sum1;

	   //Find means
	   u1 = sum1 / w1;
	   u2 = sum2 / w2;

	   // Calculate threshold Variance
	   value = (float)w1 * (float)w2 * (u1 - u2) * (u1 - u2);

	   // Check if new value is higher
	   if (value > valueMax) {
		  valueMax = value;
		  threshold = (float)t/256.0f;
	   }
	}
	return threshold;
}

// Find a threshold and convert the image into a black/white (2 colors) version
void SegmentBW(Image<ColorRGB> &img)
{
	float u1,u2;
	float tot1=0, tot2=0;
	int nbIter=12;

	float threshold;
	float greyPixel;

	//Find threshold using otsu algo
	threshold = OtsuSegment(img);
	cout << "Threshold: " << threshold << endl;

	//Apply threshold
	for(int x=0; x<img.GetWidth(); x++){
		for(int y=0; y<img.GetHeight(); y++){
			greyPixel = (img.GetPixel(x,y).R() + img.GetPixel(x,y).G() + img.GetPixel(x,y).B())/3;
			if(greyPixel  > threshold)
				img.SetPixel(x, y, ColorRGB::White);
			else
				img.SetPixel(x, y, ColorRGB::Black);
		}
	}

}

//Check borders and return true if pixel inside borders
bool isPixelInImg(Image<ColorRGB> &img, Pixel pixel)
{
	if(pixel.X >= 0 && pixel.X < img.GetWidth() && pixel.Y >=0 && pixel.Y < img.GetHeight())
		return true;
	else
		return false;
}

//Return true if in img, pixel1 is same color (black or white) then pixel2
bool isSamePixel(Image<ColorRGB> &img, Pixel pixel1, Pixel pixel2)
{
	float g1,g2;
	
	g1 = img.GetPixel(pixel1.X, pixel1.Y).R();
	g2 = img.GetPixel(pixel2.X, pixel2.Y).R();

	if(abs(g1-g2) < 0.01){
		return true;
	}
	return false;
}

//Fill a region, fill visited pixel and children list
void FillRegion(Image<ColorRGB> &img, Pixel startPixel, Region &region, bool **visitedPixels, stack<ChildPixel> &childrenPixels)
{
	Pixel currentPixel, nextPixel;
	stack<Pixel> regionPixels;

	//init min max
	region.maxX=region.minX=startPixel.X;
	region.maxY=region.minY=startPixel.Y;

	//Loop until region is filled
	regionPixels.push(startPixel);
	while(!regionPixels.empty())
	{
		currentPixel = regionPixels.top();
		regionPixels.pop();

		if(!visitedPixels[currentPixel.X][currentPixel.Y])
		{
			visitedPixels[currentPixel.X][currentPixel.Y] = 1;

			//Set min max
			if(currentPixel.X < region.minX)
				region.minX = currentPixel.X;
			if(currentPixel.Y < region.minY)
				region.minY = currentPixel.Y;
			if(currentPixel.X > region.maxX)
				region.maxX = currentPixel.X;
			if(currentPixel.Y > region.maxY)
				region.maxY = currentPixel.Y;

			//Do for all 4 neighbors
			for(int i=0; i<4; i++)
			{
				switch(i){
					case 0: nextPixel = Pixel(currentPixel.X-1, currentPixel.Y);
						break;
					case 1: nextPixel = Pixel(currentPixel.X+1, currentPixel.Y);
						break;
					case 2: nextPixel = Pixel(currentPixel.X, currentPixel.Y-1);
						break;
					case 3: nextPixel = Pixel(currentPixel.X, currentPixel.Y+1);
						break;
				}
			
				if( isPixelInImg(img, nextPixel) )
				{
					if(!visitedPixels[nextPixel.X][nextPixel.Y])
					{
						//Check is same color (black or white)
						if(isSamePixel(img, currentPixel, nextPixel) )
							regionPixels.push(nextPixel);
						else
							childrenPixels.push(ChildPixel(region.index, nextPixel));
					}
				}
				else if( region.parentIndex != Region::ROOT )
					region.parentIndex = Region::ROOT; //Set parent to root if region next to img border
			
			}
		}
	}
}

//Create region list from input image
void CreateImgTree(Image<ColorRGB> &img, vector<Region> &regions)
{
	bool **visitedPixels;
	stack<ChildPixel> childrenPixels; //List of all pixels to explore
	Pixel nextRegionStartup;
	int parentIndex;

	//Create visited pixels map
	visitedPixels = new bool*[img.GetWidth()];
	for(int i=0; i<img.GetWidth(); i++){
		visitedPixels[i] = new bool[img.GetHeight()];
		for(int j=0; j<img.GetHeight(); j++){
			visitedPixels[i][j] = 0;
		}
	}

	//Start filling and creates regions
	regions.push_back(Region()); //Create root region node
	regions[0].color = (int) (img.GetPixel(0, 0).R() + 0.5);
	FillRegion(img, Pixel(0,0), regions[0], visitedPixels, childrenPixels);
	int i=1;
	while(!childrenPixels.empty())
	{
		//Next child
		nextRegionStartup = childrenPixels.top().pixel;
		parentIndex = childrenPixels.top().parentIndex;
		childrenPixels.pop();

		//Create new region
		if(!visitedPixels[nextRegionStartup.X][nextRegionStartup.Y]){
			regions.push_back(Region(i, parentIndex));
			regions[i].color = (int) (img.GetPixel(nextRegionStartup.X, nextRegionStartup.Y).R() + 0.5);
			FillRegion(img, nextRegionStartup, regions[i], visitedPixels, childrenPixels);
			i++;
		}
	}

	//Set children index
	int index;
	for(int i=1; i<regions.size(); i++)
	{
		index = regions[i].parentIndex;
		regions[index].childIndex.push_back(i);
	}

	//Delete visited pixels map
	for(int i=0; i<img.GetWidth(); i++){
		delete[] visitedPixels[i];
	}
	delete[] visitedPixels;

}

// Read Dictionnary (.txt file) that contains marker definitions
void LoadDictionary(const string& file, std::vector<Dict>& dict)
{
	ifstream filestream(file.c_str());
	string line;
	Dict d;
	int sep;

	if (filestream.is_open())
    {
		while ( filestream.good() )
		{
		  getline(filestream,line);
		  sep = line.find(":");
		  d.id = atoi(line.substr(0,sep).c_str());
		  d.data = line.substr(sep+1);
		  BalanceTree(d.data);
		  dict.push_back(d);
		}
		filestream.close();
	}
}

//Look in the dictionnary for the string representing the region
string FindRegionString(vector<Region> &regionList, Region &region)
{
	string s="";
	stringstream ss;

	if(region.color)
		s = "w";
	else
		s = "b";

	if(region.childIndex.size() > 0)
	{
		ss << s << "[";
		for(int i=0; i<region.childIndex.size(); i++)
		{
			ss  << FindRegionString(regionList, regionList[region.childIndex[i]]);
		}
		ss << "]";
		s = ss.str();
	}
	region.nodeString = s;
	return s;
}

//Search all markers in img and add to vector
void FindMarkers(string &imgTree, vector<Dict> &dictData, vector<Region> regionList, vector<MarkerData> &markerVect)
{
	string markerTree;
	MarkerData markerData;
	int pos, posNode;
	for(int i=0; i<dictData.size(); i++)
	{
		//First find marker in entire image
		markerTree = dictData[i].data;
		pos = imgTree.find(markerTree);
		if(pos != string::npos){
			//Found marker
			markerData.Id = dictData[i].id;

			//Search marker in nodes to find position
			for(int j=0; j<regionList.size(); j++)
			{
				if(regionList[j].nodeString.length() >= markerTree.length()){
					BalanceTree(regionList[j].nodeString);
					posNode = regionList[j].nodeString.find(markerTree);
					if(posNode == 0){
						markerData.BoundingBox.Left = regionList[j].minX;
						markerData.BoundingBox.Right = regionList[j].maxX;
						markerData.BoundingBox.Top = regionList[j].maxY;
						markerData.BoundingBox.Bottom = regionList[j].minY;
						markerData.Centroid.X = (regionList[j].minX+regionList[j].maxX)/2;
						markerData.Centroid.Y = (regionList[j].minY+regionList[j].maxY)/2;
						markerVect.push_back(markerData);
					}
				}
			}
		}
	}
}

// Main function for detecting markers
std::vector<MarkerData> DetectMarkers(const std::string& aDictionaryFileName, 
                                      const std::string& aTargetImageFileName)
{
	std::vector<MarkerData> markerVect;
	std::vector<Dict> dictData;
	string imgTree;
	Image<ColorRGB> img;
	vector<Region> regionList; //List of all the regions
	
	//Load image and dictionnary
	LoadBmp(aTargetImageFileName, img);
	LoadDictionary(aDictionaryFileName, dictData);

	//Get black & white version of the image
	SegmentBW(img);

	//Create img tree
	CreateImgTree(img, regionList);
	imgTree = FindRegionString(regionList, regionList[0]);
	BalanceTree(imgTree);

	//Search all markers in img and add to vector
	FindMarkers(imgTree, dictData, regionList, markerVect);

	//Save test B&W image
	SaveBmp("Output.bmp",  img);
	cout << endl;

    return markerVect;
}


