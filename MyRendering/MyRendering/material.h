#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include"hitobject.h"
#include"hitobject_list.h"
struct hit_point;
class material {
	virtual bool rebound(const ray &r, hit_point& hpoint, ray &reback) const = 0;
};

class lambertian : public material {
public:
	color albedo;
	lambertian(color al) {
		albedo = al;
	}
	virtual bool rebound(const ray &r, hit_point& hpoint, ray &reback){
		//加上随机得到光线的方向
		vec3 reback_direction = hpoint.normal + random_unit_vector();
		reback = ray(hpoint.interaction_point, reback_direction);
		double test = dot(reback.get_direction(), hpoint.normal);
		if (test > 0)
			return true;
		else
			return false;
	}
	//virtual bool rebound(const ray &r, hit_point& hpoint, ray &reback);

};

////class metal :public material {
//public:
//	color albedo;
//	metal(color al) {
//		albedo = al;
//	}
//	virtual bool rebound(const ray &r, const hit_point& hpoint, ray &reback)  {
//		跟据公式v - 2 * dot(v, n)*n得到光线的方向
//		vec3 reback_direction = r.get_direction() - 2 * dot(r.get_direction(), hpoint.normal)*hpoint.normal;
//		reback = ray(hpoint.interaction_point, reback_direction);
//		double test = dot(reback.get_direction(), hpoint.normal);
//		if (test > 0)
//			return true;
//		else
//			return false;
//	}
//
//};
#endif