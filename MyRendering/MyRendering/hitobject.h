#pragma once

#include"ray.h"
#include"vec3.h"
#include"material.h"
using std::shared_ptr;
class material;

struct hit_point {
	point3 interaction_point;
	vec3 normal;
	double t;
	bool front_face;
	//属性
	shared_ptr<material> mat_ptr;
	//确定表面还是背面
	void set_face_normal(ray r, vec3 out_normal) {
		double result = dot(r.get_direction(), out_normal);
		if (result > 0) {
			front_face = false;
			normal = -out_normal;
		}
		else {
			front_face = true;
			normal = out_normal;
		}
	}
};

class hitobject {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_point& hpoint) const = 0;
};
