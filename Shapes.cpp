#include "Shapes.h"
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;


//Assistant::Assistant(string typeName, int wage, string studyProgram, int pointTaken) : Employment(typeName, false, wage)

MyPlane::MyPlane(Vec normal, float _d, Color color) : Shape ()
{
	n = normal;
	d = _d;
	c = color;
}

MySphere::MySphere(Vec _center, float _radius, Color _color)
{
	center = _center;
	radius = _radius;
	c = _color;
}

MyPlane::~MyPlane()
{

}



void MyPlane::test(Ray& ray, HitData& hit)
{
	float t = (-d - (n.Dot(ray.o)) /n.Dot(ray.d));
	if (hit.lastShape == nullptr && t >= 0)
	{
		hit.t = t;
		hit.color = c;
		hit.lastShape = this;
		hit.lastNormal = n;

	}
	else if (t >= 0 && t <= hit.t)
	{
		hit.t = t;
		hit.color = c;
		hit.lastShape = this;
		hit.lastNormal = n;

	}
	

	
}

void  MySphere::test(Ray& ray, HitData& hit)
{

}

Vec MyPlane::normal(Vec &point)
{
	return n;
}

Vec MySphere::normal(Vec &point)
{
	Vec temp;
	return temp;
}

//Ading a comment here