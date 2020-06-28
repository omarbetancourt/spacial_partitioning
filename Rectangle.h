#pragma once
#include "Point.h"

// This Rectangle class is the Axis-Aligned Boundary Box (AABB)
struct Rectangle
{
	Rectangle() = default;
	// contructed by passing in the center point of the box and the distance from the
	// center (Point center) to the edge (float halfDimension)
	Rectangle(Point in_center, float in_halfDim)
	{
		center = in_center;
		halfDimension = in_halfDim;
	}

	// If a point is contained within the AABB rectangle, return true.
	bool containsPoint(Point p) const
	{
		const float
			topBoundary = center.y + halfDimension,
			bottomBoundary = center.y - halfDimension,
			rightBoundary = center.x + halfDimension,
			leftBoundary = center.x - halfDimension;

		return (p.x < rightBoundary&&
				p.x > leftBoundary &&
				p.y > bottomBoundary &&
				p.y < topBoundary);
	}


	bool intersects(const Rectangle& other) const
	{
		const float
			topBoundary = center.y + halfDimension,
			bottomBoundary = center.y - halfDimension,
			rightBoundary = center.x + halfDimension,
			leftBoundary = center.x - halfDimension;

		return(!(other.center.x - other.halfDimension > rightBoundary ||
				 other.center.x + other.halfDimension < leftBoundary ||
				 other.center.y - other.halfDimension > topBoundary ||
				 other.center.x + other.halfDimension < bottomBoundary));
	}
	Point center;
	float halfDimension = 4;
};
