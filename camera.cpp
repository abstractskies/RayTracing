
#include "./camera.hpp"

Camera::Camera(float degVerticalFov, float aspect){
	const float theta = toRadians(degVerticalFov);
	const float halfHeight = tan(theta/2);
	const float halfWidth = aspect * halfHeight;
	
	lowerLeftCorner = {-halfWidth, -halfHeight, -1.0f};
	horizontal = {2 * halfWidth, 0.f, 0.f};
	vertical = {0.f, 2 * halfHeight, 0.f};
	origin = {0.f, 0.f, 0.f};
}

Camera::Camera(Vector3f lookFrom, Vector3f lookAt, Vector3f up, float degVerticalFov, float aspect){
	const float theta = toRadians(degVerticalFov);
	const float halfHeight = tan(theta/2);
	const float halfWidth = aspect * halfHeight;
	
	origin = lookFrom;
	const Vector3f w = (lookFrom - lookAt).normalized();
	const Vector3f u = cross(up, w).normalized();
	const Vector3f v = cross(w, u);
	
	lowerLeftCorner = origin - halfWidth * u - halfHeight * v - w;
	horizontal = 2 * halfWidth * u;
	vertical = 2 * halfHeight * v;
}

Rayf Camera::rayFor(Vector2f uv) const {
	return Rayf{
		origin, 
		lowerLeftCorner + uv.x * horizontal + uv.y * vertical - origin
	};
}
