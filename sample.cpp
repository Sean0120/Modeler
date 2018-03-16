// The sample model.  You should build a file
// very similar to this for when you make your model.

//push
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
#include "modelerui.h"
#include "newdraw.h"
const float PI = 3.14159265f;


// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{ 
    return new SampleModel(x,y,w,h,label); 
}

static GLfloat lightPosition0[] = { 4, 2, -4, 0 };
static GLfloat lightDiffuse0[] = { 1,1,1,1 };
static GLfloat lightPosition1[] = { -2, 1, 5, 0 };
static GLfloat lightDiffuse1[] = { 1, 1, 1, 1 };

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
void SampleModel::draw()
{
    // This call takes care of a lot of the nasty projection 
    // matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
    //To finish the bonus part, we have to deal with this part manually
	if (!valid())
	{
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_NORMALIZE);
		loadImage();
	}
	if (VAL(SAMPLE_FOG) == 1) {
		SampleFog();
	}
	else {
		glDisable(GL_FOG);
	}
	glClearColor(0.5, 0.5, 0.5, 1.0);  /* fog color */

	glViewport(0, 0, w(), h());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (VAL(FRAME_ALL)) {
		//calculate the view angle
		float max_d = max(5 * VAL(WHOLE_SCALE_Z) + VAL(WHOLE_SCALE_X), 4 * VAL(WHOLE_SCALE_Z) + 1.7 * VAL(WHOLE_SCALE_X));
		float angle = atan(max_d / 20) / M_PI * 360;
		gluPerspective( angle, float(w()) / float(h()), 1.0, 100.0);
	}
	else
	{
		gluPerspective(30.0, float(w()) / float(h()), 1.0, 100.0);
	}
	//seng the value to camera
	m_camera->height = h();
	m_camera->width = w();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_camera->applyViewingTransform();
	//get the light 0 position from the sliders
	lightPosition0[0] = VAL(LIGHT_POSITION_X);
	lightPosition0[1] = VAL(LIGHT_POSITION_Y);
	lightPosition0[2] = VAL(LIGHT_POSITION_Z);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);



	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_GREEN);
	if (ModelerUserInterface::m_controlsAnimOnMenu->value() == 0) {
		Additional_Angle = 0;
	}
	else {
		Additional_Angle += 3;
		Additional_Angle %= 360;
	}
	
{
	glPushMatrix();//Whole model begin
	if (VAL(FRAME_ALL)) {
		glTranslated(0, 0, 0);
	}
	else
	{
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
	}
	//glTranslated(0, -1, 0);
	GenerateResult();
	DrawTree();
	if (VAL(ENABLE_LSYSTEM) == 1)
		return;

	if (VAL(ENABLE_IK) == 1) {
		Vec3f Destination = Vec3f(VAL(IK_X), VAL(IK_Y), VAL(IK_Z));
		IKrightArm->setEnd(Destination);
		// draw a box to show the destination 
		glPushMatrix();
		setDiffuseColor(COLOR_RED);
		glTranslated(VAL(IK_X), VAL(IK_Y), VAL(IK_Z));
		drawBox(0.2, 0.2, 0.2);
		setDiffuseColor(COLOR_GREEN);
		glPopMatrix();
	}
	{	
		glPushMatrix();//Upper body begin
		glRotated((VAL(UPPER_BODY_ROTATE) + Additional_Angle)*VAL(MOOD), 0.0, 1.0, 0.0);
		{
			//Main body
			glPushMatrix();
			if (!VAL(HAVE_MUSCLE))
			{
				glRotated(-90, 1.0, 0, 0);
				drawCylinder(4 * VAL(WHOLE_SCALE_Z), 1 * VAL(WHOLE_SCALE_X), 1 * VAL(WHOLE_SCALE_X));
			}
			else {
				glPushMatrix();
				glTranslated(0.0, 0.25*VAL(WHOLE_SCALE_Z), 0.0);
				drawOrganic(3* VAL(WHOLE_SCALE_Z), 0, 0, 1.2);
				glPopMatrix();
			}
			if (VAL(HAS_RING) == 1) {
				if(!VAL(HAVE_MUSCLE))
				glRotated(90, 1.0, 0, 0);
				glTranslated(0.0, 6.0, 0.0);
				drawTorus(0.8, 0.2);
				glTranslated(0.0, -6.0, 0.0);
				drawExtrudedSurface(f1, f3, f5, -2*M_PI, 2 * M_PI, f6, f5, f1, 0, 2);
				drawRotationSurface(0, 1, 0, 0, 0, 0, f1, f2, f5, 0, 2);
			}
			glPopMatrix();
		}
		{
			if(VAL(COLORFUL) == 1)
				setDiffuseColor(COLOR_RED);
			//Head
			glPushMatrix();
			glTranslated(0, 0.85* VAL(WHOLE_SCALE_X) + 4 * VAL(WHOLE_SCALE_Z), 0);
			drawSphere(0.85* VAL(WHOLE_SCALE_X));
			glPopMatrix();
		}
		{
			if (VAL(COLORFUL) == 1)
				setDiffuseColor(COLOR_BLUE);
			//Joints connecting upper arm and main body
			glPushMatrix();
			glTranslated(-1.5* VAL(WHOLE_SCALE_X), 4* VAL(WHOLE_SCALE_Z) - 0.5*VAL(WHOLE_SCALE_X), 0);
			drawSphere(0.5*VAL(WHOLE_SCALE_X));
			glPopMatrix();
		}
		{
			glPushMatrix();
			glTranslated(1.5* VAL(WHOLE_SCALE_X), 4 * VAL(WHOLE_SCALE_Z) - 0.5*VAL(WHOLE_SCALE_X), 0);
			drawSphere(0.5*VAL(WHOLE_SCALE_X));
			glPopMatrix();
		}
		if(VAL(ENABLE_IK) == 1){
			//maybe another harder part besides calculation
			{
				glPushMatrix();//Left arm begin
				glTranslated(-1.5, 3.5, 0);

				glRotated(IKrightArm->Urotation_y, 0.0, 1.0, 0.0);
				glRotated(IKrightArm->Urotation_x, 1.0, 0.0, 0.0);
				


				glPushMatrix();//Upper arm
				
				glRotated(90, 1.0, 0.0, 0.0);
				drawCylinder(3, 0.4, 0.4);
				glTranslated(0, 0, 3);
				drawSphere(0.5);
				glPopMatrix();
				glPopMatrix();
			}
			{
				glPushMatrix();//Lower arm begin
				glTranslated(IKrightArm->Joint[0], IKrightArm->Joint[1],IKrightArm->Joint[2]);//Lower arm rotate
				glRotated(IKrightArm->Lrotation_y, 0.0, 1.0, 0.0);
				glRotated(IKrightArm->Lrotation_x, 1.0, 0.0, 0.0);
				
				{
					glPushMatrix();//Lower arm draw
					glRotated(90, 1.0, 0.0, 0.0);
					drawCylinder(2 , 0.4, 0.4);
					glTranslated(0, 0, 2);
					drawSphere(0.5*VAL(WHOLE_SCALE_X));
					glPopMatrix();
				}
				glPopMatrix();//lower arm end
			}
			glPopMatrix();//left arm end

		}
		else // draw the right arm without IK
		{
			if (VAL(Level_OF_DETAILS) > 1)
			{
				glPushMatrix();//Left arm begin
				glTranslated(-1.5* VAL(WHOLE_SCALE_X), 4 * VAL(WHOLE_SCALE_Z) - 0.5*VAL(WHOLE_SCALE_X), 0);
				glRotated(-VAL(LEFT_UPPER_ARM_ROTATEX)*VAL(MOOD), 1.0, 0.0, 0.0);
				glRotated(-VAL(LEFT_UPPER_ARM_ROTATEZ)*VAL(MOOD), 0.0, 0.0, 1.0);
				glTranslated(1.5* VAL(WHOLE_SCALE_X), -4 * VAL(WHOLE_SCALE_Z) + 0.5*VAL(WHOLE_SCALE_X), 0);
				{
					if (VAL(COLORFUL) == 1)
						setDiffuseColor(COLOR_GRAY);
					glPushMatrix();//Upper arm
					glTranslated(-1.5* VAL(WHOLE_SCALE_X), 4 * VAL(WHOLE_SCALE_Z) - 0.5*VAL(WHOLE_SCALE_X), 0);
					if (VAL(HAVE_MUSCLE))
					{
						glPushMatrix();
						glTranslated(0, -0.25 * VAL(WHOLE_SCALE_Z), 0);
						drawOrganic(-2.2 * VAL(WHOLE_SCALE_Z), 0, 0, 1.7);
						glPopMatrix();
					}
					glRotated(90, 1.0, 0.0, 0.0);
					drawCylinder(4 * VAL(WHOLE_SCALE_Z) - VAL(WHOLE_SCALE_X), 0.4* VAL(WHOLE_SCALE_X), 0.4* VAL(WHOLE_SCALE_X));
					glPopMatrix();
				}
				{
					glPushMatrix();//Joint
					glTranslated(-1.5* VAL(WHOLE_SCALE_X), 0.5*VAL(WHOLE_SCALE_X), 0);
					drawSphere(0.5* VAL(WHOLE_SCALE_X));
					glPopMatrix();
				}
				if (VAL(Level_OF_DETAILS) > 2)
				{
					if (VAL(COLORFUL) == 1)
						setDiffuseColor(COLOR_MAROON);
					glPushMatrix();//Lower arm begin
					glTranslated(0.0, 0.5*VAL(WHOLE_SCALE_X), 0.0);//Lower arm rotate
					glRotated(-VAL(LEFT_LOWER_ARM_ROTATE)*VAL(MOOD), 1.0, 0.0, 0.0);
					glTranslated(0.0, -0.5*VAL(WHOLE_SCALE_X), 0.0);
					{
						glPushMatrix();//Lower arm draw
						glTranslated(-1.5 * VAL(WHOLE_SCALE_X), 0.5 * VAL(WHOLE_SCALE_X), 0.0);
						if (VAL(HAVE_MUSCLE))
						{
							glPushMatrix();
							glTranslated(0.0, -0.2*VAL(WHOLE_SCALE_Z), 0.0);
							drawOrganic(-1.1 * VAL(WHOLE_SCALE_Z), 0, 0, 2.3);
							glPopMatrix();
						}
						glRotated(90, 1.0, 0.0, 0.0);
						drawCylinder(2 * VAL(WHOLE_SCALE_Z), 0.4* VAL(WHOLE_SCALE_X), 0.4* VAL(WHOLE_SCALE_X));
						glPopMatrix();
					}
					if (VAL(Level_OF_DETAILS) > 3)
					{
						glPushMatrix();//left hand
						glTranslated(-1.5 * VAL(WHOLE_SCALE_X), 0.5*VAL(WHOLE_SCALE_X) - 2 * VAL(WHOLE_SCALE_Z), 0.0);
						drawSphere(0.5*VAL(WHOLE_SCALE_X));
						glPopMatrix();
					}
					glPopMatrix();//lower arm end
				}
				glPopMatrix();//left arm end
			}
		}
		if (VAL(Level_OF_DETAILS) > 1)
		{
			if (VAL(COLORFUL) == 1)
				setDiffuseColor(COLOR_PURPLE);
			glPushMatrix();//right arm begin
			//Upper arm rotate
			glTranslated(1.5* VAL(WHOLE_SCALE_X), 4 * VAL(WHOLE_SCALE_Z) - 0.5*VAL(WHOLE_SCALE_X), 0);
			glRotated(-VAL(RIGHT_UPPER_ARM_ROTATEX)*VAL(MOOD), 1.0, 0.0, 0.0);
			glRotated(-VAL(RIGHT_UPPER_ARM_ROTATEZ)*VAL(MOOD), 0.0, 0.0, 1.0);
			glTranslated(-1.5* VAL(WHOLE_SCALE_X), -4 * VAL(WHOLE_SCALE_Z) + 0.5*VAL(WHOLE_SCALE_X), 0);
			{
				glPushMatrix();//Upper arm
				glTranslated(1.5* VAL(WHOLE_SCALE_X), 4 * VAL(WHOLE_SCALE_Z) - 0.5*VAL(WHOLE_SCALE_X), 0);
				if (VAL(HAVE_MUSCLE)) {
					glPushMatrix();
					glTranslated(0, -0.25 * VAL(WHOLE_SCALE_Z), 0);
					drawOrganic(-2.2 * VAL(WHOLE_SCALE_Z), 0, 0, 1.7);
					glPopMatrix();
				}
				glRotated(90, 1.0, 0.0, 0.0);
				drawCylinder(4 * VAL(WHOLE_SCALE_Z) - VAL(WHOLE_SCALE_X), 0.4*VAL(WHOLE_SCALE_X), 0.4*VAL(WHOLE_SCALE_X));
				glPopMatrix();
			}
			{
				glPushMatrix();//Joint
				glTranslated(1.5* VAL(WHOLE_SCALE_X), 0.5*VAL(WHOLE_SCALE_X), 0);
				drawSphere(0.5* VAL(WHOLE_SCALE_X));
				glPopMatrix();
			}
			if (VAL(Level_OF_DETAILS) > 2)
			{
				glPushMatrix();//Lower arm begin
				glTranslated(0.0, 0.5*VAL(WHOLE_SCALE_X), 0.0);//Lower arm rotate
				glRotated(-VAL(RIGHT_LOWER_ARM_ROTATE)*VAL(MOOD), 1.0, 0.0, 0.0);
				glTranslated(0.0, -0.5*VAL(WHOLE_SCALE_X), 0.0);
				{
					glPushMatrix();//Lower arm draw
					glTranslated(1.5 * VAL(WHOLE_SCALE_X), 0.5 * VAL(WHOLE_SCALE_X), 0.0);
					if (VAL(HAVE_MUSCLE))
					{
						glPushMatrix();
						glTranslated(0.0, -0.2*VAL(WHOLE_SCALE_Z), 0.0);
						drawOrganic(-1.1 * VAL(WHOLE_SCALE_Z), 0, 0, 2.3);
						glPopMatrix();
					}
					glRotated(90, 1.0, 0.0, 0.0);
					drawCylinder(2 * VAL(WHOLE_SCALE_Z), 0.4* VAL(WHOLE_SCALE_X), 0.4* VAL(WHOLE_SCALE_X));
					glPopMatrix();
				}
				if (VAL(Level_OF_DETAILS) > 3)
				{
					glPushMatrix();//left hand
					glTranslated(1.5 * VAL(WHOLE_SCALE_X), 0.5*VAL(WHOLE_SCALE_X) - 2 * VAL(WHOLE_SCALE_Z), 0.0);
					drawSphere(0.5*VAL(WHOLE_SCALE_X));
					glPopMatrix();
				}
				glPopMatrix();//lower arm end
			}
			glPopMatrix();//right arm end
		}
		glPopMatrix();//Upper body end
	}
	if(VAL(Level_OF_DETAILS) > 1)
	{
		glPushMatrix();//lower body begin
		glRotated(VAL(LOWER_BODY_ROTATE)*VAL(MOOD), 0.0, 1.0, 0.0);
		{
			if (VAL(COLORFUL) == 1)
				setDiffuseColor(COLOR_TEAL);
			glPushMatrix();//hip
			glRotated(30, 0.0, 1.0, 0.0);
			for (int i = 0; i != 6; i++) {
				if (VAL(TEXTURE) == 0) {
					//glDisable(GL_TEXTURE_2D);
					drawTriangle(0									, -0.75* VAL(WHOLE_SCALE_X)	, 0									, 0.8*sin(i*PI / 3)* VAL(WHOLE_SCALE_X), -0.75* VAL(WHOLE_SCALE_X), 0.8*cos(i*PI / 3)* VAL(WHOLE_SCALE_X), 0.8*sin((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X), -0.75* VAL(WHOLE_SCALE_X), 0.8*cos((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X));
					drawTriangle(sin(i*PI / 3)* VAL(WHOLE_SCALE_X)	, 0							, cos(i*PI / 3)* VAL(WHOLE_SCALE_X)	, 0.8*sin(i*PI / 3)* VAL(WHOLE_SCALE_X), -0.75* VAL(WHOLE_SCALE_X), 0.8*cos(i*PI / 3)* VAL(WHOLE_SCALE_X), 0.8*sin((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X), -0.75* VAL(WHOLE_SCALE_X), 0.8*cos((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X));
					drawTriangle(sin(i*PI / 3)* VAL(WHOLE_SCALE_X)	, 0							, cos(i*PI / 3)* VAL(WHOLE_SCALE_X)	, 0.8*sin((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X), -0.75* VAL(WHOLE_SCALE_X), 0.8*cos((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X), sin((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X), 0, cos((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X));
				}
				if (VAL(TEXTURE) == 1) {
					drawTriangleWithTex(0, -0.75* VAL(WHOLE_SCALE_X), 0, 0.8*sin(i*PI / 3)* VAL(WHOLE_SCALE_X), -0.75* VAL(WHOLE_SCALE_X), 0.8*cos(i*PI / 3)* VAL(WHOLE_SCALE_X), 0.8*sin((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X), -0.75* VAL(WHOLE_SCALE_X), 0.8*cos((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X));
					drawTriangleWithTex(sin(i*PI / 3)* VAL(WHOLE_SCALE_X), 0, cos(i*PI / 3)* VAL(WHOLE_SCALE_X), 0.8*sin(i*PI / 3)* VAL(WHOLE_SCALE_X), -0.75* VAL(WHOLE_SCALE_X), 0.8*cos(i*PI / 3)* VAL(WHOLE_SCALE_X), 0.8*sin((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X), -0.75* VAL(WHOLE_SCALE_X), 0.8*cos((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X));
					drawTriangleWithTex(sin(i*PI / 3)* VAL(WHOLE_SCALE_X), 0, cos(i*PI / 3)* VAL(WHOLE_SCALE_X), 0.8*sin((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X), -0.75* VAL(WHOLE_SCALE_X), 0.8*cos((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X), sin((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X), 0, cos((i + 1)*PI / 3)* VAL(WHOLE_SCALE_X));
				}
			}
			glPopMatrix();
		}
		{
			glPushMatrix();//Joints connecting legs and upper body
			glTranslated(-0.5* VAL(WHOLE_SCALE_X), -0.5* VAL(WHOLE_SCALE_X), 0);
			drawSphere(0.5* VAL(WHOLE_SCALE_X));
			glTranslated(1 * VAL(WHOLE_SCALE_X), 0, 0);
			drawSphere(0.5* VAL(WHOLE_SCALE_X));
			glPopMatrix();
		}
		if (VAL(Level_OF_DETAILS) > 2)
		{
			glPushMatrix();//Left leg begin
			glTranslated(-0.5* VAL(WHOLE_SCALE_X), -0.5* VAL(WHOLE_SCALE_X), 0);//Upper leg rotate
			glRotated(-VAL(LEFT_UPPER_LEG_ROTATEX)*VAL(MOOD), 1.0, 0.0, 0.0);
			glRotated(-VAL(LEFT_KNEE)*VAL(MOOD), 1.0, 0.0, 0.0);
			glRotated(-VAL(LEFT_UPPER_LEG_ROTATEZ)*VAL(MOOD), 0.0, 0.0, 1.0);
			glTranslated(0.5* VAL(WHOLE_SCALE_X), 0.5* VAL(WHOLE_SCALE_X), 0);
			{
				glPushMatrix();//Upper leg
				glTranslated(-0.5* VAL(WHOLE_SCALE_X), -0.5* VAL(WHOLE_SCALE_X), 0);
				if (VAL(HAVE_MUSCLE))
				drawOrganic(-1.5 * VAL(WHOLE_SCALE_Z), 0, 0, 2.1);
				glRotated(90, 1.0, 0.0, 0.0);
				drawCylinder(2 * VAL(WHOLE_SCALE_Z), 0.4* VAL(WHOLE_SCALE_X), 0.4* VAL(WHOLE_SCALE_X));
				glPopMatrix();
			}
			{
				glPushMatrix();//Joint
				glTranslated(-0.5* VAL(WHOLE_SCALE_X), -0.5* VAL(WHOLE_SCALE_X) - 2 * VAL(WHOLE_SCALE_Z), 0);
				drawSphere(0.5* VAL(WHOLE_SCALE_X));
				glPopMatrix();
			}
			if (VAL(Level_OF_DETAILS) > 3)
			{
				glPushMatrix();//Lower leg begin
				glTranslated(0.0, -0.5* VAL(WHOLE_SCALE_X) - 2 * VAL(WHOLE_SCALE_Z), 0.0);//Lower leg rotate
				glRotated(-VAL(LEFT_LOWER_LEG_ROTATE)*VAL(MOOD), 1.0, 0.0, 0.0);
				glRotated(VAL(LEFT_KNEE)*VAL(MOOD), 1.0, 0.0, 0.0);
				glTranslated(0.0, 0.5* VAL(WHOLE_SCALE_X) + 2 * VAL(WHOLE_SCALE_Z), 0.0);
				{
					glPushMatrix();//Lower leg draw
					glTranslated(-0.5* VAL(WHOLE_SCALE_X), -0.5* VAL(WHOLE_SCALE_X) - 2 * VAL(WHOLE_SCALE_Z), 0.0);
					if (VAL(HAVE_MUSCLE))
					drawOrganic(-2.0* VAL(WHOLE_SCALE_Z), 0, 0, 1.8);
					glRotated(90, 1.0, 0.0, 0.0);
					drawCylinder(3 * VAL(WHOLE_SCALE_Z), 0.4* VAL(WHOLE_SCALE_X), 0.4* VAL(WHOLE_SCALE_X));
					glPopMatrix();
				}
				{
					glPushMatrix();//left foot
					glTranslated(-0.5* VAL(WHOLE_SCALE_X), -3 * VAL(WHOLE_SCALE_Z) - 0.5* VAL(WHOLE_SCALE_X) - 2 * VAL(WHOLE_SCALE_Z), 0.0);
					drawSphere(0.5* VAL(WHOLE_SCALE_X));
					glRotated(VAL(LEFT_FOOT_ROTATE)*VAL(MOOD), 1.0, 0.0, 0.0);
					drawCylinder(1.3, 0.5*VAL(WHOLE_SCALE_X), 0.5*VAL(WHOLE_SCALE_X));
					glPopMatrix();
				}
				glPopMatrix();//lower leg end
			}
			glPopMatrix();//left leg end
		}
		if (VAL(Level_OF_DETAILS) > 2)
		{
			if (VAL(COLORFUL) == 1)
				setDiffuseColor(COLOR_ORANGE);
			glPushMatrix();//Right leg begin
			glTranslated(0.5* VAL(WHOLE_SCALE_X), -0.5* VAL(WHOLE_SCALE_X), 0);//Upper leg rotate
			glRotated(-VAL(RIGHT_UPPER_LEG_ROTATEX)*VAL(MOOD), 1.0, 0.0, 0.0);
			glRotated(-VAL(RIGHT_KNEE)*VAL(MOOD), 1.0, 0.0, 0.0);
			glRotated(-VAL(RIGHT_UPPER_LEG_ROTATEZ)*VAL(MOOD), 0.0, 0.0, 1.0);
			glTranslated(-0.5* VAL(WHOLE_SCALE_X), 0.5* VAL(WHOLE_SCALE_X), 0);
			{
				glPushMatrix();//Upper leg
				glTranslated(0.5* VAL(WHOLE_SCALE_X), -0.5* VAL(WHOLE_SCALE_X), 0); 
				if (VAL(HAVE_MUSCLE))
				drawOrganic(-1.5 * VAL(WHOLE_SCALE_Z), 0, 0,2.1);
				glRotated(90, 1.0, 0.0, 0.0);
				drawCylinder(2 * VAL(WHOLE_SCALE_Z), 0.4* VAL(WHOLE_SCALE_X), 0.4* VAL(WHOLE_SCALE_X));
				glPopMatrix();
			}
			{
				glPushMatrix();//Joint
				glTranslated(0.5* VAL(WHOLE_SCALE_X), -0.5* VAL(WHOLE_SCALE_X) - 2 * VAL(WHOLE_SCALE_Z), 0);
				drawSphere(0.5* VAL(WHOLE_SCALE_X));
				glPopMatrix();
			}
			if (VAL(Level_OF_DETAILS) > 3)
			{
				glPushMatrix();//Lower leg begin
				glTranslated(0.0, -0.5* VAL(WHOLE_SCALE_X) - 2 * VAL(WHOLE_SCALE_Z), 0.0);//Lower leg rotate
				glRotated(-VAL(RIGHT_LOWER_LEG_ROTATE)*VAL(MOOD), 1.0, 0.0, 0.0);
				glRotated(VAL(RIGHT_KNEE)*VAL(MOOD), 1.0, 0.0, 0.0);
				glTranslated(0.0, 0.5* VAL(WHOLE_SCALE_X) + 2 * VAL(WHOLE_SCALE_Z), 0.0);
				{
					glPushMatrix();//Lower leg draw
					glTranslated(0.5* VAL(WHOLE_SCALE_X), -0.5* VAL(WHOLE_SCALE_X) - 2 * VAL(WHOLE_SCALE_Z), 0.0);
					if(VAL(HAVE_MUSCLE))
					drawOrganic(-2.0* VAL(WHOLE_SCALE_Z), 0, 0,1.8);
					glRotated(90, 1.0, 0.0, 0.0);
					drawCylinder(  3 * VAL(WHOLE_SCALE_Z), 0.4* VAL(WHOLE_SCALE_X), 0.4* VAL(WHOLE_SCALE_X));
					glPopMatrix();
				}
				{
					glPushMatrix();//Right foot
					glTranslated(0.5* VAL(WHOLE_SCALE_X), -3 * VAL(WHOLE_SCALE_Z) - 0.5* VAL(WHOLE_SCALE_X) - 2 * VAL(WHOLE_SCALE_Z), 0.0);
					drawSphere(0.5* VAL(WHOLE_SCALE_X));
					glRotated(VAL(RIGHT_FOOT_ROTATE)*VAL(MOOD),1.0,0.0,0.0);
					drawCylinder(1.3, 0.5*VAL(WHOLE_SCALE_X), 0.5*VAL(WHOLE_SCALE_X));
					glPopMatrix();
				}
				glPopMatrix();//lower leg end
			}
			glPopMatrix();//Right leg end
		}
		glPopMatrix();//lower body end
	}
	
	glPopMatrix();//Whole model end
}

}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];
    controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
    controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
 	controls[LIGHT_POSITION_X] = ModelerControl("light0 position x", -5, 5, 0.1f, 4);
	controls[LIGHT_POSITION_Y] = ModelerControl("light0 position y", -5, 5, 0.1f, 2);
	controls[LIGHT_POSITION_Z] = ModelerControl("light0 position z", -5, 5, 0.1f, -4);
	controls[UPPER_BODY_ROTATE] = ModelerControl("Upper Body Rotate", -90, 90, 1, 0);
	controls[LOWER_BODY_ROTATE] = ModelerControl("Lower Body Rotate", -90, 90, 1, 0);
	controls[LEFT_UPPER_ARM_ROTATEX] = ModelerControl("Left Arm Rotate X", -135, 135, 1, 0);
	controls[LEFT_UPPER_ARM_ROTATEZ] = ModelerControl("Left Arm Rotate Z", -135, 135, 1, 0);
	controls[RIGHT_UPPER_ARM_ROTATEX] = ModelerControl("Right Arm Rotate X", -135, 135, 1, 0);
	controls[RIGHT_UPPER_ARM_ROTATEZ] = ModelerControl("Right Arm Rotate Z", -135, 135, 1, 0);
	controls[LEFT_LOWER_ARM_ROTATE] = ModelerControl("Left Fore Arm Rotate", -135, 135, 1, 0);
	controls[RIGHT_LOWER_ARM_ROTATE] = ModelerControl("Right Fore Arm Rotate", -135, 135, 1, 0);
	controls[LEFT_UPPER_LEG_ROTATEX] = ModelerControl("Left Leg Rotate X", -135, 135, 1, 0);
	controls[LEFT_UPPER_LEG_ROTATEZ] = ModelerControl("Left Leg Rotate Z", -135, 135, 1, 0);
	controls[RIGHT_UPPER_LEG_ROTATEX] = ModelerControl("Right Leg Rotate X", -135, 135, 1, 0);
	controls[RIGHT_UPPER_LEG_ROTATEZ] = ModelerControl("Right Leg Rotate Z", -135, 135, 1, 0);
	controls[LEFT_LOWER_LEG_ROTATE] = ModelerControl("Left Fore Leg Rotate", -135, 135, 1, 0);
	controls[LEFT_KNEE] = ModelerControl("Left Knee", 0, 90, 1, 0);
	controls[RIGHT_LOWER_LEG_ROTATE] = ModelerControl("Right Fore Leg Rotate", -135, 135, 1, 0);
	controls[RIGHT_KNEE] = ModelerControl("Right Knee", 0, 90, 1, 0);
	controls[LEFT_FOOT_ROTATE] = ModelerControl("Left foot Rotate", -135, 135, 1, 0);
	controls[RIGHT_FOOT_ROTATE] = ModelerControl("Right foot Rotate", -135, 135, 1, 0);
	controls[SAMPLE_FOG] = ModelerControl("sample fog effect", 0, 1, 1, 0);
	controls[Level_OF_DETAILS] = ModelerControl("levels of details", 1, 4, 1, 4);
	controls[TEXTURE] = ModelerControl("Using Texture", 0, 1, 1, 0);
	controls[WHOLE_SCALE_X] = ModelerControl("whole scale x", 0, 2, 0.01, 1.0);
	controls[WHOLE_SCALE_Z] = ModelerControl("whole scale z", 0, 2, 0.01, 1.0);
	controls[FRAME_ALL] = ModelerControl("Frame all", 0, 1, 1, 0);
	controls[TWIST_CAMERA] = ModelerControl("enable to twist camera", 0, 1, 1, 0);
	controls[HAS_RING] = ModelerControl("Has Ring", 0, 1, 1, 0);
	controls[ENABLE_LSYSTEM] = ModelerControl("enable L system", 0, 1, 1, 0);
	controls[LSYSTEM_DISTANCE] = ModelerControl("change the distance in L system", 0, 0.5, 0.01, 0.25);
	controls[LSYSTEM_ANGLE] = ModelerControl("change the angle in L system", 0, 90, 1, 45);
	controls[LSYSTEM_ITERATION] = ModelerControl("change the iteration number", 1, 4, 1, 1);
	controls[LSYSTEM_REFRESH] = ModelerControl("refresh the L system",0,1,1,0);
	controls[COLORFUL] = ModelerControl("make the model colorful", 0, 1, 1, 0);
	controls[ENABLE_IK] = ModelerControl("enable IK", 0, 1, 1, 0);
	controls[IK_X] = ModelerControl("set the goal x", -4.5, 1.5, 0.01, -1.5);
	controls[IK_Y] = ModelerControl("set the goal y", -4.5, 1.5, 0.01, -1.5);
	controls[IK_Z] = ModelerControl("set the goal z", 3, -3, 0.01, 0);
	controls[IK_CONSTRAIN] = ModelerControl("enable constrain in IK", 0, 1, 1, 0);
	controls[HAVE_MUSCLE] = ModelerControl("Have Muscle", 0, 1, 1, 0);
	controls[MOOD] = ModelerControl("Mood", 0, 1, 0.05, 1);
    ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}
