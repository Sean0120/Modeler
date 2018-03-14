#pragma once
#ifndef _IK_H
#define _IK_H
#include "vec.h"
const float threshold = 0.05;// set a threshold for iteration
class IK
{
private:
	Vec3f StartPoint; // start point of the arm 
	Vec3f Uarm; //store the current upper arm direction , save time 
	Vec3f Larm; //store the current lower arm direction , save time
	float Uarm_length; // store the length of the upper arm 
	float Larm_length; // store the length of the lower arm
	
	Vec3f End;
public:
	IK(Vec3f Start, float UAL, float LAL);
	void setEnd(Vec3f);
	void calculate();
	float Urotation_x;
	float Urotation_y;
	float Lrotation_x;
	float Lrotation_y;
	Vec3f Joint; // store the position of joint
	Vec3f result;
};
#endif // !_IK_H
