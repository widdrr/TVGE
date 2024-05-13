module Graphics.Components:CameraComponent;

import <glm/gtc/matrix_transform.hpp>;

import <cctype>;
import <algorithm>;
import <iostream>;

using namespace TVGE;
using namespace TVGE::Graphics;

CameraComponent::CameraComponent(Entity& p_entity,
								 float p_posX, float p_posY, float p_posZ,
								 float p_frontX, float p_frontY, float p_frontZ) :
	Component(p_entity),
	_relPosition(p_posX, p_posY, p_posZ),
	//roll not yet supported, not really needed
	_up(0.f, 1.f, 0.f),
	_front(p_frontX, p_frontY, p_frontZ),
	_yaw(),
	_pitch()
{
	_front = glm::normalize(_front);
	DetermineAngles();
}

const glm::mat4 CameraComponent::GetViewTransformation() const
{
	glm::vec3 position = entity.position + _relPosition;
	return glm::lookAt(position, position + _front, _up);
}

const glm::vec3 CameraComponent::GetPosition() const
{
	return entity.position + _relPosition;
}

const glm::vec3 TVGE::Graphics::CameraComponent::GetCameraFront() const
{
	return _front;
}

const glm::vec3 TVGE::Graphics::CameraComponent::GetCameraUp() const
{
	return _up;
}

void CameraComponent::MoveCamera(Movement p_movement, float p_delta) {
	
	float adjustedSpeed = _cameraSpeed * p_delta;

	if (p_movement.moveForward) {
		entity.position += _front * adjustedSpeed;
	}

	if (p_movement.moveBackward) {
		entity.position -= _front * adjustedSpeed;
	}

	if (p_movement.moveLeft) {
		entity.position -= glm::normalize(glm::cross(_front, _up)) * adjustedSpeed;
	}

	if (p_movement.moveRight) {
		entity.position += glm::normalize(glm::cross(_front, _up)) * adjustedSpeed;
	}

	if (p_movement.moveUp) {
		entity.position += _up * adjustedSpeed;
	}

	if (p_movement.moveDown) {
		entity.position -= _up * adjustedSpeed;
	}

}

void CameraComponent::SetCameraPosition(float p_posX, float p_posY, float p_posZ) {
	_relPosition.x = p_posX;
	_relPosition.y = p_posY;
	_relPosition.z = p_posZ;
}

void CameraComponent::SetCameraDirection(float p_frontX, float p_frontY, float p_frontZ) {

	_front.x = p_frontX;
	_front.y = p_frontY;
	_front.z = p_frontZ;

	DetermineAngles();
}

std::shared_ptr<Component> CameraComponent::Clone(Entity& p_entity) const
{
	auto component = p_entity.CreateComponentOfType<CameraComponent>().lock();
	component->_relPosition = _relPosition;
	component->_front = _front;
	component->_up = _up;
	component->_yaw = _yaw;
	component->_pitch = _pitch;

	return component;
}

void CameraComponent::RotateCamera(float p_offsetX, float p_offsetY) {

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

void CameraComponent::DetermineAngles()
{
	_pitch = glm::degrees(asinf(_front.y));
	_yaw = -glm::degrees(acosf(_front.x / cosf(_pitch)));
}