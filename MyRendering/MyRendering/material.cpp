//#include "material.h"

//bool lambertian::rebound(const ray & r, hit_point & hpoint, ray & reback)
//{
//	vec3 reback_direction = hpoint.normal + random_unit_vector();
//		reback = ray(hpoint.interaction_point, reback_direction);
//		double test = dot(reback.get_direction(), hpoint.normal);
//		if (test > 0)
//			return true;
//		else
//			return false;
//}
