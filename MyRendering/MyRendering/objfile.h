#pragma once
#include<vector>
#include<iostream>
#include<string>
#include <fstream>
#include"triangle.h"
class objfile :public hitobject {
public:
	//���������ε�������
	std::vector<triangle> meshs;

	//������Ϣ
	shared_ptr<material> mat_ptr;
	objfile() {}
	//����
	objfile(std::string filename, shared_ptr<material> mat) {
		mat_ptr = mat;
		std::vector<point3> vertices;
		std::ifstream in(filename.c_str());

		if (!in.good())
		{
			std::cerr << "ERROR: loading obj:(" << filename << ") file is not good" << "\n";
			exit(0);
		}

		char buffer[256], str[255];
		double f1, f2, f3;
		int i1, i2, i3;
		while (in.getline(buffer, 255))
		{
			//std::cerr << "\n" << buffer[0] << std::flush;
			buffer[255] = '\0';

			sscanf_s(buffer, "%s", str, 255);

			// reading a vertex  
			if (buffer[0] == 'v' && (buffer[1] == ' ' || buffer[1] == 32))
			{
				if (sscanf_s(buffer, "v %lf %lf %lf", &f1, &f2, &f3) == 3)
				{
					//�����˵�
					point3 vert(f1*2, f2*2, f3*2 - 1);
					vertices.push_back(vert);
				}
				else
				{
					std::cerr << "ERROR: vertex not in wanted format in OBJLoader" << "\n";
					exit(-1);
				}
			}
			// reading FaceMtls   
			else if (buffer[0] == 'f' && (buffer[1] == ' ' || buffer[1] == 32))
			{
				
				int nt = sscanf_s(buffer, "f %d %d %d", &i1, &i2, &i3);
				if (nt != 3)
				{
					std::cerr << "ERROR: I don't know the format of that FaceMtl" << "\n";
					exit(-1);
				}
				//����������Ƭ
				point3* creat = new point3[3];
				creat[0] = vertices[i1-1];
				creat[1] = vertices[i2-1];
				creat[2] = vertices[i3-1];
				triangle my(creat, mat_ptr);
				meshs.push_back(my);
			}
			//std::cerr << "\n finish read\n" << std::flush;
		}
		//std::cerr << "\n finish read\n" << std::flush;
	}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_point& hpoint) const override {
		//�����ཻ��
		double tmp_t = infinity;
		bool hitflag = false;
		hit_point tmp_hp;
		for (auto mesh : meshs)
		{
			if (mesh.hit(r, t_min, t_max, tmp_hp)) {
				hitflag = true;
				if (tmp_hp.t < tmp_t) {
					hpoint = tmp_hp;
					tmp_t = tmp_hp.t;
				}
			}
		}
		return hitflag;
	}
};