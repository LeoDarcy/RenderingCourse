#pragma once
#include"hitobject.h"
class sphere :public hitobject {
public:
	point3 center;
	double radius;
	//材质信息
	shared_ptr<material> mat_ptr;
	sphere(){}
	//省略，不能构造不带材质信息的素材
	sphere(point3 ct, double r) {
		center = ct;
		radius = r;
	}
	sphere(point3 ct, double r, shared_ptr<material> mat) {
		center = ct;
		radius = r;
		mat_ptr = mat;
	}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_point& hpoint) const override{
		vec3 out_direction = r.get_origin() - center;
		auto a = r.get_direction().length_squared();
		auto half_b = dot(out_direction, r.get_direction());
		auto c = out_direction.length_squared() - radius * radius;
		auto result = half_b * half_b - a * c;
		//判断是否有正解：
		if (result < 0)
			return false;
		//找最小的正解：
		auto sqrt_result = sqrt(result);
		auto solve = (-half_b - sqrt_result) / a;
		if (solve < t_min || solve > t_max) {
			solve = (-half_b + sqrt_result) / a;
			if (solve < t_min || solve > t_max)
				return false;
		}
		//找到了！
		hpoint.t = solve;
		hpoint.interaction_point = r.at_ray(solve);
		hpoint.normal = (hpoint.interaction_point - center) / radius;
		hpoint.set_face_normal(r, hpoint.normal);
		return true;
	}
};