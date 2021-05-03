#ifndef GEOH
#define GEOH
#include "ray.h"
using namespace std;
class material;

typedef struct hit_record {
	float t;
	vec3 p;
	vec3 nv;
} hit_record;

class hitable { //geometry parent class
public:
	virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const = 0;
};

class sphere : public hitable {
public:
	sphere() {}
	sphere(vec3 c, float r, vec3 _kd = vec3(1.0, 1.0, 1.0), float w_ri = 0.0f, float w_ti = 0.0f):
		center(c), radius(r), kd(_kd), w_r(w_ri), w_t(w_ti) {};
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	vec3 center;
	float radius;
	vec3 kd;
	float w_r; //reflected
	float w_t; //transmitted
};

bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
	/*
	To-do:
		compute whether the ray intersect the sphere
	*/

	vec3 v = r.origin() - center;
	float DdotV = dot(r.direction(), v);
	float DdotD = dot(r.direction(), r.direction());
	float discriminant = DdotV * DdotV - DdotD * (dot(v, v) - radius * radius);

	if (discriminant >= 0) {

		float near_t = (-DdotV - sqrt(discriminant)) / DdotD;

		if (near_t < tmin || near_t >= tmax) {
			return false;
		}

		rec.t = near_t;
		rec.p = r.point_at_parameter(rec.t);
		rec.nv = unit_vector(rec.p - center);

		return true;
	}

	return false;
}

vec3 reflect(const vec3 &d, const vec3 &nv) {
	/*
	To-do:
		compute the reflect direction
	*/

	vec3 reflect_direction = (1 * d) - (2 * (dot(d, nv)) * nv);

	return reflect_direction;
}

vec3 refract(const vec3& d, const vec3& nv, float object_refraction_parameter) {
	/*
	To-do:
		compute the refracted(transmitted) direction
	*/

	float air_reflection_parameter = 1;
	float refraction_parameter = air_reflection_parameter / object_refraction_parameter;

	auto cos_theta = fmin(dot((-1 * d), nv), 1.0);
	auto cos_phi = sqrt(1 - refraction_parameter * refraction_parameter * (1 - cos_theta * cos_theta));

	vec3 refracted_direction = refraction_parameter * (1 * d + cos_theta * nv) - cos_phi * nv;

	return refracted_direction;
}

#endif