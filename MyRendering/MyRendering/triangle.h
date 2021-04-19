#pragma once
#include"hitobject.h"
class triangle :public hitobject {
public:
	//���������ε�������
	point3* dots;
	
	//������Ϣ
	shared_ptr<material> mat_ptr;
	triangle() {}
	//����
	triangle(point3*d, shared_ptr<material> mat) {
		dots = d;
		mat_ptr = mat;
	}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_point& hpoint) const override {
		//�����ཻ��
		//���ҷ�����
		vec3 A = dots[0];
		vec3 B = dots[1];
		vec3 C = dots[2];
		vec3 AB = B - A;
		vec3 BC = C - A;
		//��֤�Ƿ���һ��ƽ��
		if ((AB.x() / BC.x() == AB.y() / BC.y()) && (AB.x() / BC.x() == AB.z() / BC.z())) {
			std::cerr << "\n \n Warning !!! Find triangle in a line \n\n" << std::flush;
			return false;
		}
		vec3 norm = unit_vector(cross(AB, BC));
		double poly_d = -(dot(norm, A));
		//�ҵ��ǵ�һ�����㣬���뷽��
		double vd = dot(norm, r.get_direction());
		double v0 = -(dot(norm, r.get_origin()) + poly_d);

		if (vd == 0) {//the ray is parallel to the polygon plane
			return false;
		}
		double solve_t = v0 / vd;
		

		//�жϵ��Ƿ�����������
		vec3 cross_point = r.at_ray(solve_t);
		if (PointInTriangle(A, B, C, cross_point) == false)
			return false;
		if (solve_t < t_min || solve_t > t_max) {
			return false;
		}
		//�ҵ��ˣ�
		hpoint.t = solve_t;
		hpoint.interaction_point = r.at_ray(solve_t);
		hpoint.normal = norm;
		hpoint.set_face_normal(r, hpoint.normal);
		hpoint.mat_ptr = mat_ptr;
		//std::cerr << "\n find triangle\n" << std::flush;
		return true;
	}
	/*~triangle()
	{
		delete[]dots;
	}*/
};