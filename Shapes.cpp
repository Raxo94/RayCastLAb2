#include "Shapes.h"
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<math.h>
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

MyTriangle::MyTriangle(Vec _p1, Vec _p2, Vec _p3, Color _color)
{
	p1 = _p1;
	p2 = _p2;
	p3 = _p3;

	edge0 = p2 - p1;
	edge1 = p3 - p1;
	

	c = _color;

}


MyPlane::~MyPlane()
{

}



void MyPlane::test(Ray& ray, HitData& hit)
{
	float t = (-d - (n.Dot(ray.o)) /n.Dot(ray.d));
	
	
	if (hit.lastShape == nullptr && t >= 0 || (t >= 0 && t <= hit.t))
	{
		hit.t = t;
		hit.color = c;
		hit.lastShape = this;
		hit.lastNormal = n;

	}
	
	
}

void  MySphere::test(Ray& ray, HitData& hit)
{
	float t;
	float b = ray.d.Dot(ray.o - center);
	float c2 = (ray.o - center).Dot(ray.o - center) - (radius*radius);

	Vec op = center - ray.o;
	op.Normalize();
	
	//float t1 = -b + sqrt(b *b - c2);
	t = -b - sqrt(b *b - c2);

	


		if (hit.lastShape == nullptr &&t>=0 || t >= 0 && t <= hit.t)
		{
			hit.t = t;
			hit.color = c;
			hit.lastShape = this;

			Vec op = center- ray.o;
			op.Normalize();
			hit.lastNormal = op;

		}
		
}

void MyTriangle::test(Ray& ray, HitData& hit)
{
	
	Vec s= ray.o-p1;
	Vec temp;
	float t = temp.x = (1 / (-ray.d.Dot(edge0.Cross(edge1) ) ) ) * (s.Dot(edge0.Cross(edge1) ) ) ;
	float u = temp.y = (1 / (-ray.d.Dot(edge0.Cross(edge1) ) ) ) * (-ray.d.Dot(s.Cross(edge1) ) );
	if (u<=1 && u>=0)
	{
		float v = temp.y = (1 / (-ray.d.Dot(edge0.Cross(edge1)))) * (-ray.d.Dot(edge0.Cross(s)));

		if ( v<=1 && v>=0 && u + v <= 1 && u + v >= 0)
		{
			//Seing if t is closer than hit.t
			if (hit.lastShape == nullptr &&t >= 0 || t >= 0 && t <= hit.t)
			{ 
				hit.t = t;
				hit.color = c;
				hit.lastShape = this;

				Vec n;

				hit.lastNormal = n;
			}
			
		}
	}
	
	




}

Vec MyPlane::normal(Vec &point)
{
	return n;
}

Vec MySphere::normal(Vec &point)
{
	//op Sen normalize. Borde fixa det.
	Vec op = point - center;
	op.Normalize();
	return op;
}

//Ading a comment here