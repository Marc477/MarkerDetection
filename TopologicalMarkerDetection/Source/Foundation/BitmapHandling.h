#ifndef BITMAPHANDLING_H_
#define BITMAPHANDLING_H_

#include <string>
#include <cstdio>

#include "ColorRGB.h"
#include "image.h"

using namespace std;

// Saves a 24bit RGB bitmap image at the path specified via the argument
// aFileName, from the image aImage.
void SaveBmp(const string&  aFileName,  const Image<ColorRGB> &aImage);

// Loads a 24bit RGB bitmap image located at aFileName and stores it into
// image aImage.
void LoadBmp(const string&  aFileName, Image<ColorRGB> &aImage);

//////////////////////////////////////////////////////////////////////////

#endif //BITMAPHANDLING_H_
