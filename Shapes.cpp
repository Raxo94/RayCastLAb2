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

MyPlane::~MyPlane()
{

}


void MyPlane::test(Ray& ray, HitData& hit)
{
	hit.lastNormal = n;
	static int counter = 0;
	if (counter > 200000)
	{
		//hit.t = 1;
		hit.lastNormal = n;
	}

	
	counter++;
	cout << counter;
	if (counter > 400000)
		counter = 0;

	

	
}

Vec MyPlane::normal(Vec &point)
{
	cout << "FuckYes";
	return point;
}

//Ading a comment here