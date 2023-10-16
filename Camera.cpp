#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cctype>
#include <algorithm>
#include <iostream>

//TODO: reduce parameters (pos and yaw+pitch should be enough)
Camera::Camera(float p_posX, float p_posY, float p_posZ,
	float p_frontX, float p_frontY, float p_frontZ,
	float p_upX, float p_upY, float p_upZ) :
	_position(p_posX, p_posY, p_posZ),
	_front(p_frontX, p_frontY, p_frontZ),
	_up(p_upX, p_upY, p_upZ),
	_yaw(-90),
	_pitch()
{
	_front.x = cosf(glm::radians(_yaw)) * cosf(glm::radians(_pitch));
	_front.y = sinf(glm::radians(_pitch));
	_front.z = sinf(glm::radians(_yaw)) * cosf(glm::radians(_pitch));
	_front = glm::normalize(_front);
}

glm::mat4 Camera::GetViewTransformation() const {

	return glm::lookAt(_position, _position + _front, _up);
}

void Camera::MoveCamera(Movement p_movement, float p_delta) {
	
	float adjustedSpeed = _cameraSpeed * p_delta;

	if (p_movement.moveForward) {
		_position += _front * adjustedSpeed;
	}

	if (p_movement.moveBackward) {
		_position -= _front * adjustedSpeed;
	}

	if (p_movement.moveLeft) {
		_position -= glm::normalize(glm::cross(_front, _up)) * adjustedSpeed;
	}

	if (p_movement.moveRight) {
		_position += glm::normalize(glm::cross(_front, _up)) * adjustedSpeed;
	}

	if (p_movement.moveUp) {
		_position += _up * adjustedSpeed;
	}

	if (p_movement.moveDown) {
		_position -= _up * adjustedSpeed;
	}

}

//TODO: fix that one stutter bug
void Camera::PointCamera(float p_offsetX, float p_offsetY) {

	float adjustedSensitivity = _cameraSensitivity;

	_yaw += p_offsetX * adjustedSensitivity;
	_yaw = fmodf(_yaw + 360, 360.f);
	_pitch += p_offsetY * adjustedSensitivity;
	_pitch = std::clamp<float>(_pitch, -89.f, 89.f);

	_front.x = cosf(glm::radians(_yaw)) * cosf(glm::radians(_pitch));
	_front.y = sinf(glm::radians(_pitch));
	_front.z = sinf(glm::radians(_yaw)) * cosf(glm::radians(_pitch));
	_front = glm::normalize(_front);
}
