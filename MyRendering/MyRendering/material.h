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
		//加上随机得到光线的方向
		vec3 reback_direction = hpoint.normal + random_unit_vector();
		reback = ray(hpoint.interaction_point, reback_direction);
		al = albedo;
		return true;
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
		//跟据公式v - 2 * dot(v, n)*n得到光线的方向
		vec3 reback_direction = r.get_direction() - 2 * dot(r.get_direction(), hpoint.normal)*hpoint.normal;
		reback = ray(hpoint.interaction_point, reback_direction);
		al = albedo;
		double test = dot(reback.get_direction(), hpoint.normal);
		//std::cerr << "\n" << reback.get_direction() << hpoint.normal <<"\n" << flush;;
		return true;
	}

};

class dielectric: public material {
public:
	double refraction;
	color albedo;
	dielectric(double ir){
		refraction = ir;
	}

	bool rebound(const ray &r, hit_point& hpoint, color &al, ray &reback) const override {
		//因为没有损失
		al = color(1, 1, 1);
		//跟据公式求反射的方向
		vec3 ray_unit = unit_vector(r.get_direction());
		double cosine = dot(-ray_unit, hpoint.normal);
		if (cosine > 1)
			cosine = 1;
		double sine = sqrt(1 - cosine * cosine);

		double refraction_ratio = hpoint.front_face ? (1.0 / refraction) : refraction;
		bool cannot_refract = refraction_ratio * sine > 1.0;
		//cannot_refract = false;
		if (cannot_refract) {
			//镜面反射
			vec3 reback_direction = r.get_direction() - 2 * dot(r.get_direction(), hpoint.normal)*hpoint.normal;
			reback = ray(hpoint.interaction_point, reback_direction);
		}
		else {
			vec3 reback_perp = refraction_ratio * (ray_unit + cosine * hpoint.normal);
			vec3 reback_parallel = -sqrt(fabs(1.0 - reback_perp.length_squared())) * hpoint.normal;
			vec3 reback_direction = reback_perp + reback_parallel;
			reback = ray(hpoint.interaction_point, reback_direction);
		}
		return true;
	}

};