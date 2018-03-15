#ifndef VALUEVEC_H
#define VALUEVEC_H

#include <math.h>
#include <stdio.h>
#include "vec.h"

class ValueVec
{
public:

	float x, y, z, value;
	ValueVec();
	ValueVec(float aa, float bb, float cc, float dd);
	ValueVec(const ValueVec& other);
	ValueVec(const Vec3f& v, const float value);

	void operator = (const ValueVec& v);

	operator Vec3f() const;
};
#endif
