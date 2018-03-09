#ifndef NEWDRAW_H
#define NEWDRAW_H

#include <FL/gl.h>
#include <Gl/glu.h>
#include <cstdio>
#include <math.h>
#include "modelerglobals.h"
#include "modelerdraw.h"
void _dump_current_modelview(void);
void _dump_current_material(void);
void _setupOpenGl();
void drawExtrudedSurface(double (*x1)(double), double (*y1)(double), double (*z1)(double), double s1, double e1,
                         double (*x2)(double), double (*y2)(double), double (*z2)(double), double s2, double e2);

void drawRotationSurface(double x1, double y1, double z1,
                         double x0, double y0, double z0,
                         double (*x2)(double), double (*y2)(double), double (*z2)(double), double s2, double e2);

void drawTorus(double r1, double r2);

void drawExtrudedSurface(double (*x1)(double), double (*y1)(double), double (*z1)(double), double s1, double e1,
                         double (*x2)(double), double (*y2)(double), double (*z2)(double), double s2, double e2)
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    _setupOpenGl();
    
    double delta;
    
    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, "");
        //"polymesh { points=((%f,%f,%f),(%f,%f,%f),(%f,%f,%f)); faces=((0,1,2));\n", x1, y1, z1, x2, y2, z2, x3, y3, z3);
        _dump_current_material();
        fprintf(mds->m_rayFile, "})\n");
    }
    else
    {
        switch (mds->m_quality)
        {
            case HIGH:
                delta = 1e-2; break;
            case MEDIUM:
                delta = 5e-2; break;
            case LOW:
                delta = 8e-2; break;
            case POOR:
                delta = 0.1; break;
        }
        delta = 0.1;
        double pdx, pdy, pdz;
        pdx = pdy = pdz = 0;
        for (double i = s1; i < (e1-s1); i+=delta) {
            double dx = x1(i + delta) - x1(i);
            double dy = y1(i + delta) - y1(i);
            double dz = z1(i + delta) - z1(i);
            
            for (double j = s2; j < (e2-s2); j+=delta) {
                glBegin(GL_POLYGON);
                glVertex3d(x2(j)+pdx, y2(j)+pdy,z2(j)+pdz);//std::cout << x2(j) << " " << y2(j) << " " << z2(j) << " " << std::endl;
                glVertex3d(x2(j+delta)+pdx, y2(j+delta)+pdy,z2(j+delta)+pdz);
                glVertex3d(x2(j + delta)+dx+pdx, y2(j + delta)+dy+pdy, z2(j + delta)+dz+pdz);
                glVertex3d(x2(j)+dx+pdx, y2(j)+dy+pdy, z2(j)+dz+pdz);
                
                glEnd();
            }
            
            pdx += dx;
            pdy += dy;
            pdz += dz;
        }
        
    }
}

void drawRotationSurface(double x1, double y1, double z1,
                         double x0, double y0, double z0,
                         double (*x2)(double), double (*y2)(double), double (*z2)(double), double s2, double e2)
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    _setupOpenGl();
    
    double delta;
    
    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, "");
        //"polymesh { points=((%f,%f,%f),(%f,%f,%f),(%f,%f,%f)); faces=((0,1,2));\n", x1, y1, z1, x2, y2, z2, x3, y3, z3);
        _dump_current_material();
        fprintf(mds->m_rayFile, "})\n");
    }
    else
    {
        switch (mds->m_quality)
        {
            case HIGH:
                delta = 1e-2; break;
            case MEDIUM:
                delta = 5e-2; break;
            case LOW:
                delta = 8e-2; break;
            case POOR:
                delta = 0.1; break;
        }
        int savemode;
        glGetIntegerv(GL_MATRIX_MODE, &savemode);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslated(x0, y0, z0);
        for (int i = 0; i < 720*2; i++) {
            glPushMatrix();
            for (double j = s2; j <= e2; j += delta) {
                
                glBegin(GL_LINES);
                glVertex3d(x2(j), y2(j), z2(j));
                glVertex3d(x2(j + delta), y2(j + delta), z2(j + delta));
                
                glEnd();
                
            }
            glPopMatrix();
            glRotated(0.25, x1, y1, z1);
        }
        glPopMatrix();
        glMatrixMode(savemode);
    }
}

void drawTorus(double r1, double r2)
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    _setupOpenGl();
    
    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, "");
        //"polymesh { points=((%f,%f,%f),(%f,%f,%f),(%f,%f,%f)); faces=((0,1,2));\n", x1, y1, z1, x2, y2, z2, x3, y3, z3);
        _dump_current_material();
        fprintf(mds->m_rayFile, "})\n");
    }
    else
    {
        int savemode;
        glGetIntegerv(GL_MATRIX_MODE, &savemode);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        for (int i = 0; i < 360; i++) {
            glPushMatrix();
            glTranslated(r1, 0.0, 0.0);
            drawCylinder(2 * (M_PI+1e-4)/180*r1, r2, r2);
            glPopMatrix();
            glRotated(1, 0.0, 1.0, 0.0);
        }
        
        glPopMatrix();
        glMatrixMode(savemode);
    }
}

double f1(double i) { return i; };
double f2(double i) { return i*i; };
double f3(double i) { return sin(i); };
double f4(double i) { return cos(i); };
double f5(double i) { return 0; };
double f6(double i) { return -2*M_PI; };
double f7(double i) { return -i; };

#endif
