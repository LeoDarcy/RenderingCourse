#pragma once
#include"hitobject.h"
class sphere :public hitobject {
public:
	point3 center;
	double radius;
	//������Ϣ
	shared_ptr<material> mat_ptr;
	sphere(){}
	//ʡ�ԣ����ܹ��첻��������Ϣ���ز�
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
		//�ж��Ƿ������⣺
		if (result < 0)
			return false;
		//����С�����⣺
		auto sqrt_result = sqrt(result);
		auto solve = (-half_b - sqrt_result) / a;
		if (solve < t_min || solve > t_max) {
			solve = (-half_b + sqrt_result) / a;
			if (solve < t_min || solve > t_max)
				return false;
		}
		//�ҵ��ˣ�
		hpoint.t = solve;
		hpoint.interaction_point = r.at_ray(solve);
		hpoint.normal = (hpoint.interaction_point - center) / radius;
		hpoint.set_face_normal(r, hpoint.normal);
		return true;
	}
};