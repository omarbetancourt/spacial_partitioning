#include <cmath>
#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
//#include "Data.h"
#include "QuadTree.h"
#include "Particle.h"
#include <chrono>

//std::random_device rd; // seed generator used to pass into rng to create different random numbers every time.
std::mt19937 rng; //outputs a random number (-2b, +2b), but outputs same sequence of random numbers every time
std::uniform_real_distribution<float> xDis(0.0f, 400.0f), yDis(0.0f, 400.0f);

int main()
{
	auto start = std::chrono::high_resolution_clock::now();
	int nParticles = 1000;
	int maxSimTime = 500;
	unsigned int outputFile = 0;

	//Point center(200, 200);
	//Rectangle boundary(center, 220.0f);
	//Quadtree<Particle> qt(boundary);

	std::vector<Particle> particles;

	for (int par = 0; par < nParticles; par++) // Initializing pos and vel for swarms.
	{
		Particle new_particle(par, xDis(rng), yDis(rng), 4.0);
		particles.push_back(new_particle);
		// insertp point into quadtree here
	}

	for (int sim_time = 0; sim_time <= maxSimTime; ++sim_time)		// Each iteration is 0.001 sec
	{
		Point center(200, 200);
		Rectangle boundary(center, 220.0f);
		Quadtree<Particle> qt(boundary);

		for (unsigned int par = 0; par < particles.size(); par++)
		{
			Point point(particles[par].x, particles[par].y);
			Data<Particle> particleData(point, &particles[par]);
			qt.insert(particleData);
			particles[par].move();
		}


		for (unsigned int par = 0; par < particles.size(); par++)
		{
			Point pos(particles[par].x, particles[par].y);
			Rectangle rect(pos, particles[par].rSq);// arguments should be changed to p.x, p.y, range
			std::vector<Data<Particle>> othersData = qt.queryRange(rect); // one of these is itself
			for (unsigned int other = 0; other < othersData.size(); other++)
			{
				//Particle* other = otherData.load;// made a pointer to a data of a particle in range
				if (particles[par].id != othersData[other].load->id && particles[par].intersects(othersData[other].pos))// change other to type point from otherData.pos
				{
					particles[par].isColliding = true;
				}
			}
		}

		//for (unsigned int par = 0; par < particles.size(); par++)
		//{
		//	for (unsigned int other = 0; other < particles.size(); other++)
		//	{
		//		Point otherPos(particles[other].x, particles[other].y);
		//		if (particles[par].id != particles[other].id && particles[par].intersects(otherPos))// change other to type point from otherData.pos
		//		{
		//			particles[par].isColliding = true;
		//		}
		//	}
		//} 

		std::stringstream parOutputfile;
		parOutputfile << "results//parPos-" << outputFile << ".csv"; //outputs to results folder
		std::ofstream prntPar;
		prntPar.open(parOutputfile.str().c_str());
		for (unsigned int par = 0; par < particles.size(); par++)
		{
			prntPar << particles[par].x << "," << particles[par].y << std::endl;
		}
		prntPar.close();
		outputFile += 1;
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration<double>(stop - start);
	std::cout << "Elapsed time: " << duration.count() << std::endl;
}