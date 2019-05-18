
#include "./material.hpp"
#include "./hittable.hpp"

bool DiffuseMaterial::scatter(const Rayf& inRay, const Hit& hit, Vector3f& attenuation, Rayf& scattered) const {
	Vector3f target = hit.point + hit.normal + randomInUnitSphere<float>();
	scattered = Rayf{hit.point, target - hit.point};
	attenuation = albedo;
	return true;
}

bool MetalMaterial::scatter(const Rayf& inRay, const Hit& hit, Vector3f& attenuation, Rayf& scattered) const {
	Vector3f reflected = reflect(inRay.direction.normalized(), hit.normal);
	scattered = Rayf{hit.point, reflected + fuzziness * randomInUnitSphere<float>()};
	attenuation = albedo;
	return dot(scattered.direction, hit.normal) > 0;
}

namespace {
	bool refract(const Vector3f& v, const Vector3f& n, float niOverNt, Vector3f& refracted){
		const Vector3f uv = v.normalized();
		const float dt = dot(uv, n);
		const float discriminant = 1.0f - niOverNt*niOverNt * (1.f - dt*dt);
		
		if( discriminant > 0.f ){
			refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
			return true;
		}
		
		return false;
	}
	
	float schlick(float cosine, float refractiveIndex){
		float r0 = (1 - refractiveIndex) / (1 + refractiveIndex);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow(1 - cosine, 5);
	}
}

bool DielectricMaterial::scatter(const Rayf& inRay, const Hit& hit, Vector3f& attenuation, Rayf& scattered) const {
	Vector3f outwardNormal;
	Vector3f reflected = reflect(inRay.direction, hit.normal);
	float niOverNt;
	attenuation = {1.f, 1.f, 1.f};
	Vector3f refracted;
	float reflectionProbability;
	float cosine;
	
	if( dot(inRay.direction, hit.normal) > 0.f ){
		outwardNormal = -hit.normal;
		niOverNt = refractiveIndex;
		cosine = refractiveIndex * dot(inRay.direction, hit.normal) / inRay.direction.length();
	} else {
		outwardNormal = hit.normal;
		niOverNt = 1.f / refractiveIndex;
		cosine = -dot(inRay.direction, hit.normal);
	}
	
	if( refract(inRay.direction, outwardNormal, niOverNt, refracted) ){
		reflectionProbability = schlick(cosine, refractiveIndex);
	} else {
		scattered = Rayf{hit.point, reflected};
		reflectionProbability = 1.0f;
	}
	
	if( drand48() < reflectionProbability )
		scattered = Rayf{hit.point, reflected};
	else
		scattered = Rayf{hit.point, refracted};
	
	return true;
}
