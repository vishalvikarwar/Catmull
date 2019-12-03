#include <math.h>
#include <vector>
using namespace std;

//How many points you want on the curve
const int NO_OF_POINTS = 10;

//set from 0-1
const float ALPHA = 0.5f;

struct Vec3
{
	float x;
	float y;
	float z;
	Vec3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vec3(float i, float j, float k = 0.0f)
	{
		x = i;
		y = j;
		z = k;
	}
};

static Vec3 operator*(const float& scalar, const Vec3& v)
{
	return Vec3(v.x * scalar, v.y * scalar, v.z * scalar);
}

static Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

class CatmullSpline
{
public:
	CatmullSpline();
	~CatmullSpline();

	vector<Vec3> GetNewPoints();

private:

	Vec3 inputPoints[4];
	vector<Vec3> newPoints;

	void GeneratePoints();
	float GetT(float t, Vec3 p0, Vec3 p1);
};

CatmullSpline::CatmullSpline()
{
	inputPoints[0] = Vec3(0.0f, 0.0f);
	inputPoints[1] = Vec3(1.0f, 1.0f);
	inputPoints[2] = Vec3(2.0f, 3.0f);
	inputPoints[3] = Vec3(5.0f, 1.0f);
}

CatmullSpline::~CatmullSpline()
{
}

vector<Vec3> CatmullSpline::GetNewPoints()
{
	return newPoints;
}

void CatmullSpline::GeneratePoints()
{
	Vec3 p0 = inputPoints[0];
	Vec3 p1 = inputPoints[1];
	Vec3 p2 = inputPoints[2];
	Vec3 p3 = inputPoints[3];

	float t0 = 0.0f;
	float t1 = GetT(t0, p0, p1);
	float t2 = GetT(t1, p1, p2);
	float t3 = GetT(t2, p2, p3);

	for (float t = t1; t < t2; t += ((t2 - t1) / NO_OF_POINTS))
	{
		Vec3 A1 = (t1 - t) / (t1 - t0) * p0 + (t - t0) / (t1 - t0) * p1;
		Vec3 A2 = (t2 - t) / (t2 - t1) * p1 + (t - t1) / (t2 - t1) * p2;
		Vec3 A3 = (t3 - t) / (t3 - t2) * p2 + (t - t2) / (t3 - t2) * p3;

		Vec3 B1 = (t2 - t) / (t2 - t0) * A1 + (t - t0) / (t2 - t0) * A2;
		Vec3 B2 = (t3 - t) / (t3 - t1) * A2 + (t - t1) / (t3 - t1) * A3;

		Vec3 C = (t2 - t) / (t2 - t1) * B1 + (t - t1) / (t2 - t1) * B2;

		newPoints.push_back(C);
	}

}

float CatmullSpline::GetT(float t, Vec3 p0, Vec3 p1)
{
	float a = (float)(pow((p1.x - p0.x), 2.0f) + pow((p1.y - p0.y), 2.0f));
	float b = (float)pow(a, 0.5f);
	float c = (float)pow(b, ALPHA);
	return (c + t);
}