#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include"vec3.h"
#include"ray.h"
#include"util.h"
extern double degrees_to_radians(double a);

class camera {
public:
	//参数
	point3 origin;
	vec3 low_left_corner;
	vec3 horizontal;
	vec3 vertical;
	double width;
	double height;
	//初始化。vfov是中间的fov角度，这里是度数
	camera(point3 lookfrom, point3 lookat, vec3 up, double vfov, double aspect_ratio) {
		origin = lookfrom;

		double angle = degrees_to_radians(vfov);
		double half_h = tan(angle / 2);
		height = 2 * half_h;
		width = aspect_ratio * height;
		//叉乘求坐标的方向
		auto w = unit_vector(lookfrom - lookat);
		auto u = unit_vector(cross(up, w));
		auto v = cross(w, u);

		horizontal = width * u;
		vertical = height * v;
		low_left_corner = origin - horizontal / 2 - vertical / 2 - w;
		//cerr << "Output " << horizontal << ' ' << vertical << ' ' << low_left_corner << ' ' << origin <<  flush;
		/*aspect_ratio = 16.0 / 9.0;
		auto viewport_height = 2.0;
		auto viewport_width = aspect_ratio * viewport_height;
		auto focal_length = 1.0;

		origin = point3(0, 0, 0);
		horizontal = vec3(viewport_width, 0.0, 0.0);
		vertical = vec3(0.0, viewport_height, 0.0);
		low_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
		cerr << "Output " << horizontal << ' ' << vertical << ' ' << low_left_corner << ' ' << origin << flush;
		*/
	}

	ray get_ray(double u, double v) const {
		return ray(origin, low_left_corner + u * horizontal + v * vertical - origin);
	}
};
#endif