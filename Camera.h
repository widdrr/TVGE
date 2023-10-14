#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
class Camera {

private:
	float _cameraSpeed = 0.1f;
	glm::vec3 _position;
	glm::vec3 _front;
	glm::vec3 _up;

public:

	Camera(float p_posX = 0.f, float p_posY = 0.f, float p_posZ = 0.f,
		float p_frontX = 0.f, float p_frontY = 0.f, float p_frontZ = -1.f,
		float p_upX = 0.f, float p_upY = 1.f, float p_upZ = 0.f);
	glm::mat4 GetViewTransformation() const;

	void HandleKeyboard(unsigned char key, int x, int y);
};

