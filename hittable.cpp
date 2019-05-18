
#include "./hittable.hpp"
#include "./material.hpp"

#include <cassert>

// MARK: - Hittable
Hittable::Hittable(Material *m): material(m) {
}

Hittable::~Hittable(){
	delete material;
}

// MARK: - Sphere
bool Sphere::hit(const Rayf& r, float tMin, float tMax, Hit& hit) const {
	const Vector3f oc = r.origin - center;
	const float a = dot(r.direction, r.direction);
	const float b = 2.0f * dot(oc, r.direction);
	const float c = dot(oc, oc) - radius * radius;
	const float discriminant = b * b - 4 * a * c;
	
	if( discriminant > 0 ){
		const float t1 = (-b - sqrt(discriminant)) / (2.f * a);
		
		if( t1 < tMax && t1 > tMin ){
			hit.t = t1;
			hit.point = r.pointAt(t1);
			hit.normal = (hit.point - center) / radius;
			hit.material = material;
			return true;
		}
		
		const float t2 = (-b + sqrt(discriminant)) / (2.f * a);
		
		if( t2 < tMax && t2 > tMin ){
			hit.t = t2;
			hit.point = r.pointAt(t2);
			hit.normal = (hit.point - center) / radius;
			hit.material = material;
			return true;
		}
	}
	
	return false;
}
