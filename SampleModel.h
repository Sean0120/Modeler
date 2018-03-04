#pragma once
#ifndef SAMPLEMODEL
#define SAMPLEMODEL

#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>

#include "modelerglobals.h"

#include "camera.h"

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.h>

#include <GL/glu.h>
#include <cstdio>
#include <math.h>
#include"SampleModel.h"

// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView
{
public:
	SampleModel(int x, int y, int w, int h, char *label)
		: ModelerView(x, y, w, h, label) {
		Additional_Angle = 0;
		
	}

	virtual void draw();
	void SampleFog();
	unsigned char*	m_ucBitmap;
	int loadImage();
	void drawTriangleWithTex(double x1, double y1, double z1,
		double x2, double y2, double z2,
		double x3, double y3, double z3);
	int Additional_Angle;
	int handle(int event);
};

#endif // !SAMPLEMODEL
