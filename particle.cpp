 // SAMPLE_SOLUTION
#include "particle.h"

void particle::update_force(force** forces, const int no_of_forces)
{
	f = Vec3f(0, 0, 0);
	for (int i = 0; i < no_of_forces; ++i)
	{
		f += forces[i]->compute_force(m, v);
	}
}

void particle::update_pos(const float& delta_t)
{
	x += delta_t * v;
	v += delta_t * (f / m);
}
