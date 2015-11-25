#include "Shapes.h"
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<math.h>
#include<memory.h>
using namespace std;


//Here starts Constuctors


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
	
	nor = edge0.Cross(edge1);
	nor.Normalize();

	c = _color;

}

MyOBB::MyOBB(Vec b, Vec b1, Vec b2, Vec b3, float Hu, float Hv, float Hw, Color _color)
{
	Bcenter=b;
	Bu=b1;
	Bv=b2;
	Bw=b3;

	Pu, Puo;
	Pv, Pvo;
	Pw, Pwo;

	halfBu=Hu;
	halfBv=Hv;
	halfBw=Hw;

	c = _color;
}

MyOBB::MyOBB(Vec b, float Hu, float Hv, float Hw, Color _color)
{
	Bcenter = b;
	halfBu = Hu;
	halfBv = Hv;
	halfBw = Hw;
	c = _color;
}

//End of Constructors Start of Test
 

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
			hit.lastNormal = normal(ray.o + ray.d*t);
			
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

				hit.lastNormal = nor;

			}
			
		}
	}
	
	




}

void MyOBB::test(Ray& ray, HitData& hit)

	{
		double E = pow(10, -20);
		double tmin = -INFINITY;
		double tmax = INFINITY;
		Vec P = Bcenter - ray.o;
		Vec BS[] = { Bu, Bv, Bw };
		float HS[] = { halfBu, halfBv, halfBw };

		for (int i = 0; i < 3; i++)
		{
			float e = BS[i].Dot(P);
			float f = BS[i].Dot(ray.d);

			if (abs(f) > E)
			{
				float t1 = (e + HS[i]) / f;
				float t2 = (e - HS[i]) / f;

				if (t1 > t2) { float temp = t1; t1 = t2; t2 = temp; }
				if (t1 > tmin) tmin = t1;
				if (t2 < tmax) tmax = t2;

				if (tmin > tmax) return;
				if (tmax < 0) return;

			}
			else if ((-e - HS[i]) > 0 || (-e + HS[i]) < 0) return;

		}
		if (hit.t > tmin || hit.lastShape == nullptr)
		{
			if (tmin > 0)
			{
				hit.t = tmin;
				hit.color = c;
				hit.lastShape = this;
				hit.lastNormal = normal(ray.o + ray.d * tmin);
			}
			else
			{
				hit.t = tmax;
				hit.color = c;
				hit.lastShape = this;
				hit.lastNormal = normal(ray.o + ray.d * tmin);
			}
			
		}


	}


//End of test Start of Normal


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

	Vec MyOBB::normal(Vec &point)
	{
	//träffen är H
	//S är normalen beroende på Face;
	//Få en S för varje ansikte:  Center +-(vector*halfDist)

		float E = 0.001;
		

		Vec BS[] = { Bu, Bv, Bw };
		float HS[] = { halfBu, halfBv, halfBw };
		Vec pPositive[] = { Pu,Pv,Pw };
		Vec pNegative[] = { Puo,Pvo,Pwo };

		for (int i = 0; i < 3;i++)
		{
			pPositive[i] = Bcenter + (BS[i] * HS[i]);
			pNegative[i] = Bcenter - (BS[i] * HS[i]);
		}

		for (int i = 0; i < 3;i++)
		{
			if ((abs(BS[i].Dot((point-pPositive[i]) ) ) ) < E )
			{
				return BS[i];
			}
			if ((abs(BS[i].Dot((point - pNegative[i]) ) ) ) < E)
			{
				return (BS[i] * -1);
			}
		}
		
		
	}
	

	//End of Normal Start of shading
	

	Color MyPlane::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
	{
		Vec ambientLight{ 50,50,50 };

		Vec normal = h.lastNormal;

		normal.Normalize();

		Vec LDirection = light - r.o;

		LDirection.Normalize();

		float diff = normal.Dot(LDirection);

		float Red = ((h.lastShape->c.r / 255.0) * (diff)+(h.lastShape->c.r / 255.0) * (ambientLight.x) / 255.0); //Divide by 255 to be able to multiply r is red
		float Green = (h.lastShape->c.g / 255.0) * (diff)+(h.lastShape->c.g / 255.0) * (ambientLight.y / 255.0);//g is green Ambiant light also has Color
		float Blue = (h.lastShape->c.b / 255.0) *(diff)+(h.lastShape->c.b / 255.0) * (ambientLight.z / 255.0);//b is blue


		Red = Red * 255.0;
		Green = Green * 255.0;
		Blue = Blue * 255.0;

		if (Red > 255.0)  Red = 255.0;
		if (Green > 255.0)  Green = 255.0;
		if (Blue > 255.0)  Blue = 255.0;

		Color color(Red, Green, Blue);

		return color;

	}

	Color MySphere::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
	{
		Vec ambientLight{ 50,50,50 };

		Vec normal = h.lastNormal;

		normal.Normalize();

		Vec LDirection = light - r.o;

		LDirection.Normalize();

		float diff = normal.Dot(LDirection);

		float Red = ((h.lastShape->c.r / 255.0) * (diff)+(h.lastShape->c.r / 255.0) * (ambientLight.x) / 255.0); //Divide by 255 to be able to multiply r is red
		float Green = (h.lastShape->c.g / 255.0) * (diff)+(h.lastShape->c.g / 255.0) * (ambientLight.y / 255.0);//g is green Ambiant light also has Color
		float Blue = (h.lastShape->c.b / 255.0) *(diff)+(h.lastShape->c.b / 255.0) * (ambientLight.z / 255.0);//b is blue


		Red = Red * 255.0;
		Green = Green * 255.0;
		Blue = Blue * 255.0;

		if (Red > 255.0)  Red = 255.0;
		if (Green > 255.0)  Green = 255.0;
		if (Blue > 255.0)  Blue = 255.0;

		Color color(Red, Green, Blue);

		return color;

	}
	
	Color MyTriangle::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
	{
		Vec ambientLight{ 50,50,50 };

		Vec normal = h.lastNormal;

		normal.Normalize();

		Vec LDirection = light - r.o;

		LDirection.Normalize();

		float diff = normal.Dot(LDirection);

		float Red = ((h.lastShape->c.r / 255.0) * (diff)+(h.lastShape->c.r / 255.0) * (ambientLight.x) / 255.0); //Divide by 255 to be able to multiply r is red
		float Green = (h.lastShape->c.g / 255.0) * (diff)+(h.lastShape->c.g / 255.0) * (ambientLight.y / 255.0);//g is green Ambiant light also has Color
		float Blue = (h.lastShape->c.b / 255.0) *(diff)+(h.lastShape->c.b / 255.0) * (ambientLight.z / 255.0);//b is blue


		Red = Red * 255.0;
		Green = Green * 255.0;
		Blue = Blue * 255.0;

		if (Red > 255.0)  Red = 255.0;
		if (Green > 255.0)  Green = 255.0;
		if (Blue > 255.0)  Blue = 255.0;

		Color color(Red, Green, Blue);

		return color;

	}

	Color MyOBB::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
	{
		Vec ambientLight{ 50,50,50 };
		
		Vec normal = h.lastNormal;

		normal.Normalize();

		Vec LDirection = light - r.o;

		LDirection.Normalize();

		float diff = normal.Dot(LDirection);

		float Red = ((h.lastShape->c.r / 255.0) * (diff)+(h.lastShape->c.r / 255.0) * (ambientLight.x) / 255.0); //Divide by 255 to be able to multiply r is red
		float Green = (h.lastShape->c.g / 255.0) * (diff)+(h.lastShape->c.g / 255.0) * (ambientLight.y / 255.0);//g is green Ambiant light also has Color
		float Blue = (h.lastShape->c.b / 255.0) *(diff)+(h.lastShape->c.b / 255.0) * (ambientLight.z / 255.0);//b is blue

		
		Red = Red * 255.0; 
		Green = Green * 255.0;
		Blue = Blue * 255.0;
		
		if (Red > 255.0)  Red = 255.0; 
		if (Green > 255.0)  Green = 255.0; 
		if (Blue > 255.0)  Blue = 255.0; 

		Color color(Red, Green, Blue);

		return color;

	}
