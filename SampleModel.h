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
		: ModelerView(x, y, w, h, label) { }

	virtual void draw();
	void SampleFog();
};

#endif // !SAMPLEMODEL
