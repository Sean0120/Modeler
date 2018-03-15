#include "ValueVec.h"
ValueVec::ValueVec() : x(0), y(0), z(0), value(0)
{ }

ValueVec::ValueVec(float aa, float bb, float cc, float dd) :
	x(aa), y(bb), z(cc), value(dd)
{ }

ValueVec::ValueVec(const ValueVec& other) :
	x(other.x), y(other.y), z(other.z), value(other.value)
{ }

ValueVec::ValueVec(const Vec3f& v, const float value) :
	x(v[0]), y(v[1]), z(v[2]), value(value)
{ }

void ValueVec::operator = (const ValueVec& v)
{
	x = v.x; y = v.y; z = v.z;
	value = v.value;
}

ValueVec::operator Vec3f() const
{
	return Vec3f(x, y, z);
}