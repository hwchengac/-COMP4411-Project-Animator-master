#pragma warning(disable : 4786)

#include "particleSystem.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include "modelerdraw.h"


/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem(force** f, int n_f) 
{
	// TODO
	simulate = false;
	bake_start_time = -1;	//-1 for haven't started simulation
	bake_end_time = -2;		//-2 for haven't started simulation
	forces = f;
	no_of_forces = n_f;
}


/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO
	for (int i = 0; i < no_of_forces; ++i)
		delete forces[i];
	delete[] forces;

}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
    
	// TODO

	bake_start_time = t;
	clearBaked();

	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
    
	// TODO
	particles.clear();
	bake_end_time = t;

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    
	// TODO
	particles.clear();
	b_particles.clear();

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

bool ParticleSystem::isBaked(const float& t)
{
	bool flag = false;
	for (vector<baked_particles>::iterator iter = b_particles.begin(); iter != b_particles.end(); ++iter)
	{
		if (abs(iter->get_time() - t) < 1/ bake_fps)
//		if (iter->get_time() == t)
		{
			flag = true;
		}
	}
	//cout << "flag value: " << flag << endl;	//debug
	return flag;
}

void ParticleSystem::getBakedParticles(const float& t)
{
	for (vector<baked_particles>::iterator iter = b_particles.begin(); iter != b_particles.end(); ++iter)
	{
		if (abs(iter->get_time() - t) < 1 / bake_fps)
		{
			particles = iter->get_particles();
		}
	}
}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	// TODO
	if (simulate)
	{
		if (!isBaked(t))
		{
			for (vector<particle>::iterator iter = particles.begin(); iter != particles.end(); ++iter)
			{
				iter->update_force(forces, no_of_forces);
				iter->update_pos(1 / bake_fps);
			}
			bakeParticles(t);
		}
	}
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	// TODO
	if (bake_start_time != -1 && bake_end_time != -2)
	{
		if (bake_end_time != -1)
		{
			if (t >= bake_start_time && t <= bake_end_time)
				getBakedParticles(t);
			else
				particles.clear();
		}
	}

	for (vector<particle>::iterator iter = particles.begin(); iter != particles.end(); ++iter)
	{
		Vec3f position = iter->get_position();
		Vec3f colour = iter->get_colour();
		glPushMatrix();
		setDiffuseColor(colour[0], colour[1], colour[2]);	
		glTranslatef(position[0], position[1], position[2]);
		drawSphere(0.1);
		glPopMatrix();
	}
}

/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	// TODO
	b_particles.push_back(baked_particles(particles, t));
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	// TODO
	particles.clear();
	b_particles.clear();
}

void ParticleSystem::AddParticleStartingAt(Vec3f WorldPoint, Vec3f colour, Vec3f velocity)
{
	if (simulate)
		particles.push_back(particle(WorldPoint, velocity, Vec3f(0, 0, 0), 0.3, colour));
}




