#pragma once
#include "Rectangle.h"

template <typename T>
// Data struct is data taken from a generic class T 
struct Data
{
	Data(Point pos = Point(), T* data = nullptr)
		:
		pos(pos),
		load(data)
	{
	}

	Point pos;
	T* load; // references a particle
};

template<class T>
class Quadtree
{
private:
	// Quadtree is referencing its 4 subsections/children
	Quadtree* nw;
	Quadtree* ne;
	Quadtree* sw;
	Quadtree* se;

	Rectangle boundary;

	//Objects made from Data class 
	std::vector<Data<T>> objects;

	static constexpr int capacity = 2;

public:
	Quadtree<T>();
	Quadtree<T>(Rectangle boundary);

	~Quadtree();

	bool insert(Data<T> d);
	void subdivide();
	std::vector< Data<T> > queryRange(Rectangle range);// change to circle range or ray
};

template<class T>
//need to add <T> after class name to tell c++ that the function's template parameter "T"
// is the same one we will be using for the class
Quadtree<T>::Quadtree():
	nw{ nullptr },
	ne{ nullptr },
	sw{ nullptr },
	se{ nullptr }
{
	boundary = Rectangle();
	objects = std::vector< Data<T> >();
}

template<class T>
Quadtree<T>::Quadtree(Rectangle boundary)
	:
	nw{ nullptr },
	ne{ nullptr },
	sw{ nullptr },
	se{ nullptr }
{
	objects = std::vector< Data<T> >();
	this->boundary = boundary;
}

template<class T>
Quadtree<T>::~Quadtree()
{
	delete nw;
	delete sw;
	delete ne;
	delete se;
}

template<class T>
bool Quadtree<T>::insert(Data<T> d)
{
	if (!boundary.containsPoint(d.pos))
	{
		return false;
	}

	if (objects.size() < capacity)
	{
		objects.push_back(d);
		return true;
	}

	if (nw == nullptr)
	{
		subdivide();
	}

	if (nw->insert(d))
	{
		return true;
	}
	if (ne->insert(d))
	{
		return true;
	}
	if (sw->insert(d))
	{
		return true;
	}
	if (se->insert(d))
	{
		return true;
	}
}

template<class T>
void Quadtree<T>::subdivide()
{
	float qSize = boundary.halfDimension;
	Point qCenter = Point(boundary.center.x - qSize, boundary.center.y - qSize);
	nw = new Quadtree(Rectangle(qCenter, qSize));

	qCenter = Point(boundary.center.x + qSize, boundary.center.y - qSize);
	ne = new Quadtree(Rectangle(qCenter, qSize));

	qCenter = Point(boundary.center.x - qSize, boundary.center.y + qSize);
	sw = new Quadtree(Rectangle(qCenter, qSize));

	qCenter = Point(boundary.center.x + qSize, boundary.center.y + qSize);
	se = new Quadtree(Rectangle(qCenter, qSize));
}

template<class T>
std::vector<Data<T>> Quadtree<T>::queryRange(Rectangle range)
{
	std::vector< Data<T> > pInRange = std::vector< Data<T> >();

	if (!boundary.intersects(range))
	{
		return pInRange;
	}

	for (int i = 0; i < objects.size(); i++)
	{
		if (range.containsPoint(objects.at(i).pos))
		{
			pInRange.push_back(objects.at(i));
		}
	}

	if (nw == nullptr)
	{
		return pInRange;
	}

	std::vector< Data<T> > temp = nw->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = ne->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = sw->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	temp = se->queryRange(range);
	pInRange.insert(pInRange.end(), temp.begin(), temp.end());

	return pInRange;
}
