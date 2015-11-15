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
	cout<<"Hello";
}

MyPlane::~MyPlane()
{

}


void MyPlane::test(Ray& ray, HitData& hit)
{
	cout << "Hello";
}

Vec MyPlane::normal(Vec &point)
{
	cout << "FuckYes";
}

//Ading a comment here