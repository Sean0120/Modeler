#include"IK.h"
#include<math.h>
#include<iostream>
#include"modelerglobals.h"

#include <windows.h>
#include <Fl/gl.h>
#include <gl/glu.h>
#include"SampleModel.h"
#include"modelerui.h"
#include "camera.h"
#include "modelerglobals.h"
#include "modelerapp.h"
#include <math.h>
using namespace std;
double PI = 3.14159265359;
IK::IK(Vec3f Start, float UAL, float LAL) {
	StartPoint = Start;
	Uarm = Vec3f(0,-1,0); 
	Larm = Vec3f(0, -1, 0);
	Uarm_length = UAL;  
	Larm_length = LAL; 
	Joint = StartPoint;
	Joint[1] -= UAL;
	//cout << Joint << endl;
}

void IK::setEnd(Vec3f expected_end) {
	if (VAL(IK_CONSTRAIN) == 1) {
		if (expected_end[0] > -1.5) {
			expected_end[0] = -1.5;
		}
	}

	End = expected_end;
	calculate();
}

void IK::calculate() {
	//use a for loop to gradually approach the expected_end and at most do 100 times

	for (int i = 0; i < 100; ++i) {
		result = End; 
		Vec3f r_jdir = result - Joint;
		//without this, there is something 
		if (r_jdir * Larm == 0) {
			Joint = Joint + Vec3f(0.1, 0.1, 0.1);
		}
		Larm = End - Joint;
		Larm.normalize(); // get the new direction 

		//cout << End << endl;
		//cout << Larm_length << endl;
		//cout << Larm << endl;
		Joint = End - Larm_length * Larm;
		Uarm = Joint - StartPoint;
		Uarm.normalize();
		//cout << Joint << endl;
		//cout << Uarm_length * Uarm << endl;
		Vec3f FakeStart = Joint - Uarm_length * Uarm;
		//cout << FakeStart << endl;
		Vec3f gap = FakeStart - StartPoint;
		//cout << StartPoint << endl;
		Joint -= gap;
		//cout << gap << endl;
		
		//cout << "----" << endl;
		float distance = gap.length();
		if (distance < threshold)
		{
			break;
		}
		
	}
	//cout << Joint << endl;
	//cout << result << endl;
	//till here we have get the joint, next we will use it to calculate the result 
	//actually 2 rotation is enough 
	Vec3f x_axes(1, 0, 0);
	Vec3f y_axes(0, 1, 0);
	Vec3f z_axes(0, 0, 1);
	//calculate the upper arm roation 
	Vec3f UAproj_y = (Uarm * y_axes) * y_axes;//the projection on y axes
	Vec3f rotationVec = Uarm - UAproj_y;
	Vec3f UAproj_yz = Vec3f(0, UAproj_y[1], rotationVec.length());
	if (UAproj_y.length() == 0) {
		//perhaps not necessary
		if (Uarm[2] > 0)
		{
			Urotation_x = -90;
		}
		else
		{
			Urotation_x = 90;
		}
	}
	else
	{
		Urotation_x = -(acos(UAproj_yz * (-y_axes)) / PI * 180);
	}

	if (rotationVec.length()==0)
	{
		Urotation_y = 0;
	}
	else
	{
		Urotation_y = acos(z_axes*rotationVec / rotationVec.length()) / PI * 180;
	}
	if (Uarm[0] < 0)
		Urotation_y = -Urotation_y;
	//next we need to calculate the rotation of the lower arm, it is quite similar to the upper arm 

	Vec3f LAproj_y = (Larm*y_axes) * y_axes;
	rotationVec = Larm - LAproj_y;
	Vec3f LAproj_yz(0, LAproj_y[1], rotationVec.length());

	if (rotationVec.length() == 0)
	{
		Lrotation_y = 0;
	}
	else
	{
		Lrotation_y = acos((z_axes*rotationVec) / rotationVec.length()) / PI * 180;
	}
	if (LAproj_y.length() == 0) {
		Lrotation_x = -90;
	}
	else
	{
		Lrotation_x = -acos(LAproj_yz*(-y_axes)) / PI * 180;
	}
	if (Larm[0] < 0) {
		Lrotation_y = -Lrotation_y;
	}
	//finish the calculation , next we need to adjust the rotation operatiion.
	
}