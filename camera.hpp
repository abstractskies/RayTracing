#ifndef camera_h
#define camera_h

#include "./math.hpp"

struct Camera {
	Vector3f origin, lowerLeftCorner, horizontal, vertical;
	Vector3f u, v, w;
	float lensRadius;
	
	Camera(Vector3f lookFrom, Vector3f lookAt, Vector3f up, 
		   float degVerticalFov, float aspect, 
		   float aperture, float focusDistance);
	
	Rayf rayFor(Vector2f uv) const;
};

#endif /* camera_h */
