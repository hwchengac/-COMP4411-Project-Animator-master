 // SAMPLE_SOLUTION
#ifndef PARTICLE_H
#define PARTICLE_H


#include "vec.h"
#include "force.h"
#include <vector>

class particle
{
	//data members
	private:
		Vec3f x;
		Vec3f v;
		Vec3f f;
		float m;
		Vec3f c;

	public:
		//constructor
		particle(const Vec3f& position, const Vec3f& velocity, const Vec3f& force, const float& mass, const Vec3f& colour)
		{
			x = position;
			v = velocity;
			f = force;
			m = mass;
			c = colour;
		}

		//getter
		Vec3f get_position() const { return x; }
		Vec3f get_velocity() const { return v; }
		Vec3f get_force() const { return f; }
		float get_mass() const { return m; }
		Vec3f get_colour() const { return c; }

		//setter
		void set_position(const Vec3f& position) { x = position; }
		void set_velocity(const Vec3f& velocity) { v = velocity; }
		void set_force(const Vec3f& force) { f = force; }
		void set_mass(const float& mass) { m = mass; }

		void update_force(force** forces, const int no_of_forces);
		void update_pos(const float& delta_t);
};

class baked_particles
{
	private:
		//data members
		vector<particle> particles;
		float time;

	public:
		//constructor
		baked_particles(vector<particle> p, float t) : particles(p), time(t) {}

		//getter
		float get_time() const { return time; }
		vector<particle> get_particles() { return particles; }
};

#endif // !PARTICLE_H