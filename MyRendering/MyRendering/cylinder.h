#pragma once
#include"hitobject.h"
class cylinder :public hitobject {
public:
	//半径，只支持垂直的，也就是x和z构成圆形，y是高，center是底面的
	double radius;
	point3 center;
	double height;

	//材质信息
	shared_ptr<material> mat_ptr;
	cylinder() {}
	//构造
	cylinder(point3 c, double r, double h, shared_ptr<material> mat) {
		center = c;
		radius = r;
		height = h;
		mat_ptr = mat;
	}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_point& hpoint) const override {
		double tmp_t = infinity;
		bool updown = false;
		//先判断是否与上下表面相交
		vec3 direction = r.get_direction();
		if (direction.y() != 0)
		{
			//不等于才可能相交
			//上表面
			double up_t = (center.y() + height - r.get_origin.y()) / direction.y();
			bool upflag = false;
			bool downflag = false;
			double down_t = (center.y() - r.get_origin.y()) / direction.y();
			if (up_t >= 0 && up_t >= t_min && up_t <= t_max ){
				point3 inx = r.at_ray(up_t);
				if (inx.x() * inx.x() + inx.z() * inx.z() <= radius) {
					upflag = true;
					hpoint.t = up_t;
					hpoint.interaction_point = inx;
					hpoint.mat_ptr = mat_ptr;
					hpoint.normal = vec3(0, 1, 0);
				}
			}
			if (down_t >= 0 && down_t >= t_min && down_t <= t_max) {
				point3 inx = r.at_ray(down_t);
				if (inx.x() * inx.x() + inx.z() * inx.z() <= radius) {
					downflag = true;
					hpoint.t = down_t;
					hpoint.interaction_point = inx;
					hpoint.mat_ptr = mat_ptr;
					hpoint.normal = vec3(0, -1, 0);
				}
			}
			if (upflag && tmp_t > up_t)
				tmp_t = up_t;
			if (downflag && tmp_t > down_t)
				tmp_t = down_t;
			updown = (upflag || downflag);
		}
		double tmp_solve = infinity;
		bool cylinder_flag = false;
		double a = direction.x() * direction.x() + direction.z() * direction.z();
		double b = 2 * (direction.x() * (r.get_origin().x() - center.x()) + direction.z() * (r.get_origin().z() - center.z()));
		double c = (r.get_origin().x() - center.x())*(r.get_origin().x() - center.x()) + (r.get_origin().z() - center.z())*(r.get_origin().z() - center.z()) - radius * radius;
		if (a == 0) {
			if (b == 0) {
				cylinder_flag = false;
			}
			tmp_solve = -c / b;
			if (tmp_solve > t_min && tmp_solve <= t_max) {
				if (updown && tmp_solve < tmp_t) {
					cylinder_flag = true;
					hpoint.t = tmp_solve;
					hpoint.interaction_point = r.at_ray(tmp_solve);
					hpoint.mat_ptr = mat_ptr;
					hpoint.normal = vec3(0, -1, 0);
				}
			}
		}
		else {
			double discri = b * b - 4 * a * c;
					if (discri < 0) {
						cylinder_flag = false;
					}
					else {
						;
					}
		}
		
		return false;
	}
};