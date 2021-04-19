#pragma once
#include"hitobject.h"
class cylinder :public hitobject {
public:
	//�뾶��ֻ֧�ִ�ֱ�ģ�Ҳ����x��z����Բ�Σ�y�Ǹߣ�center�ǵ����
	double radius;
	point3 center;
	double height;

	//������Ϣ
	shared_ptr<material> mat_ptr;
	cylinder() {}
	//����
	cylinder(point3 c, double r, double h, shared_ptr<material> mat) {
		center = c;
		radius = r;
		height = h;
		mat_ptr = mat;
	}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_point& hpoint) const override {
		double tmp_t = infinity;
		bool updown = false;
		//���ж��Ƿ������±����ཻ
		vec3 direction = r.get_direction();
		if (direction.y() != 0)
		{
			//�����ڲſ����ཻ
			//�ϱ���
			point3 ori = r.get_origin();
			double up_t = (center.y() + height - ori.y()) / direction.y();
			bool upflag = false;
			bool downflag = false;
			double down_t = (center.y() - ori.y()) / direction.y();
			if (up_t >= 0 && up_t >= t_min && up_t <= t_max ){
				point3 inx = r.at_ray(up_t);
				if ((inx.x()-center.x()) * (inx.x() - center.x()) + (inx.z() - center.z()) * (inx.z() - center.z()) <= radius * radius) {
					upflag = true;
					hpoint.t = up_t;
					hpoint.interaction_point = inx;
					hpoint.mat_ptr = mat_ptr;
					hpoint.normal = vec3(0, 1, 0);
					hpoint.set_face_normal(r, hpoint.normal);
				}
			}
			if (down_t >= 0 && down_t >= t_min && down_t <= t_max) {
				point3 inx = r.at_ray(down_t);
				if ((inx.x() - center.x()) * (inx.x() - center.x()) + (inx.z() - center.z()) * (inx.z() - center.z()) <= radius * radius) {
					downflag = true;
					if (down_t < up_t) {
						hpoint.t = down_t;
						hpoint.interaction_point = inx;
						hpoint.mat_ptr = mat_ptr;
						hpoint.normal = vec3(0, -1, 0);
						hpoint.set_face_normal(r, hpoint.normal);
					}
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
					hpoint.normal = vec3((hpoint.interaction_point.x() - center.x()) / radius, 0, (hpoint.interaction_point.z() - center.z()) / radius);
					hpoint.set_face_normal(r, hpoint.normal);
				}
			}
		}
		else {
			double discri = b * b - 4 * a * c;
			if (discri < 0) {
				cylinder_flag = false;
			}
			else {
				double solve1 = (-b - sqrt(discri)) / (2 * a);
				double solve2 = (-b + sqrt(discri)) / (2 * a);
				if (solve1 > t_min && solve1 < t_max) {
					point3 inx = r.at_ray(solve1);
					if (inx.y() >= center.y() && inx.y() <= (center.y() + height)) {
						cylinder_flag = true;
						if (updown && solve1 >= tmp_t) {
							;//����hpoint
						}
						else if (solve1 < tmp_solve) {
							tmp_solve = solve1;
							hpoint.t = tmp_solve;
							hpoint.interaction_point = r.at_ray(tmp_solve);
							hpoint.mat_ptr = mat_ptr;
							hpoint.normal = vec3((hpoint.interaction_point.x() - center.x()) / radius, 0, (hpoint.interaction_point.z() - center.z()) / radius);
							hpoint.set_face_normal(r, hpoint.normal);
						}
					}
				}
				if (solve2 > t_min && solve2 < t_max) {
					point3 inx = r.at_ray(solve2);
					if (inx.y() >= center.y() && inx.y() <= (center.y() + height)) {
						cylinder_flag = true;
						if (updown && solve2 >= tmp_t) {
							;//����hpoint
						}
						else if (solve2 < tmp_solve) {
							tmp_solve = solve2;
							hpoint.t = tmp_solve;
							hpoint.interaction_point = r.at_ray(tmp_solve);
							hpoint.mat_ptr = mat_ptr;
							hpoint.normal = vec3((hpoint.interaction_point.x() - center.x()) / radius, 0, (hpoint.interaction_point.z() - center.z()) / radius);
							hpoint.set_face_normal(r, hpoint.normal);
						}
					}
				}
			}
		}
		if (updown || cylinder_flag)
			return true;
		return false;
	}
};