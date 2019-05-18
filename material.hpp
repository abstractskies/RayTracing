#ifndef material_h
#define material_h

#include "./math.hpp"

struct Hit;

struct Material {
	virtual ~Material(){}
	virtual bool scatter(const Rayf& inRay, const Hit& hit, Vector3f& attenuation, Rayf& scattered) const =0;
};

struct DiffuseMaterial: public Material {
	Vector3f albedo;
	
	DiffuseMaterial(const Vector3f& a): albedo(a) {}
	
	bool scatter(const Rayf& inRay, const Hit& hit, Vector3f& attenuation, Rayf& scattered) const override;
};

struct MetalMaterial: public Material {
	Vector3f albedo;
	float fuzziness;
	
	MetalMaterial(const Vector3f& a, float f): albedo(a), fuzziness(f) {}
	
	bool scatter(const Rayf& inRay, const Hit& hit, Vector3f& attenuation, Rayf& scattered) const override;
};

struct DielectricMaterial: public Material {
	float refractiveIndex;
	
	DielectricMaterial(float i): refractiveIndex(i) {}
	
	bool scatter(const Rayf& inRay, const Hit& hit, Vector3f& attenuation, Rayf& scattered) const override;
};

#endif /* material_h */
