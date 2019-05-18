
#include "./camera.hpp"

Camera::Camera(Vector3f lookFrom, Vector3f lookAt, Vector3f up, 
			   float degVerticalFov, float aspect, 
			   float aperture, float focusDistance){
	lensRadius = aperture / 2;
	
	const float theta = toRadians(degVerticalFov);
	const float halfHeight = tan(theta/2);
	const float halfWidth = aspect * halfHeight;
	
	origin = lookFrom;
	w = (lookFrom - lookAt).normalized();
	u = cross(up, w).normalized();
	v = cross(w, u);
	
	lowerLeftCorner = origin - halfWidth * focusDistance * u - halfHeight * focusDistance * v - focusDistance * w;
	horizontal = 2 * halfWidth * focusDistance * u;
	vertical = 2 * halfHeight * focusDistance * v;
}

Rayf Camera::rayFor(Vector2f uv) const {
	Vector3f rd = lensRadius * randomInUnitDisk<float>();
	Vector3f offset = u * rd.x + v * rd.y;
	
	return Rayf{
		origin + offset,
		lowerLeftCorner + uv.x * horizontal + uv.y * vertical - origin - offset
	};
}
