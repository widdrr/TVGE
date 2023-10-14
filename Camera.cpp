#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cctype>

Camera::Camera(float p_posX, float p_posY, float p_posZ, 
			   float p_frontX, float p_frontY, float p_frontZ, 
	           float p_upX, float p_upY, float p_upZ) : 
	_position(p_posX, p_posY, p_posZ),
	_front(p_frontX, p_frontY, p_frontZ),
	_up(p_upX, p_upY, p_upZ)
{}

glm::mat4 Camera::GetViewTransformation() const {

	return glm::lookAt(_position, _position + _front, _up);
}

void Camera::HandleKeyboard(unsigned char key, int x, int y) {

	switch (std::toupper(key)) {
		case 'W':
			_position += _front * _cameraSpeed;
			break;

		case 'S':
			_position -= _front * _cameraSpeed;
			break;
		
		case 'A':
			_position -= glm::normalize(glm::cross(_front, _up)) * _cameraSpeed;
			break;
		
		case 'D':
			_position += glm::normalize(glm::cross(_front, _up)) * _cameraSpeed;
			break;

		default:
			break;
	}
}
