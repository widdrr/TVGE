export module Graphics.Components:Camera;

import <glm/vec3.hpp>;
import <glm/mat4x4.hpp>;


//TODO: make Camera a component
export class Camera {

private:
	float _cameraSpeed = 5.f;
	float _cameraSensitivity = .05f;
	float _yaw, _pitch;
	glm::vec3 _position;
	glm::vec3 _front;
	glm::vec3 _up;

public:
	//TODO: use a bitfield for this, microoptimization ftw
	struct Movement {
		bool moveForward;
		bool moveBackward;
		bool moveLeft;
		bool moveRight;
		bool moveUp;
		bool moveDown;
	};

	Camera(float p_posX = 0.f, float p_posY = 0.f, float p_posZ = 0.f, 
		   float p_frontX = 0.f, float p_frontY = 0.f, float p_frontZ = -1.f);
	
	const glm::vec3 GetPosition() const;
	const glm::mat4 GetViewTransformation() const;

	void MoveCamera(Movement p_movement, float p_delta);
	void SetCameraPosition(float p_posX, float p_posY, float p_posZ);
	
	void RotateCamera(float p_offsetX, float p_offsetY);
	void SetCameraDirection(float p_frontX, float p_frontY, float p_frontZ);

private:

	void DetermineAngles();
	
};

