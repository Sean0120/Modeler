#include"SampleModel.h"
#include"bitmap.h"
#include <FL/fl_ask.H>
void _dump_current_modelview(void);
void _dump_current_material(void);
void _setupOpenGl();
static GLuint texName;
int	width, height;
int SampleModel::loadImage() {
	unsigned char*	data;
	
	char * iname = "plane.bmp";
	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}
	
	m_ucBitmap = data;
	//init the textures
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width,
		height, 0, GL_RGB, GL_UNSIGNED_BYTE,
		m_ucBitmap);

	return 1;
}


void SampleModel::drawTriangleWithTex(double x1, double y1, double z1,
	double x2, double y2, double z2,
	double x3, double y3, double z3)
{
	ModelerDrawState *mds = ModelerDrawState::Instance();

	_setupOpenGl();

	if (mds->m_rayFile)
	{
		_dump_current_modelview();
		fprintf(mds->m_rayFile,
			"polymesh { points=((%f,%f,%f),(%f,%f,%f),(%f,%f,%f)); faces=((0,1,2));\n", x1, y1, z1, x2, y2, z2, x3, y3, z3);
		_dump_current_material();
		fprintf(mds->m_rayFile, "})\n");
	}
	else
	{
		double a, b, c, d, e, f;

		/* the normal to the triangle is the cross product of two of its edges. */
		a = x2 - x1;
		b = y2 - y1;
		c = z2 - z1;

		d = x3 - x1;
		e = y3 - y1;
		f = z3 - z1;

		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glBindTexture(GL_TEXTURE_2D, texName);
		glBegin(GL_TRIANGLES);

		glNormal3d(b*f - c*e, c*d - a*f, a*e - b*d);
		glTexCoord2f(0.0, 0.0);
		glVertex3d(x1, y1, z1);
		glTexCoord2f(1.0, 0.0);
		glVertex3d(x2, y2, z2);
		glTexCoord2f(0.5, 1.0);
		glVertex3d(x3, y3, z3);
		
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}