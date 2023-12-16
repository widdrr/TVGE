module Graphics.Components:Camera;

import <glm/gtc/matrix_transform.hpp>;

import <cctype>;
import <algorithm>;
import <iostream>;

Camera::Camera(float p_posX, float p_posY, float p_posZ, float p_frontX, float p_frontY, float p_frontZ) :
	_position(p_posX, p_posY, p_posZ),
	//roll not yet supported, not really needed
	_up(0.f, 1.f, 0.f),
	_front(p_frontX, p_frontY, p_frontZ),
	_yaw(),
	_pitch()
{
	_front = glm::normalize(_front);
	DetermineAngles();
}

const glm::mat4 Camera::GetViewTransformation() const
{
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

void Camera::SetCameraPosition(float p_posX, float p_posY, float p_posZ) {
	_position.x = p_posX;
	_position.y = p_posY;
	_position.z = p_posZ;
}

void Camera::SetCameraDirection(float p_frontX, float p_frontY, float p_frontZ) {

	_front.x = p_frontX;
	_front.y = p_frontY;
	_front.z = p_frontZ;

	DetermineAngles();
}

void Camera::RotateCamera(float p_offsetX, float p_offsetY) {

	float adjustedSensitivity = _cameraSensitivity;

	//_yaw is in [0,360) to prevent floating point errors
	_yaw += p_offsetX * adjustedSensitivity;
	_yaw = fmodf(_yaw + 360, 360.f);

	//_pitch is in (-90, 90) to prevent gimbal locks and other weird stuff
	_pitch += p_offsetY * adjustedSensitivity;
	_pitch = std::clamp<float>(_pitch, -89.9f, 89.9f);

	_front.x = cosf(glm::radians(_yaw)) * cosf(glm::radians(_pitch));
	_front.y = sinf(glm::radians(_pitch));
	_front.z = sinf(glm::radians(_yaw)) * cosf(glm::radians(_pitch));
	_front = glm::normalize(_front);
}

void Camera::DetermineAngles()
{
	_pitch = glm::degrees(asinf(_front.y));
	_yaw = -glm::degrees(acosf(_front.x / cosf(_pitch)));
}