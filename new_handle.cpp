#include"SampleModel.h"
#include"modelerui.h"
static const int	kMouseRotationButton = FL_LEFT_MOUSE;
static const int	kMouseTranslationButton = FL_MIDDLE_MOUSE;
static const int	kMouseZoomButton = FL_RIGHT_MOUSE;

int SampleModel::handle(int event)
{
	unsigned eventCoordX = Fl::event_x();
	unsigned eventCoordY = Fl::event_y();
	unsigned eventButton = Fl::event_button();
	unsigned eventState = Fl::event_state();

	switch (event)
	{
	case FL_PUSH:
	{
		if (VAL(TWIST_CAMERA) == 0) {
			switch (eventButton)
			{
			case kMouseRotationButton:
				m_camera->clickMouse(kActionRotate, eventCoordX, eventCoordY);
				break;
			case kMouseTranslationButton:
				m_camera->clickMouse(kActionTranslate, eventCoordX, eventCoordY);
				break;
			case kMouseZoomButton:
				m_camera->clickMouse(kActionZoom, eventCoordX, eventCoordY);
				break;
			}
		}
		else
		{
			switch (eventButton)
			{
			case kMouseRotationButton:
				m_camera->clickMouse(kActionTwist, eventCoordX, eventCoordY);
				break;
			case kMouseTranslationButton:
				m_camera->clickMouse(kActionTranslate, eventCoordX, eventCoordY);
				break;
			case kMouseZoomButton:
				m_camera->clickMouse(kActionZoom, eventCoordX, eventCoordY);
				break;
			}
		}
		// printf("push %d %d\n", eventCoordX, eventCoordY);
	}
	break;
	case FL_DRAG:
	{
		m_camera->dragMouse(eventCoordX, eventCoordY);
		//printf("drag %d %d\n", eventCoordX, eventCoordY);
	}
	break;
	case FL_RELEASE:
	{
		switch (eventButton)
		{
		case kMouseRotationButton:
		case kMouseTranslationButton:
		case kMouseZoomButton:
			m_camera->releaseMouse(eventCoordX, eventCoordY);
			break;
		}
		//  printf("release %d %d\n", eventCoordX, eventCoordY);
	}
	break;
	case FL_MOVE:
		return 0;
	//default:
		//if(ModelerUserInterface::m_controlsAnimOnMenu->value() == 0)
			//return 0;
	}

	redraw();

	return 1;
}