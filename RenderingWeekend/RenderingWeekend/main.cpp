#include"util.h"
#include"vec3.h"
#include"ray.h"
#include"color.h"
#include"hittable_list.h"
#include"sphere.h"
#include"camera.h"
#include<iostream>

using namespace std;




bool hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(oc, r.direction());
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = b * b - 4 * a*c;
	//cerr << "\nMy output " << discriminant << flush;
	return (discriminant > 0);
}
color ray_color(const ray& r, const hittable& world, int depth) {
	hit_record rec;

	if (depth <= 0) {
		return color(0, 0, 0);
	}
	if (world.hit(r, 0.001, infinity, rec)) {
		point3 target = rec.p + rec.normal + random_unit_vector();
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0, 0, 0);
		//return 0.5 * (rec.normal + color(1, 1, 1));
	}
	//if (hit_sphere(point3(0, 0, -1), 0.5, r))
	//	return color(1, 0, 0);
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)*color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}


int main() {
	//参数设计
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 10;

	// World
	hittable_list world;
	//world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	//world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
	auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8));
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2));

	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));


	//Camera
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0;

	auto origin = point3(0, 0, 0);
	auto horizontal = vec3(viewport_width, 0, 0);
	auto vertical = vec3(0, viewport_height, 0);
	auto low_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - vec3(0, 0, focal_length);
	camera cam;


	//Render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; --j) {
		cerr << "\rScanlines remaining " << j << ' ' << flush;
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