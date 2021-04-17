#pragma once
#ifndef HITOBJECT_LIST_H
#define HITOBJECT_LIST_H

#include"hitobject.h"
#include<memory>
#include<vector>
using std::shared_ptr;
using std::make_shared;
//记录的是面片群

class hitobject_list :public hitobject {
public:
	//保存所有的面片点
	std::vector<shared_ptr<hitobject>> objects;

	hitobject_list() {}
	hitobject_list(shared_ptr<hitobject> obs) {
		objects.push_back(obs);
	}

	void clear() {
		objects.clear();
	}

	void add(shared_ptr<hitobject> ob) {
		objects.push_back(ob);
	}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_point& hpoint) const override{
		hit_point tmp_hp;
		bool ishit = false;
		//记录最近的点
		double tmp_max = t_max;
		for (auto object : objects) {
			if (object->hit(r, t_min, tmp_max, tmp_hp)) {
				//每次更新最近的点
				ishit = true;
				tmp_max = tmp_hp.t;
				hpoint = tmp_hp;
			}
		}
		return ishit;
	}

};
#endif