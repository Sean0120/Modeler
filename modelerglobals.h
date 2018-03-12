#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values 
// of the controls from the user interface.
enum SampleModelControls
{ 
	XPOS, YPOS, ZPOS, 

	UPPER_BODY_ROTATE,
	LEFT_UPPER_ARM_ROTATEX, LEFT_UPPER_ARM_ROTATEZ, LEFT_LOWER_ARM_ROTATE, 
	RIGHT_UPPER_ARM_ROTATEX, RIGHT_UPPER_ARM_ROTATEZ, RIGHT_LOWER_ARM_ROTATE,
	LOWER_BODY_ROTATE,
	LEFT_UPPER_LEG_ROTATEX, LEFT_UPPER_LEG_ROTATEZ, LEFT_LOWER_LEG_ROTATE,
	RIGHT_UPPER_LEG_ROTATEX, RIGHT_UPPER_LEG_ROTATEZ, RIGHT_LOWER_LEG_ROTATE,
	LIGHT_POSITION_X,
	LIGHT_POSITION_Y,
	LIGHT_POSITION_Z,
	Level_OF_DETAILS,
	SAMPLE_FOG,
	TEXTURE,
	WHOLE_SCALE_X,
	WHOLE_SCALE_Z,
	FRAME_ALL,
	TWIST_CAMERA,
	HAS_RING,
	ENABLE_LSYSTEM,
	LSYSTEM_DISTANCE,
	LSYSTEM_ANGLE,
	LSYSTEM_ITERATION,
	LSYSTEM_REFRESH,
	COLORFUL,
	NUMCONTROLS
};

// Colors
#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f
#define COLOR_GRAY		0.5f, 0.5f, 0.5f
#define COLOR_MAROON	0.5f, 0.0f, 0.0f
#define COLOR_PURPLE	0.5f, 0.0f, 0.5f
#define COLOR_TEAL		0,0.5,0.5
#define COLOR_ORANGE    1,0.5,0
// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif