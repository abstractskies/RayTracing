#ifndef hittable_h
#define hittable_h

#include "./math.hpp"

struct Material;

struct Hit {
	float t;
	Vector3f point;
	Vector3f normal;
	Material *material = nullptr;
};

struct Hittable {
	virtual ~Hittable();
	virtual bool hit(const Rayf&, float tMin, float tMax, Hit&) const = 0;
	Material *material;
	
protected:
	Hittable(Material *m);
};

struct Sphere: public Hittable {
	Vector3f center;
	float radius;
	
	Sphere(const Vector3f c, float r, Material *m)
	: Hittable(m), center(c), radius(r) {}
	
	bool hit(const Rayf& r, float tMin, float tMax, Hit& hit) const override;
};

#endif /* hittable_h */
