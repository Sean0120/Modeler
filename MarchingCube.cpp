#include "MarchingCube.h"
//dont
Vec3f LinearInterp(ValueVec p1, ValueVec p2, float val)
{
	Vec3f p;
	if (p1.value != p2.value)
		p = (Vec3f)p1 + ((Vec3f)p2 - (Vec3f)p1) / (p2.value - p1.value)*(val - p1.value);
	else
		p = (Vec3f)p1;
	return p;
}

TRIANGLE* MarchingCubes(int ncellsX, int ncellsY, int ncellsZ, float minValue, ValueVec * points,
	INTERSECTION intersection, int &numTriangles) {
	TRIANGLE * triangles = new TRIANGLE[3 * ncellsX*ncellsY*ncellsZ];
	numTriangles = 0;
	int YtimeZ = (ncellsY + 1)*(ncellsZ + 1);

	//Go through all the cubes
	for(int i=0; i<ncellsX; ++i)
		for(int j=0; j<ncellsY; ++j)
			for (int k = 0; k < ncellsZ; ++k) {
				ValueVec verts[8];	//8 vertices of the cube
				int ind = i * YtimeZ + j * (ncellsZ + 1) + k;	//The index of the first vertex of the cube
				
				verts[0] = points[ind];
				verts[1] = points[ind + YtimeZ];
				verts[2] = points[ind + YtimeZ + 1];
				verts[3] = points[ind + 1];
				verts[4] = points[ind + (ncellsZ + 1)];
				verts[5] = points[ind + YtimeZ + (ncellsZ + 1)];
				verts[6] = points[ind + YtimeZ + (ncellsZ + 1) + 1];
				verts[7] = points[ind + (ncellsZ + 1) + 1];

				int cubeIndex = 0;	
				for (int n = 0; n < 8; n++)
					if (verts[n].value <= minValue) cubeIndex |= (1 << n);

				if (!edgeTable[cubeIndex]) continue;	//check if all inside or outside

				Vec3f intVerts[12];	//The intersection points on the 12 edges of the cube, the sepcific order can be found on the internet
				if (edgeTable[cubeIndex] & 1) intVerts[0] = intersection(verts[0], verts[1], minValue);
				if (edgeTable[cubeIndex] & 2) intVerts[1] = intersection(verts[1], verts[2], minValue);
				if (edgeTable[cubeIndex] & 4) intVerts[2] = intersection(verts[2], verts[3], minValue);
				if (edgeTable[cubeIndex] & 8) intVerts[3] = intersection(verts[3], verts[0], minValue);
				if (edgeTable[cubeIndex] & 16) intVerts[4] = intersection(verts[4], verts[5], minValue);
				if (edgeTable[cubeIndex] & 32) intVerts[5] = intersection(verts[5], verts[6], minValue);
				if (edgeTable[cubeIndex] & 64) intVerts[6] = intersection(verts[6], verts[7], minValue);
				if (edgeTable[cubeIndex] & 128) intVerts[7] = intersection(verts[7], verts[4], minValue);
				if (edgeTable[cubeIndex] & 256) intVerts[8] = intersection(verts[0], verts[4], minValue);
				if (edgeTable[cubeIndex] & 512) intVerts[9] = intersection(verts[1], verts[5], minValue);
				if (edgeTable[cubeIndex] & 1024) intVerts[10] = intersection(verts[2], verts[6], minValue);
				if (edgeTable[cubeIndex] & 2048) intVerts[11] = intersection(verts[3], verts[7], minValue);

				//Generate triangles by looking up triTable in MCTable.h to find the type of the intersection
				for (int n = 0; triTable[cubeIndex][n] != -1; n += 3) {
				 	triangles[numTriangles].p[0] = intVerts[triTable[cubeIndex][n + 2]];
					triangles[numTriangles].p[1] = intVerts[triTable[cubeIndex][n + 1]];
					triangles[numTriangles].p[2] = intVerts[triTable[cubeIndex][n]];
					Vec3f temp = ((triangles[numTriangles].p[1] -
								triangles[numTriangles].p[0]) ^ (triangles[numTriangles].p[2] -
								triangles[numTriangles].p[0]));
					temp.normalize();
					triangles[numTriangles].norm = temp;
					numTriangles++;
				}
			}
	TRIANGLE * retTriangles = new TRIANGLE[numTriangles];	//Generate triangles
	for (int i = 0; i < numTriangles; i++)
		retTriangles[i] = triangles[i];
	delete[] triangles;

	return retTriangles;
}

float Potential(Vec3f p, double posx, double posy, double posz)
{
	Vec3f dp1 = Vec3f(0, posx,0) - p;
	Vec3f dp2 = Vec3f(0, 0, 0) - p;
	//Vec3f dp3 = Vec3f(posx, -posx, 0.0) - p;
	//Vec3f dp4 = Vec3f(-posx, posx, 0.0) - p;
	//Vec3f dp5 = Vec3f(-0.5, 3.1, -1.0) - p;
	//Vec3f dp6 = Vec3f(0.0, 0.0, -4.0) - p;
	//return (1 - dp1.length2())*(1 - dp1.length2()) + (1 - dp2.length2())*(1 - dp2.length2());
	return 1 / dp1.length() + 1 / dp2.length();//+ 1 / dp3.length() + 1 / dp4.length();//+ 1 / dp5.length() +
		//1 / dp6.length();
}