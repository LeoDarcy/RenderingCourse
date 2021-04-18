#pragma once
//这个文件主要是完成一些基本的功能函数
#ifndef UTIL_H
#define UTIL_H
#include<iostream>
#include "vec3.h"

//#include "ray.h"
// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

inline double random_double() {
	// Returns a random real in [0,1).
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
	// Returns a random real in [min,max).
	return min + (max - min)*random_double();
}

double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

//用于判断三角面片的方法
bool sameside(vec3 A, vec3 B, vec3 C, vec3 P)
{
	vec3 AB = B - A;
	vec3 AC = C - A;
	vec3 AP = P - A;

	vec3 v1 = cross(AB, AC);
	vec3 v2 = cross(AB, AP);

	// v1 and v2 should point to the same direction
	return dot(v1, v2) >= 0;
}

// 同侧法
// 决定P是否在三角形ABC中
bool PointInTriangle(vec3 A, vec3 B, vec3 C, vec3 P)
{
	return sameside(A, B, C, P) && sameside(B, C, A, P) && sameside(C, A, B, P);
}

#endif