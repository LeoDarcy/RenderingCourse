#pragma once

#include"hitobject.h"
#include"hitobject_list.h"
struct hit_point;
class material {
public:
	virtual bool rebound(const ray &r, hit_point& hpoint, color &al, ray &reback)const = 0;
};

class lambertian : public material {
public:
	color albedo;
	lambertian(color al) {
		albedo = al;
	}
	bool rebound(const ray &r, hit_point& hpoint, color &al, ray &reback) const override {
		//��������õ����ߵķ���
		vec3 reback_direction = hpoint.normal + random_unit_vector();
		reback = ray(hpoint.interaction_point, reback_direction);
		al = albedo;
		double test = dot(reback.get_direction(), hpoint.normal);
		if (test > 0)
			return true;
		else
			return false;
	}
	//virtual bool rebound(const ray &r, hit_point& hpoint, ray &reback);

};

class metal :public material {
public:
	color albedo;
	metal(color al) {
		albedo = al;
	}
	bool rebound(const ray &r, hit_point& hpoint, color &al, ray &reback) const override {
		//���ݹ�ʽv - 2 * dot(v, n)*n�õ����ߵķ���
		vec3 reback_direction = r.get_direction() - 2 * dot(r.get_direction(), hpoint.normal)*hpoint.normal;
		reback = ray(hpoint.interaction_point, reback_direction);
		al = albedo;
		double test = dot(reback.get_direction(), hpoint.normal);
		//std::cerr << "\n" << reback.get_direction() << hpoint.normal <<"\n" << flush;;
		if (test > 0) {
			//std::cerr << "\ntrue\n" << flush;;
			return true;
		}
		else
			return false;
	}

};
