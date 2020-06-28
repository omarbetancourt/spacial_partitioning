#pragma once

// Holds an x and a y together.
struct Point
{
	Point()
	{
		x = 0;
		y = 0;
		//userdata = referencing a particle or any generic type
	}

	Point(float in_x, float in_y)
	{
		x = in_x;
		y = in_y;
	}

	float x, y;
};
