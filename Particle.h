#pragma once

std::random_device rdPar; // seed generator used to pass into rng to create different random numbers every time.
std::mt19937 rngPar(rdPar()); //outputs a random number (-2b, +2b), but outputs same sequence of random numbers every time
std::uniform_real_distribution<float> randWalk(-1.0f, 1.0f);

class Particle
{
public:
	Particle(int in_id, float in_x, float in_y, float in_r)
	{
		id = in_id;
		x = in_x;
		y = in_y;
		rSq = in_r;
		isColliding = false;
	}

	void move()
	{
		x += randWalk(rngPar);
		y += randWalk(rngPar);
	}

	bool intersects(Point other)
	{
		const float distSq = abs(pow((x - other.x), 2.0f) + pow((y - other.y), 2.0f));
		return (distSq < rSq);
	}

	bool isColliding = false;
	void begin();
	void end();
	int id;
	float x;
	float y;
	float rSq = 16.0f;
};