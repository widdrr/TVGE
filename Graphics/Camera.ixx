export module Graphics.Components:Camera;

import Graphics.Resources;

import <glm/vec3.hpp>;
import <glm/mat4x4.hpp>;

//TODO: make Camera a component
export class Camera 
{
public:
	struct Movement {
		bool moveForward : 1;
		bool moveBackward : 1;
		bool moveLeft : 1;
		bool moveRight : 1;
		bool moveUp : 1;
		bool moveDown : 1;
	};

	Camera(float p_posX = 0.f, float p_posY = 0.f, float p_posZ = 0.f,
		float p_frontX = 0.f, float p_frontY = 0.f, float p_frontZ = -1.f);
	
	
	const glm::mat4 GetViewTransformation() const;

	void MoveCamera(Movement p_movement, float p_delta);
	void SetCameraPosition(float p_posX, float p_posY, float p_posZ);

	void RotateCamera(float p_offsetX, float p_offsetY);
	void SetCameraDirection(float p_frontX, float p_frontY, float p_frontZ);


private:
	void DetermineAngles();

	float _cameraSpeed = 5.f;
	float _cameraSensitivity = .05f;
	float _yaw, _pitch;
	glm::vec3 _position;
	glm::vec3 _front;
	glm::vec3 _up;
};