#ifndef camera_h
#define camera_h

#include "./math.hpp"

struct Camera {
	Vector3f origin;
	Vector3f lowerLeftCorner;
	Vector3f horizontal;
	Vector3f vertical;
	
	Camera(float degVerticalFov, float aspect);
	Camera(Vector3f lookFrom, Vector3f lookAt, Vector3f up, float degVerticalFov, float aspect);
	
	Rayf rayFor(Vector2f uv) const;
};

#endif /* camera_h */
