#include"util.h"
#include"color.h"
#include"camera.h"
#include"hitobject_list.h"
#include"sphere.h"
#include"material.h"
using namespace std;

color ray_color(const ray& r, hitobject_list world, double md) {
	if (md <= 0) {
		return color(0, 0, 0);
	}
	hit_point hpoint;
	if (world.hit(r, 0.001, infinity, hpoint)) {
		ray next_ray;
		color attenuation;
		if (hpoint.mat_ptr->rebound(r, hpoint, attenuation, next_ray))
			return attenuation * ray_color(next_ray, world, md - 1);
		return color(0, 0, 0);
		//漫反射衰减程度
		//double diffuse_factor = 0.5;
		//ray next_ray(hpoint.interaction_point, hpoint.normal + random_unit_vector());
		//return diffuse_factor * ray_color(next_ray, world, md - 1);
		//return 0.5 * (hpoint.normal + color(1, 1, 1));
	}
	vec3 unit_direction = unit_vector(r.get_direction());
	auto t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)*color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
	//return t*color(1.0, 1.0, 1.0) ;
}
int main() {
	//参数设定
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 10;
	const int max_depth = 10;
	//图片参数

	//相机参数
	point3 lookfrom(1, 1, 1);
	point3 lookat(0, 0, -1);
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 70, aspect_ratio);


	//世界参数
	hitobject_list world;
	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
	auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8));
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2));

	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
	//world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	//world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
	//开始画图，判断光线返回的颜色
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	//生成图像
	for (int j = image_height - 1; j >= 0; --j) {
		cerr << "Scanlines remaining " << j << ' ' << flush;
		for (int i = 0; i < image_width; ++i) {
			color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; s++) {
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}

			write_color(cout, pixel_color, samples_per_pixel);
		}
	}
}