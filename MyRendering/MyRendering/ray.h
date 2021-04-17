#pragma once
#ifndef RAY_H
#define RAY_H
#include"vec3.h"
class ray
{
public:
	point3 origin;
	vec3 direction;
	ray() {}
	ray(const point3& ori, const vec3&dir) {
		origin = ori;
		direction = dir;
	}

	point3 get_origin() const { return origin; }
	vec3 get_direction() const { return direction; }

	point3 at_ray(double t) const {
		return origin + t * direction;
	}
private:

};

#endif