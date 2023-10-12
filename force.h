#ifndef FORCE_H
#define FORCE_H

#include "vec.h"
class force
{
	public:
		virtual Vec3f compute_force(float m, Vec3f v) = 0;
};

class gravity_force : public force
{
	private:
		Vec3f g_vector = Vec3f(0, -3, 0);

	public:
		gravity_force(Vec3f g) { g_vector = g; }
		virtual Vec3f compute_force(float m, Vec3f v);

};

class viscous_drag_force : public force
{
	private:
		float k_constant = 1;

	public:
		viscous_drag_force(float k) { k_constant = k; }
		virtual Vec3f compute_force(float m, Vec3f v);
};
#endif