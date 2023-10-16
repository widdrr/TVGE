#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
class Camera {

private:
	float _cameraSpeed = 5.f;
	float _cameraSensitivity = .05f;
	float _yaw, _pitch;
	glm::vec3 _position;
	glm::vec3 _front;
	glm::vec3 _up;

public:
	struct Movement {
		bool moveForward;
		bool moveBackward;
		bool moveLeft;
		bool moveRight;
		bool moveUp;
		bool moveDown;
	};

	Camera(float p_posX = 0.f, float p_posY = 0.f, float p_posZ = 0.f,
		float p_frontX = 0.f, float p_frontY = 0.f, float p_frontZ = -1.f,
		float p_upX = 0.f, float p_upY = 1.f, float p_upZ = 0.f);
	glm::mat4 GetViewTransformation() const;

	//TODO: put more thought if there's a better way to do this
	void MoveCamera(Movement p_movement, float p_delta);
	void PointCamera(float p_offsetX, float p_offsetY);
};

