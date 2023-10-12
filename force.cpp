#include "force.h"

Vec3f gravity_force::compute_force(float m, Vec3f v)
{
	Vec3f new_force = m * g_vector;
	return new_force;
}

Vec3f viscous_drag_force::compute_force(float m, Vec3f v)
{
	Vec3f new_force = (-k_constant) * v;
	return new_force;
}