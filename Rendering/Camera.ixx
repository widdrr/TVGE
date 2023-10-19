export module Camera;

import <glm/vec3.hpp>;
import <glm/mat4x4.hpp>;

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
		float p_yaw = 90.f, float p_pitch = 0.f);
	
	const glm::vec3 GetPosition() const;
	const glm::mat4 GetViewTransformation() const;

	void MoveCamera(Movement p_movement, float p_delta);
	void PointCamera(float p_offsetX, float p_offsetY);
	
};

