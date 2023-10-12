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
	MODEL_DETAIL,
	ENDER_HEAD,
	ENDER_BODY,
	ENDER_HAND,
	ENDER_LEG,
	HANDS_MOVEMENT,
	LEGS_MOVEMENT,
	EYE_MOVEMENT,
	HEAD_X_ROTATE,
	HEAD_Y_ROTATE,
	HEAD_Z_ROTATE,
	RU_HAND_X_ROTATE,
	RU_HAND_Y_ROTATE,
	RU_HAND_Z_ROTATE,
	RL_HAND_X_ROTATE,
	RU_FINGER_Z_ROTATE,
	RL_FINGER_Z_ROTATE,
	RT_HAND_Z_ROTATE,
	LU_HAND_X_ROTATE,
	LU_HAND_Y_ROTATE,
	LU_HAND_Z_ROTATE,
	LL_HAND_X_ROTATE,
	LU_FINGER_Z_ROTATE,
	LL_FINGER_Z_ROTATE,
	LT_HAND_Z_ROTATE,
	WAIST_X_ROTATE,
	WAIST_Y_ROTATE,
	RU_LEG_X_ROTATE,
	RU_LEG_Y_ROTATE,
	RU_LEG_Z_ROTATE,
	RL_LEG_X_ROTATE,
	RL_LEG_Y_ROTATE,
	RL_LEG_Z_ROTATE,
	LU_LEG_X_ROTATE,
	LU_LEG_Y_ROTATE,
	LU_LEG_Z_ROTATE,
	LL_LEG_X_ROTATE,
	LL_LEG_Y_ROTATE,
	LL_LEG_Z_ROTATE,
	LIGHT_POS_X,
	LIGHT_POS_Y,
	LIGHT_POS_Z,
	NUMCONTROLS
};

// Colors
#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f
#define COLOR_WHITE		1.0f, 1.0f, 1.0f
#define COLOR_BLACK		0.0f, 0.0f, 0.0f
#define COLOR_DARK_PURPLE	0.784f, 0.0f, 0.980f
#define COLOR_PURPLE	0.871f, 0.478f, 0.980f

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif