#ifndef MARCHINGCUBE_H
#define MARCHINGCUBE_H

#include "vec.h"
#include "ValueVec.h"
#include "MCTable.h"

typedef struct {
	Vec3f p[3];
	Vec3f norm;
} TRIANGLE;

//Linear interpolation
Vec3f LinearInterp(ValueVec p1, ValueVec p2, float val);

//POINTERS TO FUNCTIONS
//pointer to function which computes if point p is outside the surface
typedef bool(*OUTSIDE)(Vec3f);

//pointer to function which determines the point of intersection of the edge with 
//the isosurface between points p1 and p2
//any other information is passed in the void array mcInfo
typedef Vec3f(*INTERSECTION)(ValueVec, ValueVec, float);

//pointer to function which computes the value at point p
typedef float(*FORMULA)(Vec3f);

//Generate triangles
TRIANGLE* MarchingCubes(int ncellsX, int ncellsY, int ncellsZ, float minValue, ValueVec * points,
	INTERSECTION intersection, int &numTriangles);

float Potential(Vec3f p, double posx, double posy, double posz);

#endif 
