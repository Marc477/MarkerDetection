#ifndef MARKERDATA_H_
#define MARKERDATA_H_

//Simple 2D Point structure
struct Point2D
{
    float X;
    float Y;
};

// Simple Bounding box structure, the four data members
// correspond to the extrimities of the bounding box.
// When grouped together, they should form an axis-aligned
// bounding box.
struct AABB
{
    float Top;
    float Bottom;
    float Left;
    float Right;
};

//Data outputted by the marker detection system. Gives the marker's
//location, size and axis-aligned bounding box.
struct MarkerData
{
    //  The unique identifier of the marker. The identifier is given
    //  when reading the dictionary file.
    int             Id;

    // Centroid of the marker, basically an average position of all the
    // pixels detected to be part of the marker.
    Point2D         Centroid;

    // The bounding box of the marker in window space (pixel coordinates)
    AABB            BoundingBox;
};

#endif //MARKERDATA_H_
