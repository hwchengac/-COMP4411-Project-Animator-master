#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "camera.h"
#include <FL/gl.h>
#include <GL/glu.h>

#include "modelerglobals.h"
#include "particleSystem.h"
#include "force.h"
#include <stdlib.h>
#include <time.h>

Mat4f getModelViewMatrix()
{
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	Mat4f matMV(m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);

	return matMV.transpose(); // convert to row major
}

void SpawnParticles(Mat4f CameraTransforms, Vec3f colour, Vec3f velocity, int no_of_particles)
{
	Mat4f WorldMatrix = CameraTransforms.inverse() * getModelViewMatrix();
	Vec3f WorldPoint = WorldMatrix * Vec3f(0, 0, 0);
	for (int i = 0; i < no_of_particles; ++i)
	{
		srand(time(0));
		int v1 = rand() % 3 - 2;
//		srand(time(0));
		int v2 = rand() % 3 - 2;
//		srand(time(0));
		int v3 = rand() % 3 - 2;
		Vec3f rand_velocity = Vec3f(velocity[0] + v1, velocity[1] + v2, velocity[2] + v3);
		ModelerApplication::Instance()->GetParticleSystem()->AddParticleStartingAt(WorldPoint, colour, rand_velocity);
	}
}

// To make a SampleModel, we inherit off of ModelerView
class MinecraftModel : public ModelerView
{
public:
	MinecraftModel(int x, int y, int w, int h, char* label)
		: ModelerView(x, y, w, h, label) { }

	virtual void draw();
private:
	float animation_angle = 0.0;
	float step = 2.0;
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char* label)
{
	return new MinecraftModel(x, y, w, h, label);
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
void MinecraftModel::draw()
{
	// This call takes care of a lot of the nasty projection 
	// matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
	//ModelerView::draw();

	//if (ModelerApplication::Instance()->getAnimating() == true) {
	//	animation_angle += step;
	//	if (animation_angle >= 45 || animation_angle <= -45) {
	//		step = -step;
	//	}
	//}

	/*if (!valid())
	{
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_NORMALIZE);
	}

	glViewport(0, 0, w(), h());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, float(w()) / float(h()), 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ModelerView::m_camera->applyViewingTransform();

	GLfloat lightPosition0[] = { 4, 2, -10, 0 };
    GLfloat lightDiffuse0[] = { 0.3,0.3,0.3,1 };
	GLfloat lightPosition1[] = { -2, 1, 10, 0 };
	GLfloat lightDiffuse1[] = { 0.3, 0.3, 0.3, 1 };
	GLfloat lightPosition2[] = { VAL(LIGHT_POS_X), VAL(LIGHT_POS_Y), VAL(LIGHT_POS_Z), 0 };
	GLfloat lightDiffuse2[] = { 1, 1, 1, 1 };
	GLfloat lightSpecular2[] = { 0.7, 0.7, 0.7, 1 };

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpecular2);*/


	ModelerView::draw();
	Mat4f CameraMatrix = getModelViewMatrix();


	setAmbientColor(.1f, .1f, .1f);
	setSpecularColor(0.7, 0.7, 0.7);
	setShininess(100);
	// Body as root
	glPushMatrix();
	glTranslated(-2, 0, -1);
	if (VAL(MODEL_DETAIL) > 0) {
		if (VAL(ENDER_BODY) == 0) {
			setDiffuseColor(0.137, 0.82, 0.776);		// Upper body
			drawBox(4, 4, 2);
		}
		else {
			glPushMatrix();
			setDiffuseColor(COLOR_WHITE);		// ender body
			glTranslated(0, -2, 0);
			drawBox(4, 6, 2);
			glPopMatrix();
		}
	}
	// Head
	if (VAL(MODEL_DETAIL) >= 0) {
		glPushMatrix();
			if (VAL(ENDER_HEAD) == 0) {
				setDiffuseColor(0.961, 0.847, 0.561);
			}
			else {
				setDiffuseColor(COLOR_WHITE);
			}
			glTranslated(0, 4, -1);
			glTranslated(2, 0, 2);
			glRotated(VAL(HEAD_Y_ROTATE), 0.0, 1.0, 0.0);
			glRotated(VAL(HEAD_X_ROTATE), 1.0, 0.0, 0.0);
			glRotated(VAL(HEAD_Z_ROTATE), 0.0, 0.0, 1.0);
			glTranslated(-2, 0, -2);
			drawBox(4, 4, 4);				// head

		glPushMatrix();
			if (VAL(ENDER_HEAD) == 0) {
				setDiffuseColor(COLOR_WHITE);
				glTranslated(0.5, 1.5, 4);
				drawBox(1, 0.5, 0.05);				// Right Eye
				setDiffuseColor(COLOR_BLACK);
				glTranslated(0.25 + VAL(EYE_MOVEMENT), 0, 0.05);
				drawBox(0.5, 0.5, 0.05);
			}
			else {
				setDiffuseColor(COLOR_PURPLE);
				glTranslated(0.5, 1.5, 4);
				drawBox(1, 0.5, 0.05);				// Right Eye
				setDiffuseColor(COLOR_DARK_PURPLE);
				glTranslated(0.25 + VAL(EYE_MOVEMENT), 0, 0.05);
				drawBox(0.5, 0.5, 0.05);
			}
			//tears particle
			SpawnParticles(CameraMatrix, Vec3f(1, 1, 1), Vec3f(-10, 0, 1), 1);	//white colour
		glPopMatrix();
		glPushMatrix();
			if (VAL(ENDER_HEAD) == 0) {
				setDiffuseColor(COLOR_WHITE);
				glTranslated(2.5, 1.5, 4);
				drawBox(1, 0.5, 0.05);				// Left Eye
				setDiffuseColor(COLOR_BLACK);
				glTranslated(0.25 + VAL(EYE_MOVEMENT), 0, 0.05);
				drawBox(0.5, 0.5, 0.05);
			}
			else {
				setDiffuseColor(COLOR_PURPLE);
				glTranslated(2.5, 1.5, 4);
				drawBox(1, 0.5, 0.05);				// Left Eye
				setDiffuseColor(COLOR_DARK_PURPLE);
				glTranslated(0.25 + VAL(EYE_MOVEMENT), 0, 0.05);
				drawBox(0.5, 0.5, 0.05);
			}
			//tears particle
			SpawnParticles(CameraMatrix, Vec3f(1, 1, 1), Vec3f(10, 0, 1), 1);	//white colour
		glPopMatrix();
		glPopMatrix();
	}
	// Right Hand
	glPushMatrix();
		if (VAL(ENDER_HAND) == 0) {
			if (VAL(MODEL_DETAIL) > 1) {
				glTranslated(-2, 1.5, 0);
				setDiffuseColor(0.137, 0.82, 0.776);	// Right Upper arm
				glTranslated(1, 1.5, 1);
				glRotated(VAL(HANDS_MOVEMENT) + animation_angle, 1.0, 0.0, 0.0);
				glRotated(VAL(RU_HAND_Y_ROTATE), 0.0, 1.0, 0.0);
				glRotated(VAL(RU_HAND_X_ROTATE), 1.0, 0.0, 0.0);
				glRotated(VAL(RU_HAND_Z_ROTATE), 0.0, 0.0, 1.0);
				glTranslated(-1, -1.5, -1);
				drawBox(2, 2.5, 2);
			}
			if (VAL(MODEL_DETAIL) > 2) {
				glTranslated(0, -2.5, 0);
				setDiffuseColor(0.961, 0.847, 0.561);	// Right Lower arm
				glTranslated(0, 2.5, 1);
				glRotated(VAL(RL_HAND_X_ROTATE), 1.0, 0.0, 0.0);
				glTranslated(0, -2.5, -1);
				drawBox(2, 2.5, 2);
			}
			if (VAL(MODEL_DETAIL) > 6) {
				glPushMatrix();					// Right 4 fingers
					glTranslated(0.5, 0, 0);
					glTranslated(-0.25, 0, 0);
					glRotated(VAL(RU_FINGER_Z_ROTATE), 0.0, 0.0, 1.0);
					glTranslated(0.25, 0, 0);
					setDiffuseColor(0.886, 0.752, 0.416);
					drawBox(-0.5, -0.75, 2);

					glTranslated(0, -0.75, 0);
					glTranslated(-0.25, 0, 0);
					glRotated(VAL(RL_FINGER_Z_ROTATE), 0.0, 0.0, 1.0);
					glTranslated(0.25, 0, 0);
					drawBox(-0.5, -0.75, 2);
				glPopMatrix();

				glPushMatrix();					// Right thumb
					glTranslated(1.5, 0, 1.5);
					glTranslated(0.25, 0, 0);
					glRotated(VAL(RT_HAND_Z_ROTATE), 0.0, 0.0, 1.0);
					glTranslated(-0.25, 0, 0);
					setDiffuseColor(0.886, 0.752, 0.416);
					drawBox(0.5, -0.75, 0.5);
				glPopMatrix();
			}
		}
		else {
			if (VAL(MODEL_DETAIL) > 1) {
				glTranslated(-1, -10, 0.5);
				setDiffuseColor(COLOR_WHITE);	// Right Upper arm
				glTranslated(0.5, 13.5, 0.5);
				glRotated(VAL(HANDS_MOVEMENT) + animation_angle, 1.0, 0.0, 0.0);
				glRotated(VAL(RU_HAND_Y_ROTATE), 0.0, 1.0, 0.0);
				glRotated(VAL(RU_HAND_X_ROTATE), 1.0, 0.0, 0.0);
				glRotated(VAL(RU_HAND_Z_ROTATE), 0.0, 0.0, 1.0);
				glTranslated(-0.5, -13.5, -0.5);
				drawBox(1, 14, 1);
			}
		}

	glPopMatrix();

	// Left Hand
	glPushMatrix();
		if (VAL(ENDER_HAND) == 0) {
			if (VAL(MODEL_DETAIL) > 1) {
				glTranslated(4, 1.5, 0);
				setDiffuseColor(0.137, 0.82, 0.776);	// Left Upper arm
				glTranslated(1, 1.5, 1);
				glRotated(-(VAL(HANDS_MOVEMENT) + animation_angle), 1.0, 0.0, 0.0);
				glRotated(VAL(LU_HAND_Y_ROTATE), 0.0, 1.0, 0.0);
				glRotated(VAL(LU_HAND_X_ROTATE), 1.0, 0.0, 0.0);
				glRotated(VAL(LU_HAND_Z_ROTATE), 0.0, 0.0, 1.0);
				glTranslated(-1, -1.5, -1);
				drawBox(2, 2.5, 2);
			}
			if (VAL(MODEL_DETAIL) > 2) {
				glTranslated(0, -2.5, 0);
				setDiffuseColor(0.961, 0.847, 0.561);	// Left Lower arm
				glTranslated(0, 2.5, 1);
				glRotated(VAL(LL_HAND_X_ROTATE), 1.0, 0.0, 0.0);
				glTranslated(0, -2.5, -1);
				drawBox(2, 2.5, 2);
			}

			if (VAL(MODEL_DETAIL) > 6) {
				glPushMatrix();					// Left 4 fingers
					glTranslated(1.5, 0, 0);
					glTranslated(0.25, 0, 0);
					glRotated(VAL(LU_FINGER_Z_ROTATE), 0.0, 0.0, 1.0);
					glTranslated(-0.25, 0, 0);
					setDiffuseColor(0.886, 0.752, 0.416);
					drawBox(0.5, -0.75, 2);
					glTranslated(0, -0.75, 0);
					glTranslated(0.25, 0, 0);
					glRotated(VAL(LL_FINGER_Z_ROTATE), 0.0, 0.0, 1.0);
					glTranslated(-0.25, 0, 0);
					setDiffuseColor(0.886, 0.752, 0.416);
					drawBox(0.5, -0.75, 2);
				glPopMatrix();

				glPushMatrix();					// Left thumb
					glTranslated(0.5, 0, 1.5);
					glTranslated(-.25, 0, 0);
					glRotated(VAL(LT_HAND_Z_ROTATE), 0.0, 0.0, 1.0);
					glTranslated(0.25, 0, 0);
					setDiffuseColor(0.886, 0.752, 0.416);
					drawBox(-0.5, -0.75, 0.5);
				glPopMatrix();
			}
		}
		else {
			if (VAL(MODEL_DETAIL) > 1) {
				glTranslated(4, -10, 0.5);
				setDiffuseColor(COLOR_WHITE);	// Left Upper arm
				glTranslated(0.5, 13.5, 0.5);
				glRotated(-(VAL(HANDS_MOVEMENT) + animation_angle), 1.0, 0.0, 0.0);
				glRotated(VAL(LU_HAND_Y_ROTATE), 0.0, 1.0, 0.0);
				glRotated(VAL(LU_HAND_X_ROTATE), 1.0, 0.0, 0.0);
				glRotated(VAL(LU_HAND_Z_ROTATE), 0.0, 0.0, 1.0);
				glTranslated(-0.5, -13.5, -0.5);
				drawBox(1, 14, 1);
			}
		}
	glPopMatrix();

	// Lower
	glPushMatrix();
		if (VAL(MODEL_DETAIL) > 3) {
			glTranslated(0, -2, 0);
			if (VAL(ENDER_BODY) == 0) {
				setDiffuseColor(0.086, 0.514, 0.486);
				glTranslated(2, 2, 1);
				glRotated(VAL(WAIST_Y_ROTATE), 0.0, 1.0, 0.0);
				glRotated(VAL(WAIST_X_ROTATE), 1.0, 0.0, 0.0);
				glTranslated(-2, -2, -1);
				drawBox(4, 2, 2);				// Lower Body
			}
		}

		glPushMatrix();
		if (VAL(ENDER_LEG) == 0) {
			if (VAL(MODEL_DETAIL) > 4) {
				setDiffuseColor(0.223, 0.325, 0.776);
				glTranslated(0, -2.5, 0);		// Right Upper Leg
				glTranslated(1, 2.5, 1);
				glRotated(VAL(LEGS_MOVEMENT) - animation_angle, 1.0, 0.0, 0.0);
				glRotated(VAL(RU_LEG_X_ROTATE), 1.0, 0.0, 0.0);
				glRotated(VAL(RU_LEG_Y_ROTATE), 0.0, 1.0, 0.0);
				glRotated(VAL(RU_LEG_Z_ROTATE), 0.0, 0.0, 1.0);
				glTranslated(-1, -2.5, -1);
				drawBox(2, 2.5, 2);
			}
			if (VAL(MODEL_DETAIL) > 5) {
				glPushMatrix();
					glTranslated(0, -3.5, 0);		// Right Lower Leg + foot
					setDiffuseColor(0.223, 0.325, 0.776);
					glTranslated(1, 3.5, 1);
					glRotated(VAL(RL_LEG_X_ROTATE), 1.0, 0.0, 0.0);
					glRotated(VAL(RL_LEG_Y_ROTATE), 0.0, 1.0, 0.0);
					glRotated(VAL(RL_LEG_Z_ROTATE), 0.0, 0.0, 1.0);
					glTranslated(-1, -3.5, -1);
					drawBox(2, 3.5, 2);
				glPopMatrix();
			}
		}
		else {
			if (VAL(MODEL_DETAIL) > 2) {
				setDiffuseColor(COLOR_WHITE);
				glTranslated(0.5, -14, 0.5);		// Right Leg
				glTranslated(0.5, 13.5, 0.5);
				glRotated(VAL(LEGS_MOVEMENT) - animation_angle, 1.0, 0.0, 0.0);
				glRotated(VAL(RU_LEG_X_ROTATE), 1.0, 0.0, 0.0);
				glRotated(VAL(RU_LEG_Y_ROTATE), 0.0, 1.0, 0.0);
				glRotated(VAL(RU_LEG_Z_ROTATE), 0.0, 0.0, 1.0);
				glTranslated(-0.5, -13.5, -0.5);
				drawBox(1, 14, 1);
			}
		}
		////dust particle
		//SpawnParticles(CameraMatrix, Vec3f(1, 1, 1), Vec3f(0, 3, -5), 1);	//white colour
		glPopMatrix();
		glPushMatrix();
		if (VAL(ENDER_LEG) == 0) {
			if (VAL(MODEL_DETAIL) > 4) {
				setDiffuseColor(0.223, 0.325, 0.776);
				glTranslated(2, -2.5, 0);		// Left Upper Leg
				glTranslated(1, 2.5, 1);
				glRotated(-(VAL(LEGS_MOVEMENT) - animation_angle), 1.0, 0.0, 0.0);
				glRotated(VAL(LU_LEG_X_ROTATE), 1.0, 0.0, 0.0);
				glRotated(VAL(LU_LEG_Y_ROTATE), 0.0, 1.0, 0.0);
				glRotated(VAL(LU_LEG_Z_ROTATE), 0.0, 0.0, 1.0);
				glTranslated(-1, -2.5, -1);
				drawBox(2, 2.5, 2);
			}
			if (VAL(MODEL_DETAIL) > 5) {
				glTranslated(0, -3.5, 0);		// Left Lower Leg + foot
				setDiffuseColor(0.223, 0.325, 0.776);
				glTranslated(1, 3.5, 1);
				glRotated(VAL(LL_LEG_X_ROTATE), 1.0, 0.0, 0.0);
				glRotated(VAL(LL_LEG_Y_ROTATE), 0.0, 1.0, 0.0);
				glRotated(VAL(LL_LEG_Z_ROTATE), 0.0, 0.0, 1.0);
				glTranslated(-1, -3.5, -1);
				drawBox(2, 3.5, 2);
			}
		}
		else {
			if (VAL(MODEL_DETAIL) > 2) {
				setDiffuseColor(COLOR_WHITE);
				glTranslated(2.5, -14, 0.5);		// Left Leg
				glTranslated(0.5, 13.5, 0.5);
				glRotated(-(VAL(LEGS_MOVEMENT) - animation_angle), 1.0, 0.0, 0.0);
				glRotated(VAL(LU_LEG_X_ROTATE), 1.0, 0.0, 0.0);
				glRotated(VAL(LU_LEG_Y_ROTATE), 0.0, 1.0, 0.0);
				glRotated(VAL(LU_LEG_Z_ROTATE), 0.0, 0.0, 1.0);
				glTranslated(-0.5, -13.5, -0.5);
				drawBox(1, 14, 1);
			}
		}
		////dust particle
		//SpawnParticles(CameraMatrix, Vec3f(1, 1, 1), Vec3f(0, 3, -5), 1);	//white colour
		glPopMatrix();

	glPopMatrix();

	glPopMatrix();

	//*** DON'T FORGET TO PUT THIS IN YOUR OWN CODE **/
	endDraw();
}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
	ModelerControl controls[NUMCONTROLS];
	controls[MODEL_DETAIL] = ModelerControl("Model Detail", 0, 7, 1, 7);
	controls[ENDER_HEAD] = ModelerControl("Enderman Head", 0, 1, 1, 0);
	controls[ENDER_BODY] = ModelerControl("Enderman Body", 0, 1, 1, 0);
	controls[ENDER_HAND] = ModelerControl("Enderman Hand", 0, 1, 1, 0);
	controls[ENDER_LEG] = ModelerControl("Enderman Leg", 0, 1, 1, 0);
	controls[HANDS_MOVEMENT] = ModelerControl("Hands Movement", -90.0, 90.0, 1, 0);
	controls[LEGS_MOVEMENT] = ModelerControl("Legs Movement", -90.0, 90.0, 1, 0);
	controls[EYE_MOVEMENT] = ModelerControl("Eyes Movement", -0.25, 0.25, 0.01, 0);
	controls[HEAD_X_ROTATE] = ModelerControl("Head X-Rotate", -90, 90, 1, 0);
	controls[HEAD_Y_ROTATE] = ModelerControl("Head Y-Rotate", -90, 90, 1, 0);
	controls[HEAD_Z_ROTATE] = ModelerControl("Head Z-Rotate", -60, 60, 1, 0);
	controls[RU_HAND_X_ROTATE] = ModelerControl("Right Upper Hand X-Rotate", -180, 180, 1, 0);
	controls[RU_HAND_Y_ROTATE] = ModelerControl("Right Upper Hand Y-Rotate", -90, 90, 1, 0);
	controls[RU_HAND_Z_ROTATE] = ModelerControl("Right Upper Hand Z-Rotate", -180, 0, 1, 0);
	controls[RL_HAND_X_ROTATE] = ModelerControl("Right Lower Hand Z-Rotate", -135, 0, 1, 0);
	controls[RU_FINGER_Z_ROTATE] = ModelerControl("Right Upper Fingers Z-Rotate", 0, 65, 1, 22);
	controls[RL_FINGER_Z_ROTATE] = ModelerControl("Right Lower Fingers Z-Rotate", 0, 65, 1, 22);
	controls[RT_HAND_Z_ROTATE] = ModelerControl("Right Thumb Hand Z-Rotate", -65, 0, 1, -22);
	controls[LU_HAND_X_ROTATE] = ModelerControl("Left Upper Hand X-Rotate", -180, 180, 1, 0);
	controls[LU_HAND_Y_ROTATE] = ModelerControl("Left Upper Hand Y-Rotate", -90, 90, 1, 0);
	controls[LU_HAND_Z_ROTATE] = ModelerControl("Left Upper Hand Z-Rotate", 0, 180, 1, 0);
	controls[LL_HAND_X_ROTATE] = ModelerControl("Left Lower Hand Z-Rotate", -135, 0, 1, 0);
	controls[LU_FINGER_Z_ROTATE] = ModelerControl("Left Upper Fingers Z-Rotate", -65, 0, 1, -22);
	controls[LL_FINGER_Z_ROTATE] = ModelerControl("Left Lower Fingers Z-Rotate", -65, 0, 1, -22);
	controls[LT_HAND_Z_ROTATE] = ModelerControl("Left Thumb Hand Z-Rotate", 0, 65, 1, 22);
	controls[WAIST_X_ROTATE] = ModelerControl("Waist X-Rotate", -90, 90, 1, 0);
	controls[WAIST_Y_ROTATE] = ModelerControl("Waist Y-Rotate", -90, 90, 1, 0);
	controls[RU_LEG_X_ROTATE] = ModelerControl("Right Upper Leg X-Rotate", -120, 120, 1, 0);
	controls[RU_LEG_Y_ROTATE] = ModelerControl("Right Upper Leg Y-Rotate", -30, 30, 1, 0);
	controls[RU_LEG_Z_ROTATE] = ModelerControl("Right Upper Leg Z-Rotate", -60, 0, 1, 0);
	controls[RL_LEG_X_ROTATE] = ModelerControl("Right Lower Leg X-Rotate", 0, 90, 1, 0);
	controls[RL_LEG_Y_ROTATE] = ModelerControl("Right Lower Leg Y-Rotate", -15, 15, 1, 0);
	controls[RL_LEG_Z_ROTATE] = ModelerControl("Right Lower Leg Z-Rotate", -30, 30, 1, 0);
	controls[LU_LEG_X_ROTATE] = ModelerControl("Left Upper Leg X-Rotate", -120, 120, 1, 0);
	controls[LU_LEG_Y_ROTATE] = ModelerControl("Left Upper Leg Y-Rotate", -30, 30, 1, 0);
	controls[LU_LEG_Z_ROTATE] = ModelerControl("Left Upper Leg Z-Rotate", 0, 60, 1, 0);
	controls[LL_LEG_X_ROTATE] = ModelerControl("Left Lower Leg X-Rotate", 0, 90, 1, 0);
	controls[LL_LEG_Y_ROTATE] = ModelerControl("Left Lower Leg Y-Rotate", -15, 15, 1, 0);
	controls[LL_LEG_Z_ROTATE] = ModelerControl("Left Lower Leg Z-Rotate", -30, 30, 1, 0);
	controls[LIGHT_POS_X] = ModelerControl("Light X-Position", -30.0, 30.0, 0.1, 0.0);
	controls[LIGHT_POS_Y] = ModelerControl("Light Y-Position", -30.0, 30.0, 0.1, 5.0);
	controls[LIGHT_POS_Z] = ModelerControl("Light Z-Position", -30.0, 30.0, 0.1, 5.0);

	//set up forces
	int no_of_forces = 2;
	force** forces = new force*[no_of_forces];
	forces[0] = new gravity_force(Vec3f(0, -9.81, 0));
	forces[1] = new viscous_drag_force(1);

	//set up particle system
	ParticleSystem* ps = new ParticleSystem(forces, no_of_forces);
	ModelerApplication::Instance()->SetParticleSystem(ps);

	ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
	return ModelerApplication::Instance()->Run();
}
