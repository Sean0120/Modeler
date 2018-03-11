#include "SampleModel.h"
#include<string>
#include<ctime>
#include<stdlib.h>
//create 4 kinds for rotation
// ÈÆyÖáÐý×ª $%(+-)  
// ÈÆxÖáÐý×ª ^&(+-) 
// ÈÆzÖáÐý×ª */(+-)  
string order0 = "F[*F]F[$F]";
string order1 = "F[/F]F[$F[&F]]";
string order2 = "FF*[^F^F]%[*F]";
void SampleModel::GenerateResult() {
	if (VAL(ENABLE_LSYSTEM) == 0)
	{
		return;
	}
	
	//check whether need refresh
	if (refresh_L == VAL(LSYSTEM_REFRESH)) {
		return;
	}
	else
	{
		refresh_L = VAL(LSYSTEM_REFRESH);
	}
	//first we need to get the parameter
	int iteration = VAL(LSYSTEM_ITERATION);
	//after this we need to know them, we need to generate a string which represent the command and store it in the string
	//use F to represent go forward  and use number to represent the rotation, angle will be decide in the drawtree function
	//here we just need to consider how to generate the command 
	//init 
	FractalResult = "F";
	for (int i = 0; i < iteration; ++i) {
		string temp = "";
		for (int j = 0; FractalResult[j] != '\0'; ++j) {
			if (FractalResult[j] == 'F')
			{
				switch (rand() % 3) {
					case 0: {
						temp += order0;
						break;
					}
					case 1:{
						temp += order1;
						break;
					}
					case 2: {
						temp += order2;
						break;
					}
				}
			}
			else
			{
				temp += FractalResult[j];
			}
		}
		FractalResult = temp;
		//cout << FractalResult << endl;
	}
	
};

//create 4 kinds for rotation
// ÈÆyÖáÐý×ª $%(+-)  
// ÈÆxÖáÐý×ª ^&(+-) 
// ÈÆzÖáÐý×ª */(+-)  
//we have finish the generate part, next we need to draw this on the screem, maybe much harder
void SampleModel::DrawTree() {
	if (VAL(ENABLE_LSYSTEM) == 0)
	{
		return;
	}
	//first we need to get the parameters
	float distance = VAL(LSYSTEM_DISTANCE);
	int angle = VAL(LSYSTEM_ANGLE);
	glPushMatrix();
	//cout << FractalResult << endl;
	for (int i = 0; FractalResult[i] != '\0'; ++i) {
		
		switch (FractalResult[i])
		{
			case 'F': {
				drawCylinder(distance, 0.1, 0.1);

				glTranslated(0, 0, distance);
				//cout << "draw c" << endl;
				break;
			}
			case '[': {
				glPushMatrix();
				//cout << "push" << endl;
				break;
				
			}
			case ']': {
				glPopMatrix();
				//cout << "pop" << endl;
				break;
				
			}
			case '$': {
				glRotated(angle, 0, 1.0, 0);
				//cout << "rotate" << endl;
				break;
			}
			case '%': {
				glRotated(-angle,0, 1.0 , 0); 
				//cout << "rotate" << endl;
				break;
			}
			case '^': {
				glRotated(angle, 1.0, 0, 0);
				//cout << "rotate" << endl;
				break;
			}
			case '&': {
				glRotated(-angle, 1.0, 0, 0);
				//cout << "rotate" << endl;
				break;
			}
			case '*': {
				glRotated(angle, 0, 0, 1.0);
				//cout << "rotate" << endl;
				break;
			}
			case '/': {
				glRotated(-angle, 0, 0, 1.0);
				//cout << "rotate" << endl;
				break;
			}
		}
		
	}
	glPopMatrix();
};

